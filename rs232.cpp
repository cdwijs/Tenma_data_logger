#include "rs232.h"

#include <QDebug>

#define TIME_LED 100
const unsigned char RS232_TERMINATION_CHAR=0x3B;
const unsigned char RS232_CR=0x0D;


RS232::RS232(QWidget *parent)
{
    //qDebug()<<Q_FUNC_INFO;
#ifdef FAKE_MESSAGES
    myTimer = new QTimer();
    connect(myTimer,&QTimer::timeout,this,&RS232::slotTimer);
    myTimer->setInterval(500);
    myTimer->start();
#endif
    mySettingsDia = new SettingsDialog;
    myRxQueue = new QQueue<unsigned char>;
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

    myHlayout = new QSplitter();
    myHlayout->setChildrenCollapsible(false);
    myHlayout->setOrientation(Qt::Horizontal);
    myHlayout->addWidget(myLabel);
    myHlayout->addWidget(myTxLED);
    myHlayout->addWidget(myRxLED);
    myHlayout->addWidget(mySettingsBtn);
    myHlayout->addWidget(myConnectBtn);

    connect(mySettingsBtn,&QPushButton::clicked,this,&RS232::slotSettings);
    connect(myConnectBtn,&QPushButton::clicked,this,&RS232::slotConnect);
    connect(mySerialPort,&QSerialPort::readyRead,this,&RS232::slotRx);
    connect(myRxTimer,&QTimer::timeout,this,slotRxTimer);
    connect(myTxTimer,&QTimer::timeout,this,slotTxTimer);
    myHlayout->setParent(parent);
    myHlayout->show();
}

RS232::slotSettings(bool clicked)
{
    mySettingsDia->show();
}

RS232::slotConnect(bool clicked)
{
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
    else
    {
        QMessageBox::critical(this, tr("Error"), mySerialPort->errorString());
        //qDebug(tr("Open error"));
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

RS232::slotDisconnect(bool clicked)
{
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

RS232::slotMessage(QString string)
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

RS232::slotRx()
{
    const SettingsDialog::Settings p = mySettingsDia->settings();
    //qDebug()<<"slotRx";
    int numstrings=0;
    bool endofstringseen=false;
    myRxLED->setState(true);
    myRxTimer->start(TIME_LED);
    const QByteArray data = mySerialPort->readAll();
    for (int i = 0; i < data.size(); ++i)
    {
        unsigned char rxchar;
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
        QByteArray queuedata;
        queuedata.clear();
        int len = myRxQueue->length();
        for (int j = 0; j < (len); ++j)
        {
            unsigned char rxchar;
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

RS232::slotRxTimer()
{
    myRxLED->setState(false);
}

RS232::slotTxTimer()
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
