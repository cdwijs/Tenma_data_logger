#ifndef TENMA_H
#define TENMA_H

#include <QtWidgets>
#include <QObject>
#include "rs232.h"

class TENMA : public QWidget
{
    Q_OBJECT
public:
    TENMA(QWidget *parent=nullptr);
public slots:
    void slotRx(QString msg);
signals:
    void sigValue(QString msg);
private:
    RS232 *rs232;

    QWidget *centralWidget;
    QVBoxLayout *completeLayout;

    QLineEdit *valueline;

};

#endif // TENMA_H
