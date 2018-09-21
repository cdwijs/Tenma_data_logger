
#include <QApplication>
#include <QDebug>
#include <QSettings>
#include "rs232datalogger.h"

int main(int argc, char *argv[])
{
    QString git=GIT_COMMIT;
    QString git_url=GIT_URL;
    qDebug() << "Git:" << qPrintable(git);
    qDebug() << "Repository:" << qPrintable(git_url);
    qDebug() << "Build:" << __DATE__ << __TIME__;
    QApplication::setOrganizationName("LUMC");
    QApplication::setOrganizationDomain("lumc.nl");
    QApplication::setApplicationName("Temna Datalogger");
    QSettings * settings = new QSettings; //dummy instance to ensure ini files are read by every next read/write action
    settings->setDefaultFormat(QSettings::IniFormat);
    //qDebug()<<"Reading settings from:"<< settings->fileName();
    QSettings * settings2 = new QSettings;
    Q_UNUSED(settings2);
    //qDebug()<<"Reading settings from:"<< settings2->fileName();
    QApplication a(argc, argv);

    RS232DATALOGGER rs232Monitor;

    return a.exec();
}
