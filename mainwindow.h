#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QDateTime>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slot_scan();
    void refresh();

private:
    int scan(const QString &dir);

    int m_nb_dirs;
    QLabel *l_dir;
    QLineEdit *le_pattern;
    QTextEdit *te_results;
    QPushButton *pb_scan;
    QDateTime from;
};

#endif // MAINWINDOW_H
