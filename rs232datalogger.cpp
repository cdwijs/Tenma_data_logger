#include "rs232datalogger.h"
#include <QDebug>

const int MAX_LOG_BLOCKS=100000;


RS232DATALOGGER::RS232DATALOGGER(QWidget *parent) : QWidget(parent)
{
   //qDebug()<<Q_FUNC_INFO;

    myTenma1 = new TENMA(this);
    myTenma2 = new TENMA(this);
    myTimebase = new TIMEBASE(this);

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
    myVlayout = new QSplitter();
    myVlayout->setChildrenCollapsible(false);
    myVlayout->setOrientation(Qt::Vertical);
    myHlayout->addWidget(myClearBtn);
    myFileWriter = new FileWriter(myHlayout);

    myVlayout->addWidget(myTenma1);
    myVlayout->addWidget(myTenma2);

    myVlayout->addWidget(myLog);
    myVlayout->addWidget(myHlayout);
    myVlayout->show();

    myLog->setMaximumBlockCount(MAX_LOG_BLOCKS); //one block per hard line break
    myLog->setReadOnly(true);

    connect(myTenma1,&TENMA::sigValue,this,&RS232DATALOGGER::slotParseRx);
    connect(myTenma2,&TENMA::sigValue,this,&RS232DATALOGGER::slotParseRx);
    connect(myClearBtn,&QPushButton::clicked,this,&RS232DATALOGGER::slotClear);
    connect(this,&RS232DATALOGGER::sigToFile,myFileWriter,&FileWriter::slotRx);
    connect(myTimebase,&TIMEBASE::sig1Hz,this,&RS232DATALOGGER::slotSecond);
}

void RS232DATALOGGER::slotParseRx(QString msg)
{
    emit sigForwardTx(msg);
    myLog->appendPlainText(msg);
    emit sigToFile(msg);
}

void RS232DATALOGGER::slotClear(bool clicked)
{
    Q_UNUSED(clicked);
    myLog->clear();
}

void RS232DATALOGGER::slotSecond()
{

}
