#include "registrationform.h"
#include "ui_registrationform.h"

#include <QLineEdit>
#include <QPushButton>

RegistrationForm::RegistrationForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordConfirmLineEdit->setEchoMode(QLineEdit::Password);

    ui->premiumButton->setCheckable(true);
    ui->premiumButton->setText(QStringLiteral("Премиум пользователь"));

    connect(ui->premiumButton, &QPushButton::clicked, this, &RegistrationForm::onPremiumButtonClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &RegistrationForm::onRegisterButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &RegistrationForm::onBackButtonClicked);
    connect(ui->togglePasswordButton, &QPushButton::clicked, this, &RegistrationForm::onTogglePasswordVisibility);
    connect(ui->togglePasswordConfirmButton, &QPushButton::clicked, this, &RegistrationForm::onTogglePasswordConfirmVisibility);
}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

bool RegistrationForm::isPremiumUser() const
{
    return premiumUser_;
}

void RegistrationForm::setErrorMessage(const QString &message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->setStyleSheet(QStringLiteral("color: #d32f2f; font-weight: bold;"));
}

void RegistrationForm::clearErrorMessage()
{
    ui->errorLabel->setText(QString());
}

void RegistrationForm::onPremiumButtonClicked()
{
    premiumUser_ = !premiumUser_;
    ui->premiumButton->setChecked(premiumUser_);
    ui->premiumButton->setText(premiumUser_
        ? QStringLiteral("✓ Премиум включен")
        : QStringLiteral("Премиум пользователь"));
}

void RegistrationForm::onRegisterButtonClicked()
{
    clearErrorMessage();
    emit registrationSubmitted(ui->loginLineEdit->text(),
                               ui->passwordLineEdit->text(),
                               ui->passwordConfirmLineEdit->text(),
                               ui->usernameLineEdit->text(),
                               premiumUser_);
}

void RegistrationForm::onBackButtonClicked()
{
    clearErrorMessage();
    emit backRequested();
}

void RegistrationForm::onTogglePasswordVisibility()
{
    if (ui->passwordLineEdit->echoMode() == QLineEdit::Password) {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->togglePasswordButton->setText(QStringLiteral("🙈"));
    } else {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        ui->togglePasswordButton->setText(QStringLiteral("👁"));
    }
}

void RegistrationForm::onTogglePasswordConfirmVisibility()
{
    if (ui->passwordConfirmLineEdit->echoMode() == QLineEdit::Password) {
        ui->passwordConfirmLineEdit->setEchoMode(QLineEdit::Normal);
        ui->togglePasswordConfirmButton->setText(QStringLiteral("🙈"));
    } else {
        ui->passwordConfirmLineEdit->setEchoMode(QLineEdit::Password);
        ui->togglePasswordConfirmButton->setText(QStringLiteral("👁"));
    }
}
