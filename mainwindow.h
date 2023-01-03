#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logparser.h"
#include "testdisplay.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void logcatButtonClicked();

private:
    Ui::MainWindow *ui;
    DiagonalUI::LogParser logParser;
    QList<DiagonalUI::TestDisplay*> tests;
};
#endif // MAINWINDOW_H
