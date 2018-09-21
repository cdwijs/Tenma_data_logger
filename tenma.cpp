#include "tenma.h"

TENMA::TENMA(QWidget *parent) : QWidget(parent)
{
    rs232 = new RS232(this);
    valueline = new QLineEdit();

    centralWidget = new QWidget();
    completeLayout = new QVBoxLayout;

    completeLayout->addWidget(valueline);
    completeLayout->addWidget(rs232);

    setLayout(completeLayout);

    valueline->setEnabled(false);

    connect(rs232,&RS232::sigReceived,this,&TENMA::slotRx);
}

void TENMA::slotRx(QString msg)
{
    //qDebug()<<Q_FUNC_INFO;
    //magle text so it becomes readable
    //protocol description: https://sigrok.org/wiki/UNI-T_UT71x_series#Protocol
    //http://www.produktinfo.conrad.com/datenblaetter/100000-124999/121791-da-01-en-Schnittst_Protokoll_VC960_DMM.pdf
    const int IDX_RANGE=5;
    const int IDX_FUNCTION=6;
    const int IDX_STATE_ACDC=7;
    const int IDX_STATE_AUTO=8;
    const char FUNC_VOLTAGE='1';
    const char FUNC_CURRENT='9';

    const char STATE_OFF='0';
    const char STATE_AC='1';
    const char STATE_DC='2';
    const char STATE_ACDC='3';

    if (msg.at(0)==0xff8a)
    {
        msg.remove(0,1);
    }


    QString result;
    QChar range = msg.at(IDX_RANGE);
    QChar function = msg.at(IDX_FUNCTION);
    QChar state_acdc = msg.at(IDX_STATE_ACDC);
    QChar state_auto = msg.at(IDX_STATE_AUTO);

    ushort msgchar;
    msgchar= function.unicode();
    msgchar &=0x007F;
    function = msgchar;

    msgchar= range.unicode();
    msgchar &=0x007F;
    range = msgchar;

    msgchar= state_acdc.unicode();
    msgchar &=0x007F;
    state_acdc = msgchar;

    msgchar= state_auto.unicode();
    msgchar &=0x007F;
    state_auto = msgchar;


    if (state_acdc==STATE_OFF)
    {
        result.append("OFF");
    }
    if (state_acdc==STATE_AC)
    {
        result.append("AC");
    }
    if (state_acdc==STATE_DC)
    {
        result.append("DC");
    }
    if (state_acdc==STATE_ACDC)
    {
        result.append("ACDC");
    }

    result.append('\t');

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

    result.append(digit1);
    result.append(digit2);
    result.append('.');
    result.append(digit3);
    result.append(digit4);
    result.append(digit5);

    result.append('\t');

    if (function==FUNC_VOLTAGE)
    {
        result.append('V');
    }
    if (function==FUNC_CURRENT)
    {
        result.append('A');
    }
    valueline->setText(result);
    emit sigValue(result);
}
