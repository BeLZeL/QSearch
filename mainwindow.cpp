#include "mainwindow.h"
#include <QDir>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QMessageBox>
#include <QApplication>
#include <QThread>
#include <QUrl>
#include <QDesktopServices>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    l_dir    = new QLabel();
    le_to_find = new QLineEdit();
    te_left  = new QTextEdit();
    pb_scan  = new QPushButton("Scan !");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(l_dir);
    vbox->addWidget(le_to_find);
    vbox->addWidget(te_left);
    vbox->addWidget(pb_scan);

    setLayout(vbox);
    showMaximized();

    QObject::connect(pb_scan, SIGNAL(clicked()), this, SLOT(slot_scan()));

    //--------
/*
    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(250);
    */
}

void MainWindow::refresh()
{
    //te_left->append("<font color=green>TOTO</font>");
    static int count = 0;
    if ( count++ % 2500 == 0 )
        qApp->processEvents();
}


MainWindow::~MainWindow()
{

}

void MainWindow::slot_scan()
{
    int nb_found = 0;

    nb_found += scan("C:\\");
    nb_found += scan("E:\\");
    nb_found += scan("F:\\");
    nb_found += scan("G:\\");
    nb_found += scan("H:\\");
    nb_found += scan("Z:\\");
    qApp->processEvents();
}

// Return0 if OK
// Otherwise return nb_errors
int MainWindow::scan(const QString &str_dir)
{
    //te_left->append("<font color=green>" + str_dir + "</font>");
    if ( ! QFileInfo(str_dir).isDir() )
        return 0;
    //te_left->append("<font color=green>passhere</font>");

    QDir dir = QDir(str_dir);
    l_dir->setText(dir.absolutePath());
    //te_left->clear();
    //te_right->clear();

    QStringList files  = dir.entryList(QDir::AllDirs | QDir::Files | QDir::NoDot | QDir::NoDotDot);

    // CHECK LEFT

    int nb_found = 0;
    //te_left->append("<font color=green>" + QString::number(files.size()) + "</font>");
    for ( int i=0 ; i<files.size() ; ++i )
    {
        if ( files[i].contains(le_to_find->text(), Qt::CaseInsensitive) )
        {
            te_left->append("<font color=green>" + str_dir + "\\" + files[i] + "</font>");
            ++nb_found;
        }
        else
        {
            //te_left->append("<font color=red>" + files[i] + "</font>");
        }
    }

    // Scan subdirs if no errors
    if ( nb_found == 0 )
    {
        for ( int i=0 ; nb_found == 0 && i<files.size() ; ++i )
        {
            QString str = files[i];
            //qApp->processEvents();
            refresh();
            nb_found = scan(str_dir + "\\" + str);
        }
    }
    // Launch explorer if errors are found
    else
    {
        QString final = str_dir;
        final.replace("\\", "/");
        QDesktopServices::openUrl(QUrl("file:///"+final, QUrl::TolerantMode));
    }

    //qApp->processEvents();
    refresh();

    return nb_found;
}
