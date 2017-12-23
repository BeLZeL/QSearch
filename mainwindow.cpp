#include "mainwindow.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    le_pattern = new QLineEdit("");
    pb_scan    = new QPushButton("Scan !");
    l_dir      = new QLabel();
    te_results = new QTextEdit();

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(le_pattern);
    vbox->addWidget(pb_scan);
    vbox->addWidget(l_dir);
    vbox->addWidget(te_results);

    setLayout(vbox);
    showMaximized();

    from = QDateTime::currentDateTime().addSecs(-5*24*60*60);

    qDebug() << from;

    QObject::connect(pb_scan, SIGNAL(clicked()), this, SLOT(slot_scan()));
}

void MainWindow::refresh()
{
    static int count = 0;
    if ( count++ % 1000 == 0 )
        qApp->processEvents();
}

MainWindow::~MainWindow()
{

}

void MainWindow::slot_scan()
{
    te_results->clear();

    int nb_found = 0;
    //nb_found += scan("C:\");
    nb_found += scan("E:\\");
    nb_found += scan("F:\\");
    nb_found += scan("G:\\");
    //nb_found += scan("H:\");
    //nb_found += scan("Z:\");
    qApp->processEvents();
}

// Return0 if OK
// Otherwise return nb_errors
int MainWindow::scan(const QString &str_dir)
{
    // File, returns
    if ( ! QFileInfo(str_dir).isDir() )
        return 0;

    QDir dir = QDir(str_dir);
    l_dir->setText(dir.absolutePath());
    int nb_found = 0;
    refresh();

    if ( !le_pattern->text().isEmpty() )
    {
        QStringList files  = dir.entryList(QDir::AllDirs | QDir::Files | QDir::NoDot | QDir::NoDotDot);

        for ( int i=0 ; i<files.size() ; ++i )
        {
            if ( files[i].contains(le_pattern->text(), Qt::CaseInsensitive) )
            {
                te_results->append("<font color=green>" + str_dir + files[i] + "</font>");
                ++nb_found;
                refresh();
            }
        }

        // Launch explorer if found
        /*
        if ( nb_found > 0 )
        {
            QString final = str_dir;
            final.replace("\\", "/");
            QDesktopServices::openUrl(QUrl("file:///"+final, QUrl::TolerantMode));
        }
        */

        // Scan subdirs
        for ( int i=0 ; i<files.size() ; ++i )
        {
            QString str = files[i];
            nb_found = scan(str_dir + str + "\\");
        }
    }
    else
    {
        QList<QFileInfo> files = dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoDot | QDir::NoDotDot);

        int nb_found = 0;
        for ( int i=0 ; i<files.size() ; ++i )
        {
            if ( files[i].isDir() )
            {
                nb_found = scan(str_dir + files[i].fileName() + "\\");
            }
            else
            {
                //qDebug() << files[i].fileName();
                if ( files[i].created() >= from || files[i].lastModified() >= from )
                {
                    te_results->append("<font color=green>" + str_dir + files[i].fileName() + "</font>");
                    refresh();
                    ++nb_found;
                    qDebug() << files[i].lastModified();
                }
            }

            // Launch explorer if found
            /**/
            if ( nb_found > 0 )
            {
                QString final = str_dir;
                final.replace("\\", "/");
                QDesktopServices::openUrl(QUrl("file:///"+final, QUrl::TolerantMode));
            }
            /**/
        }
    }

    return nb_found;
}
