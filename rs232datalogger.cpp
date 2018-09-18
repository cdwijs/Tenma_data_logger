#include "rs232datalogger.h"
#include <QDebug>

const int MAX_LOG_BLOCKS=100000;


RS232DATALOGGER::RS232DATALOGGER(QWidget *parent) : QWidget(parent)
{
   //qDebug()<<Q_FUNC_INFO;

    myRxLabel = new QLabel("Rx");
    myTxLabel = new QLabel("Tx");
    myClearBtn = new QPushButton();
    myClearBtn->setIcon(QIcon(":/images/delete.png"));
    myClearBtn->setToolTip("Clear");
    myLog = new QPlainTextEdit();
    myHlayout = new QSplitter();
    myHlayout->setChildrenCollapsible(false);
    myHlayout->setOrientation(Qt::Horizontal);
    myHlayout->addWidget(myRxLabel);
    rs232Rx = new RS232(myHlayout);
    myHlayout->addWidget(rs232Rx);
    myHlayout->addWidget(myTxLabel);
    rs232Tx = new RS232(myHlayout);
    myHlayout->addWidget(rs232Tx);
    myVlayout = new QSplitter();
    myVlayout->setChildrenCollapsible(false);
    myVlayout->setOrientation(Qt::Vertical);
    myHlayout->addWidget(myClearBtn);
    myFileWriter = new FileWriter(myHlayout);
    myVlayout->addWidget(myLog);
    myVlayout->addWidget(myHlayout);
    myVlayout->show();

    myLog->setMaximumBlockCount(MAX_LOG_BLOCKS); //one block per hard line break
    myLog->setReadOnly(true);

    connect(rs232Rx,&RS232::sigReceived,this,&RS232DATALOGGER::slotParseRx);
    connect(rs232Tx,&RS232::sigReceived,this,&RS232DATALOGGER::slotParseTx);
    connect(myClearBtn,&QPushButton::clicked,this,&RS232DATALOGGER::slotClear);
    connect(this,&RS232DATALOGGER::sigOpenRx,rs232Rx,&RS232::slotConnect);
    connect(this,&RS232DATALOGGER::sigOpenTx,rs232Tx,&RS232::slotConnect);
    connect(this,&RS232DATALOGGER::sigToFile,myFileWriter,&FileWriter::slotRx);
    emit sigOpenRx(false);
    emit sigOpenTx(false);
    //connect()
}

void RS232DATALOGGER::slotParseRx(QString msg)
{
    //qDebug()<<Q_FUNC_INFO;
    emit sigForwardRx(msg);
    //msg.prepend("Rx: ");

    //magle text so it becomes readable
    //protocol description: https://sigrok.org/wiki/UNI-T_UT71x_series#Protocol
    //http://www.produktinfo.conrad.com/datenblaetter/100000-124999/121791-da-01-en-Schnittst_Protokoll_VC960_DMM.pdf
    const int IDX_RANGE=5;
    const int IDX_FUNCTION=6;
    const int IDX_STATE_ACDC=7;
    const int IDX_STATE_AUTO=8;
    const char FUNC_VOLTAGE='1';
    const char FUNC_CURRENT='9';

    if (msg.at(0)==0xff8a)
    {
        msg.remove(0,1);
    }


    QString result;
    QChar range = msg.at(IDX_RANGE);
    QChar function = msg.at(IDX_FUNCTION);
    QChar state_acdc = msg.at(IDX_STATE_ACDC);
    QChar state_auto = msg.at(IDX_STATE_AUTO);

    QChar digit1 = msg.at(0);
    QChar digit2 = msg.at(1);
    QChar digit3 = msg.at(2);
    QChar digit4 = msg.at(3);
    QChar digit5 = msg.at(4);

    ushort digit;

    digit= digit1.unicode();
    digit &=0x007F;
    digit1 = digit;

    digit= digit2.unicode();
    digit &=0x007F;
    digit2 = digit;

    digit= digit3.unicode();
    digit &=0x007F;
    digit3 = digit;

    digit= digit4.unicode();
    digit &=0x007F;
    digit4 = digit;

    digit= digit5.unicode();
    digit &=0x007F;
    digit5 = digit;

    result=digit1;
    result.append(digit2);
    result.append('.');
    result.append(digit3);
    result.append(digit4);
    result.append(digit5);
    if (function==FUNC_VOLTAGE)
    {
        result.append('V');
    }
    if (function==FUNC_CURRENT)
    {
        result.append('A');
    }
    //myLog->appendPlainText(msg);
    myLog->appendPlainText(result);
    emit sigToFile(msg);
}

void RS232DATALOGGER::slotParseTx(QString msg)
{
    //qDebug()<<Q_FUNC_INFO;
    emit sigForwardTx(msg);
    msg.prepend("Tx: ");
    myLog->appendPlainText(msg);
    emit sigToFile(msg);
}

void RS232DATALOGGER::slotClear(bool clicked)
{
    Q_UNUSED(clicked);
    myLog->clear();
}
