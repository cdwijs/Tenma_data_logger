#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>

#include <QTimer>
#include <QtWidgets>
#include <QWidget>
#include <QTimer>
#include <QFile>
#include <QFileDialog>

class FileWriter : public QWidget
{
public:
    FileWriter(QWidget *parent=nullptr);
public slots:
    void slotRx(QString msg);
    void slotFile(bool clicked);
    void slotTimer();
signals:
private:
    QPushButton *myFileBtn;
    QTimer *myTimer;
    QSplitter *myHlayout;
    QFile *myFile;
};

#endif // FILEWRITER_H
