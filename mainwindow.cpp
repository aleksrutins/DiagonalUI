#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QJsonObject>
#include <QtConcurrent/QtConcurrent>
#include <iostream>

using namespace DiagonalUI;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , logParser("diagonal")
    , tests()
    , proc(new QProcess(this))
{
    ui->setupUi(this);

    ui->testList->setAlignment(Qt::AlignTop);

    logParser.addHandler<QString>("logLine", [&] (QString line) {
        ui->outputLines->addItem(line);
    });

    logParser.addHandler<QString>("enumerateTest", [&] (QString testName) {
        auto display = new DiagonalUI::TestDisplay();
        display->setData(DiagonalUI::Test(testName));
        ui->testList->addWidget(display);
        display->setVisible(true);
        tests.append(display);
    });

    logParser.addHandler<QJsonValue>("clearTests", [&](auto){
        foreach(auto test, tests)
            delete test;
        tests.clear();
        ui->outputLines->clear();
    });

    logParser.addHandler<QString>("testStart", [&] (QString testName) {

        foreach (auto test, tests) {
            auto data = test->data();
            if(data.name() == testName) {
                 ui->outputLines->addItem("*** TEST START: " + testName + " ***");
                data.start();
                test->setData(data);
                test->updateDisplay(data);
                break;
            }
        }
    });

    logParser.addHandler<QJsonObject>("statusReport", [&] (QJsonObject data) {
        ui->outputLines->addItem("*** TEST FINISHED: " + data["name"].toString() + " | STATUS: " + (data["success"].toBool()? "Success" : "Failure") + " ***");
        foreach (auto test, tests) {
            auto testData = test->data();
            if(testData.name() == data["name"].toString()) {
                testData.statusReport(data["success"].toBool());
                test->setData(testData);
                test->updateDisplay(testData);
                break;
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logcatButtonClicked() {
    ui->logcatButton->setDisabled(true);
    ui->adbPath->setDisabled(true);
    proc->setEnvironment( QProcess::systemEnvironment() );
    proc->setProcessChannelMode( QProcess::MergedChannels );
    proc->setReadChannel(QProcess::StandardOutput);
    proc->start(ui->adbPath->text(), {"logcat"});
    connect(proc.data(), SIGNAL(readyReadStandardOutput()), this, SLOT(processOutputReady()));
    connect(proc.data(), SIGNAL(readyReadStandardError()), this, SLOT(processErrorReady()));
}

void MainWindow::processOutputReady()
{
    auto output = proc->readAllStandardOutput();
    foreach(auto line, output.split('\n')) {
        //cout << line.data() << endl;
        logParser.processMessage(line);
    }
}


void MainWindow::processErrorReady()
{
    auto output = proc->readAllStandardError();
    foreach(auto line, output.split('\n')) {
        //cout << line.data() << endl;
        logParser.processMessage(line);
    }
}

