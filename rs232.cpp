#include "rs232.h"

#include <QDebug>

#define TIME_LED 100
const unsigned char RS232_TERMINATION_CHAR=0x3B;
const unsigned char RS232_CR=0x0D;


RS232::RS232(QWidget *parent) : QWidget(parent)
{
    //qDebug()<<Q_FUNC_INFO;
#ifdef FAKE_MESSAGES
    myTimer = new QTimer();
    connect(myTimer,&QTimer::timeout,this,&RS232::slotTimer);
    myTimer->setInterval(500);
    myTimer->start();
#endif
    mySettingsDia = new SettingsDialog;
    myRxQueue = new QQueue<char>;
    myRxQueue->clear();

    mySerialPort = new QSerialPort ;
    myTxTimer = new QTimer();
    myTxTimer->setSingleShot(true);
    myRxTimer = new QTimer();
    myRxTimer->setSingleShot(true);

    myLabel = new QLabel("COM-");

    mySettingsBtn = new QPushButton;
    mySettingsBtn->setIcon(QIcon(":/images/wrench.png"));
    mySettingsBtn->setToolTip("Settings");

    myConnectBtn = new QPushButton;
    myConnectBtn->setIcon(QIcon(":/images/disconnected.png"));
    myConnectBtn->setToolTip("Connect");

    const int LED_SIZE = 10;
    myRxLED = new LedIndicator();
    myRxLED->setLedSize(LED_SIZE);
    myRxLED->setOnColor(Qt::green);
    myRxLED->setToolTip("Rx");

    myTxLED = new LedIndicator();
    myTxLED->setLedSize(LED_SIZE);
    myTxLED->setOnColor(Qt::green);
    myTxLED->setToolTip("Tx");

    myHlayout = new QHBoxLayout();
    myHlayout->addWidget(myLabel);
    myHlayout->addWidget(myTxLED);
    myHlayout->addWidget(myRxLED);
    myHlayout->addWidget(mySettingsBtn);
    myHlayout->addWidget(myConnectBtn);

    connect(mySettingsBtn,&QPushButton::clicked,this,&RS232::slotSettings);
    connect(myConnectBtn,&QPushButton::clicked,this,&RS232::slotConnect);
    connect(mySerialPort,&QSerialPort::readyRead,this,&RS232::slotRx);
    connect(myRxTimer,&QTimer::timeout,this,&RS232::slotRxTimer);
    connect(myTxTimer,&QTimer::timeout,this,&RS232::slotTxTimer);
    setLayout(myHlayout);
}

void RS232::slotSettings(bool clicked)
{
    Q_UNUSED(clicked);
    mySettingsDia->show();
}

void RS232::slotConnect(bool clicked)
{
    Q_UNUSED(clicked);
    const SettingsDialog::Settings p = mySettingsDia->settings();
    if(mySerialPort->isOpen())
    {
        mySerialPort->close();
    }
    mySerialPort->setPortName(p.name);
    mySerialPort->setBaudRate(p.baudRate);
    mySerialPort->setDataBits(QSerialPort::Data8);
    mySerialPort->setParity(QSerialPort::NoParity);
    mySerialPort->setStopBits(QSerialPort::OneStop);
    mySerialPort->setFlowControl(QSerialPort::NoFlowControl);
    if (mySerialPort->open(QIODevice::ReadWrite))
    {
        emit sigOpenChanged(true);
    }

    if (mySerialPort->error() != QSerialPort::NoError)
    {
        QString string;
        string = mySerialPort->errorString();
        string.prepend(p.name);
        string.prepend("Serial Port Error: ");
        myLabel->setText(string);
    }
    else
    {
        if(mySerialPort->isOpen())
        {
            myLabel->setText(p.name);
        }
        else
        {
            myLabel->setText("COM-");
        }
    }
}

void RS232::slotDisconnect(bool clicked)
{
    Q_UNUSED(clicked);
    const SettingsDialog::Settings p = mySettingsDia->settings();
    if(mySerialPort->isOpen())
    {
        mySerialPort->close();
    }
    if(mySerialPort->isOpen())
    {
        myLabel->setText(p.name);
    }
    else
    {
        myLabel->setText("COM-");
    }
}

void RS232::slotMessage(QString string)
{
    //qDebug()<<string;
    QByteArray mydata;
    mydata.resize(0);
    mydata.append(string);
    if(mySerialPort->isOpen())
    {
        mySerialPort->write(mydata);
        myTxLED->setState(true);
        myTxTimer->start(TIME_LED);
    }
    else
    {
        //emit error signal?
    }
}

void RS232::slotRx(void)
{
    const SettingsDialog::Settings p = mySettingsDia->settings();
    //qDebug()<<"slotRx";
    int numstrings=0;
    myRxLED->setState(true);
    myRxTimer->start(TIME_LED);
    const QByteArray data = mySerialPort->readAll();
    for (int i = 0; i < data.size(); ++i)
    {
        char rxchar;
        rxchar = data.at(i);
        QString str(rxchar);
        if (p.terminators.contains(str))
        {
            numstrings++;
        }
        myRxQueue->enqueue(rxchar);
    }
    if (numstrings)
    {
        QString rxstring;
        rxstring.clear();
        int len = myRxQueue->length();
        for (int j = 0; j < (len); ++j)
        {
            char rxchar;
            rxchar = myRxQueue->dequeue();
            rxstring.append(rxchar);

            QString str(rxchar);
            if (p.terminators.contains(str))
            {
                emit sigReceived(rxstring);
                rxstring.clear();
                numstrings--;
                if (numstrings==0)
                { //leave partial messages in the buffer
                    break;
                }
            }
        }
    }
}

void RS232::slotRxTimer(void)
{
    myRxLED->setState(false);
}

void RS232::slotTxTimer(void)
{
    myTxLED->setState(false);
}

#ifdef FAKE_MESSAGES
RS232::slotTimer()
{
    QString string("\COMMUNICATION");
    string.append(0x0D); //is \r  (\n is 0x0A)
    slotMessage(string);
}
#endif
