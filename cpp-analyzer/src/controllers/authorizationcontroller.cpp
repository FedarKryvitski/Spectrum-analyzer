#include "authorizationcontroller.h"
#include "ui/windows/authorizationform.h"

AuthorizationController::AuthorizationController(AuthorizationForm *form, QObject *parent)
    : QObject(parent)
    , authForm_(form)
{
    if (authForm_) {
        connect(authForm_, &AuthorizationForm::authorizationSubmitted, this, &AuthorizationController::onAuthorizationSubmitted);
    }
}

AuthorizationController::~AuthorizationController() = default;

void AuthorizationController::onAuthorizationSubmitted(const QString &email, const QString &password)
{
    if (authorizeUser(email, password)) {
        authForm_->clearErrorMessage();
        emit authorizationSuccess();
    }
}

bool AuthorizationController::authorizeUser(const QString &email, const QString &password)
{
    QString errorMessage;

    // Валидация почты
    if (!validator_.validateEmail(email, errorMessage)) {
        authForm_->setErrorMessage(errorMessage);
        emit authorizationFailed(errorMessage);
        return false;
    }

    // Валидация пароля
    if (!validator_.validatePassword(password, errorMessage)) {
        authForm_->setErrorMessage(errorMessage);
        emit authorizationFailed(errorMessage);
        return false;
    }

    // TODO: Проверка учетных данных на сервере
    // Заглушка - всегда возвращаем true если валидация прошла
    return true;
}