#include "mainwindow.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    le_pattern = new QLineEdit("Pattern");
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

    QObject::connect(pb_scan, SIGNAL(clicked()), this, SLOT(slot_scan()));
}

void MainWindow::refresh()
{
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
    if ( ! QFileInfo(str_dir).isDir() )
        return 0;

    QDir dir = QDir(str_dir);
    l_dir->setText(dir.absolutePath());

    QStringList files  = dir.entryList(QDir::AllDirs | QDir::Files | QDir::NoDot | QDir::NoDotDot);

    int nb_found = 0;
    for ( int i=0 ; i<files.size() ; ++i )
    {
        if ( files[i].contains(le_pattern->text(), Qt::CaseInsensitive) )
        {
            te_results->append("<font color=green>" + str_dir + "\\" + files[i] + "</font>");
            ++nb_found;
        }
    }

    // Scan subdirs
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
    // Launch explorer if found
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
