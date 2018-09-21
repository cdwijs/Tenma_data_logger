#ifndef RS232_H
#define RS232_H

//#define FAKE_MESSAGES

#include <QTimer>
#include <QtWidgets>
#include <QWidget>
#include <QSerialPort>
#include <QString>
#include "settingsdialog.h"
#include "ledindicator.h"
#include <QObject>
#include <QQueue>



class RS232 : public QWidget
{
    Q_OBJECT
public:
    RS232(QWidget *parent=nullptr);
public slots:
    void slotSettings(bool clicked);
    void slotConnect(bool clicked);
    void slotDisconnect(bool clicked);
    void slotMessage(QString string);
    void slotRx(void);
    void slotRxTimer(void);
    void slotTxTimer(void);
#ifdef FAKE_MESSAGES
    slotTimer();
#endif
signals:
    void sigOpenChanged(bool open);
    void sigReceived(QString msg);
private:
#ifdef FAKE_MESSAGES
    QTimer *myTimer;
#endif
    QTimer *myTxTimer;
    QTimer *myRxTimer;
    LedIndicator *myRxLED;
    LedIndicator *myTxLED;
    SettingsDialog *mySettingsDia;
    QHBoxLayout *myHlayout;
    QLabel *myLabel;
    QPushButton *mySettingsBtn;
    QPushButton *myConnectBtn;
    SettingsDialog::Settings *mySerSettings;
    QSerialPort *mySerialPort;
    QQueue<char> *myRxQueue;
};

#endif // RS232_H
