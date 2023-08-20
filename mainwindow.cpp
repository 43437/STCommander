
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mainwidget.h"
#include "qmlinwidget.h"
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    auto pWidget = new MainWidget;
    auto pTabWidget = new QTabWidget;
    this->setCentralWidget(pTabWidget);
    this->setMinimumWidth(900);

    pTabWidget->addTab(pWidget, "basic");
    pTabWidget->addTab(new QmlInWidget, "dynamic");
}


