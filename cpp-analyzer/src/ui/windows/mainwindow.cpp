#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

#include "analyzerform.h"
#include "authorizationform.h"
#include "registrationform.h"
#include "controllers/registrationcontroller.h"
#include "controllers/authorizationcontroller.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    analyzerForm_ = new AnalyzerForm(this);
    ui->stackedWidget->addWidget(analyzerForm_);

    authForm_ = new AuthorizationForm(this);
    ui->stackedWidget->addWidget(authForm_);

    registrationForm_ = new RegistrationForm(this);
    ui->stackedWidget->addWidget(registrationForm_);

    // Создаем контроллеры
    registrationController_ = std::make_unique<RegistrationController>(registrationForm_, this);
    authorizationController_ = std::make_unique<AuthorizationController>(authForm_, this);

    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(analyzerForm_, &AnalyzerForm::backRequested, this, &MainWindow::onBackFromAnalyzer);
    connect(authForm_, &AuthorizationForm::authorizationSubmitted, this, &MainWindow::onAuthorizationSubmitted);
    connect(authForm_, &AuthorizationForm::backRequested, this, &MainWindow::onBackFromAuthOrRegistration);
    connect(registrationForm_, &RegistrationForm::registrationSubmitted, this, &MainWindow::onRegistrationSubmitted);
    connect(registrationForm_, &RegistrationForm::backRequested, this, &MainWindow::onBackFromAuthOrRegistration);
    connect(registrationController_.get(), &RegistrationController::registrationSuccess, this, &MainWindow::onRegistrationSuccess);
    connect(authorizationController_.get(), &AuthorizationController::authorizationSuccess, this, &MainWindow::onAuthorizationSuccess);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::onAuthorizationSubmitted(const QString &email, const QString &password)
{
    Q_UNUSED(email)
    Q_UNUSED(password)

    // Контроллер обработает авторизацию
}

void MainWindow::onRegistrationSubmitted(const QString &email,
                                        const QString &password,
                                        const QString &passwordConfirm,
                                        const QString &username,
                                        bool premiumUser)
{
    Q_UNUSED(email)
    Q_UNUSED(password)
    Q_UNUSED(passwordConfirm)
    Q_UNUSED(username)
    Q_UNUSED(premiumUser)

    // Контроллер обработает регистрацию
}

void MainWindow::onRegistrationSuccess()
{
    QMessageBox::information(this, QStringLiteral("Успех"), QStringLiteral("Регистрация прошла успешно!"));
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onAuthorizationSuccess()
{
    ui->stackedWidget->setCurrentWidget(analyzerForm_);
}
