#include "timebase.h"
#include <QDebug>

TIMEBASE::TIMEBASE(QWidget *parent) : QWidget(parent)
{
    centralWidget = new QWidget();
    completeLayout = new QVBoxLayout;
    setLayout(completeLayout);
    myTimer = new QTimer;
    myInterval = INTERVAL_1_SEC;
    connect(myTimer,&QTimer::timeout,this,&TIMEBASE::slotSecond);

    myTimer->start(myInterval);
}

void TIMEBASE::slotSecond()
{
    //compensate for imperfect time
    qint64 now = myQDateTime->currentDateTime().toMSecsSinceEpoch();
    qint64 next = (now + myInterval);
    qint64 interval = myInterval - (next % myInterval);
    if (interval < (myInterval / 2))
    {
        interval += myInterval;
    }
    myTimer->setInterval(interval);
    //end compensate
    emit sig1Hz();

    qDebug()<<now << "\t" <<(now-myLastNow);

    myLastNow = now;
}
