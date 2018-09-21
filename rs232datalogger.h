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
#include "tenma.h"

class RS232DATALOGGER : public QWidget
{
    Q_OBJECT
public:

    RS232DATALOGGER(QWidget *parent=nullptr);
public slots:
    void slotParseRx(QString msg);
    void slotParseTx(QString msg);
    void slotClear(bool clicked);

signals:
    void sigForwardRx(QString msg);
    void sigForwardTx(QString msg);
    void sigOpenTx(bool clicked);
    void sigOpenRx(bool clicked);
    void sigToFile(QString msg);
private:
    TENMA *myTenma1;
    TENMA *myTenma2;

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
