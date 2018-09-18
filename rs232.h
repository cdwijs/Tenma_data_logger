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
    RS232(QWidget *parent=0);
public slots:
    slotSettings(bool clicked);
    slotConnect(bool clicked);
    slotDisconnect(bool clicked);
    slotMessage(QString string);
    slotRx();
    slotRxTimer();
    slotTxTimer();
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
    QSplitter *myHlayout;
    QLabel *myLabel;
    QPushButton *mySettingsBtn;
    QPushButton *myConnectBtn;
    SettingsDialog::Settings *mySerSettings;
    QSerialPort *mySerialPort;
    QQueue<unsigned char> *myRxQueue;
};

#endif // RS232_H
