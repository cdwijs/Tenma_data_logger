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
    msg.prepend("Rx: ");
    myLog->appendPlainText(msg);
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
