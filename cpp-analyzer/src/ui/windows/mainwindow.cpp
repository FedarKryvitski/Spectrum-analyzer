#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "analyzerform.h"
#include "authorizationform.h"
#include "registrationform.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    analyzerForm_ = new AnalyzerForm(this);
    ui->stackedWidget->addWidget(analyzerForm_);

    authForm_ = new AuthorizationForm(this);
    ui->stackedWidget->addWidget(authForm_);

    registrationForm_ = new RegistrationForm(this);
    ui->stackedWidget->addWidget(registrationForm_);

    connect(ui->startAnalyzerButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(analyzerForm_, &AnalyzerForm::backRequested, this, &MainWindow::onBackFromAnalyzer);
    connect(authForm_, &AuthorizationForm::authorizationSubmitted, this, &MainWindow::onAuthorizationSubmitted);
    connect(authForm_, &AuthorizationForm::backRequested, this, &MainWindow::onBackFromAuthOrRegistration);
    connect(registrationForm_, &RegistrationForm::registrationSubmitted, this, &MainWindow::onRegistrationSubmitted);
    connect(registrationForm_, &RegistrationForm::backRequested, this, &MainWindow::onBackFromAuthOrRegistration);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartClicked()
{
    ui->stackedWidget->setCurrentWidget(analyzerForm_);
}

void MainWindow::onLoginClicked()
{
    ui->stackedWidget->setCurrentWidget(authForm_);
}

void MainWindow::onRegisterClicked()
{
    ui->stackedWidget->setCurrentWidget(registrationForm_);
}

void MainWindow::onBackFromAnalyzer()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onBackFromAuthOrRegistration()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onAuthorizationSubmitted(const QString &login, const QString &password)
{
    Q_UNUSED(login)
    Q_UNUSED(password)

    // здесь можно обработать данные авторизации
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onRegistrationSubmitted(const QString &login,
                                        const QString &password,
                                        const QString &passwordConfirm,
                                        const QString &username,
                                        bool premiumUser)
{
    Q_UNUSED(login)
    Q_UNUSED(password)
    Q_UNUSED(passwordConfirm)
    Q_UNUSED(username)
    Q_UNUSED(premiumUser)

    // здесь можно обработать данные регистрации
    ui->stackedWidget->setCurrentIndex(0);
}
