#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "analyzerform.h"
#include "pluginslistform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
      , analyzerForm_(nullptr)
      , pluginsListForm_(nullptr)
{
    ui->setupUi(this);

    analyzerForm_ = new AnalyzerForm(this);
    pluginsListForm_ = new PluginsListForm(this);

    ui->stackedWidget->addWidget(analyzerForm_);
    ui->stackedWidget->addWidget(pluginsListForm_);

    init();
    connectPages();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->stackedWidget->setCurrentWidget(analyzerForm_);
}

void MainWindow::connectPages()
{
    connect(analyzerForm_, &AnalyzerForm::openListRequested,
            this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(pluginsListForm_);
            });

    connect(pluginsListForm_, &PluginsListForm::backRequested,
            this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(analyzerForm_);
            });
}
