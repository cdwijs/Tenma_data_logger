#ifndef RS232MONITOR_H
#define RS232MONITOR_H
#include <QTimer>
#include <QtWidgets>
#include <QWidget>
#include "rs232.h"
#include <QObject>
#include <QPlainTextEdit>
#include <QTimer>
#include "filewriter.h"

class RS232MONITOR : public QWidget
{
    Q_OBJECT
public:

    RS232MONITOR(QWidget *parent=0);
public slots:
    slotParseRx(QString msg);
    slotParseTx(QString msg);
    slotClear(bool clicked);


signals:
    sigForwardRx(QString msg);
    sigForwardTx(QString msg);
    sigOpenTx(bool clicked);
    sigOpenRx(bool clicked);
    sigToFile(QString msg);
private:
    RS232 *rs232Rx;
    RS232 *rs232Tx;
    QPlainTextEdit *myLog;
    QSplitter *myHlayout;
    QSplitter *myVlayout;
    QPushButton *myClearBtn;
    QLabel *myRxLabel;
    QLabel *myTxLabel;
    FileWriter *myFileWriter;
};

#endif // RS232MONITOR_H
