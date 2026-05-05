#include "authorizationform.h"
#include "ui_authorizationform.h"

#include <QLineEdit>
#include <QPushButton>

AuthorizationForm::AuthorizationForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthorizationForm)
{
    ui->setupUi(this);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    connect(ui->loginButton, &QPushButton::clicked, this, &AuthorizationForm::onLoginButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &AuthorizationForm::onBackButtonClicked);
    connect(ui->togglePasswordButton, &QPushButton::clicked, this, &AuthorizationForm::onTogglePasswordVisibility);
}

AuthorizationForm::~AuthorizationForm()
{
    delete ui;
}

void AuthorizationForm::setErrorMessage(const QString &message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->setStyleSheet(QStringLiteral("color: #d32f2f; font-weight: bold;"));
}

void AuthorizationForm::clearErrorMessage()
{
    ui->errorLabel->setText(QString());
}

void AuthorizationForm::onLoginButtonClicked()
{
    clearErrorMessage();
    emit authorizationSubmitted(ui->loginLineEdit->text(), ui->passwordLineEdit->text());
}

void AuthorizationForm::onBackButtonClicked()
{
    clearErrorMessage();
    emit backRequested();
}

void AuthorizationForm::onTogglePasswordVisibility()
{
    if (ui->passwordLineEdit->echoMode() == QLineEdit::Password) {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->togglePasswordButton->setText(QStringLiteral("🙈"));
    } else {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        ui->togglePasswordButton->setText(QStringLiteral("👁"));
    }
}
