#include "registrationcontroller.h"
#include "ui/windows/registrationform.h"

RegistrationController::RegistrationController(RegistrationForm *form, QObject *parent)
    : QObject(parent)
    , registrationForm_(form)
{
    if (registrationForm_) {
        connect(registrationForm_, &RegistrationForm::registrationSubmitted, this, &RegistrationController::onRegistrationSubmitted);
    }
}

RegistrationController::~RegistrationController() = default;

void RegistrationController::onRegistrationSubmitted(const QString &email,
                                                     const QString &password,
                                                     const QString &passwordConfirm,
                                                     const QString &username,
                                                     bool premiumUser)
{
    if (registerUser(email, password, passwordConfirm, username, premiumUser)) {
        registrationForm_->clearErrorMessage();
        emit registrationSuccess();
    }
}

bool RegistrationController::registerUser(const QString &email,
                                         const QString &password,
                                         const QString &passwordConfirm,
                                         const QString &username,
                                         bool premiumUser)
{
    QString errorMessage;

    // Валидация почты
    if (!validator_.validateEmail(email, errorMessage)) {
        registrationForm_->setErrorMessage(errorMessage);
        emit registrationFailed(errorMessage);
        return false;
    }

    // Валидация пароля
    if (!validator_.validatePassword(password, errorMessage)) {
        registrationForm_->setErrorMessage(errorMessage);
        emit registrationFailed(errorMessage);
        return false;
    }

    // Проверка совпадения паролей
    if (!validator_.validatePasswordMatch(password, passwordConfirm, errorMessage)) {
        registrationForm_->setErrorMessage(errorMessage);
        emit registrationFailed(errorMessage);
        return false;
    }

    // Валидация имени пользователя
    if (!validator_.validateUsername(username, errorMessage)) {
        registrationForm_->setErrorMessage(errorMessage);
        emit registrationFailed(errorMessage);
        return false;
    }

    // TODO: Отправка на сервер
    // Заглушка - всегда возвращаем true если валидация прошла
    Q_UNUSED(premiumUser)
    return true;
}