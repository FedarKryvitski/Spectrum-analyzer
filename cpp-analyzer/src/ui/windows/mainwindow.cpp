#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "analyzerform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    analyzerForm_ = new AnalyzerForm(this);
    ui->stackedWidget->addWidget(analyzerForm_);

    connect(ui->startAnalyzerButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(analyzerForm_, &AnalyzerForm::backRequested, this, &MainWindow::onBackFromAnalyzer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartClicked()
{
    ui->stackedWidget->setCurrentWidget(analyzerForm_);
}

void MainWindow::onBackFromAnalyzer()
{
    ui->stackedWidget->setCurrentIndex(0);
}
