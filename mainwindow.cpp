#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QJsonObject>

using namespace DiagonalUI;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , logParser("diagonal")
    , tests()
{
    ui->setupUi(this);

    logParser.addHandler<QString>("logLine", [&] (QString line) {
        ui->outputLines->addItem(line);
    });

    logParser.addHandler<QString>("enumerateTest", [&] (QString testName) {
        auto display = new DiagonalUI::TestDisplay();
        display->setData(DiagonalUI::Test(testName));
        display->setParent(ui->testList);
        tests.append(display);
    });

    logParser.addHandler<QJsonValue>("clearTests", [&](auto){
        foreach(auto test, tests)
            delete test;
        tests.clear();
    });

    logParser.addHandler<QString>("testStart", [&] (QString testName) {
        auto needle = new TestDisplay(testName);
        auto display = tests.at(tests.indexOf(needle));
        delete needle;
        auto test = display->data();
        test.start();
        display->setData(test);
    });

    logParser.addHandler<QJsonObject>("statusReport", [&] (QJsonObject data) {
        auto needle = new TestDisplay(data["name"].toString());
        auto display = tests.at(tests.indexOf(needle));
        delete needle;
        auto test = display->data();
        test.statusReport(data["success"].toBool());
        display->setData(test);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logcatButtonClicked() {
    logParser.processMessage("/* log:tag:diagonal */ {\"message\": \"logLine\", \"data\": \"Hello World\"}");
}
