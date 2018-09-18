#include "filewriter.h"
#define TIME_FILE 100

#define KEY_DIRECTORY "Directory"
#define DEFAULT_DIRECTORY ""

FileWriter::FileWriter(QWidget *parent)
{
    //qDebug()<<Q_FUNC_INFO;

    myFile = new QFile();
    myHlayout = new QSplitter();

    myTimer = new QTimer();
    myFileBtn = new QPushButton();
    myFileBtn->setIcon(QIcon("://images/disk_edit.png"));
    myFileBtn->setToolTip("File");

    connect(myTimer,&QTimer::timeout,this,slotTimer);
    myHlayout->setOrientation(Qt::Horizontal);
    myHlayout->addWidget(myFileBtn);
    myHlayout->setParent(parent);
    myHlayout->show();
    connect(myFileBtn,&QPushButton::clicked,this,&FileWriter::slotFile);
}

FileWriter::slotRx(QString msg)
{
    //store the msg in the file here



    if (myFile->isOpen())
    {
        myTimer->start(TIME_FILE);
        myFileBtn->setIcon(QIcon("://images/disk_in.png"));
        myFile->write(msg.toLocal8Bit());
    }
    else
    {
        myTimer->stop();
        //myFileBtn->setIcon(QIcon("://images/disk_edit.png"));
    }
}

FileWriter::slotFile(bool clicked)
{
    //close the file when clicked again
    if (myFile->isOpen())
    {
        myFile->close();
        myTimer->stop();
        myFileBtn->setIcon(QIcon("://images/disk_edit.png"));
    }
    else
    {
        QString basedir;
        QSettings * settings = new QSettings;
        if (settings->contains(KEY_DIRECTORY))
        {
            basedir=settings->value(KEY_DIRECTORY,DEFAULT_DIRECTORY).toString();
        }
        else
        {
            basedir="";
        }

        QString filename;
        filename = QFileDialog::getSaveFileName(Q_NULLPTR,tr("Save File"),basedir,"Txt Files(*.txt);;All files(*.*)",Q_NULLPTR,QFileDialog::DontUseNativeDialog); //nativedialog crashes after 2 minutes

        basedir = filename.section("/",0,-2);
        if (basedir.isEmpty())
        {

        }
        else
        {
            settings->setValue(KEY_DIRECTORY,basedir);
            //filename is known here
            myFile->close();
            myFile->setFileName(filename);
            if (myFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
            {
                myFileBtn->setIcon(QIcon("://images/disk.png"));
            }
            else
            {

            }

        }
        delete settings; //so the destructor of Qsettings calls sync, and the settings are flushed to disk
    }
}


FileWriter::slotTimer()
{
    myFileBtn->setIcon(QIcon("://images/disk.png"));
}
