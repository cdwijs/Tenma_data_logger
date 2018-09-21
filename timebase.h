#ifndef TIMEBASE_H
#define TIMEBASE_H
#include <QtWidgets>
#include <QTimer>
#include <QDateTime>

const int INTERVAL_1_SEC=1000;

class TIMEBASE : public QWidget
{
    Q_OBJECT
public:
    TIMEBASE(QWidget *parent=nullptr);
public slots:
    void slotSecond (void);
signals:
    void sig1Hz(void);
private:
    QWidget *centralWidget;
    QVBoxLayout *completeLayout;

    QTimer *myTimer;
    QDateTime *myQDateTime;
    int myInterval;
    qint64 myLastNow;
};

#endif // TIMEBASE_H
