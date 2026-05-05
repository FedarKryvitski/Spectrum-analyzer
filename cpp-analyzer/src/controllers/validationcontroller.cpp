#include "validationcontroller.h"
#include <QRegularExpression>

bool ValidationController::validateEmail(const QString &email, QString &errorMessage) const
{
    if (email.isEmpty()) {
        errorMessage = QStringLiteral("Почта не может быть пустой");
        return false;
    }

    // Простая регулярное выражение для проверки почты
    static const QRegularExpression emailRegex(QStringLiteral(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"));
    if (!emailRegex.match(email).hasMatch()) {
        errorMessage = QStringLiteral("Введите корректный адрес почты");
        return false;
    }

    return true;
}

bool ValidationController::validatePassword(const QString &password, QString &errorMessage) const
{
    if (password.isEmpty()) {
        errorMessage = QStringLiteral("Пароль не может быть пустым");
        return false;
    }

    if (password.length() < MIN_PASSWORD_LENGTH) {
        errorMessage = QString::asprintf("Пароль должен содержать минимум %d символов", MIN_PASSWORD_LENGTH);
        return false;
    }

    if (password.length() > MAX_PASSWORD_LENGTH) {
        errorMessage = QString::asprintf("Пароль не должен превышать %d символов", MAX_PASSWORD_LENGTH);
        return false;
    }

    return true;
}

bool ValidationController::validatePasswordMatch(const QString &password, const QString &passwordConfirm, QString &errorMessage) const
{
    if (password != passwordConfirm) {
        errorMessage = QStringLiteral("Пароли не совпадают");
        return false;
    }

    return true;
}

bool ValidationController::validateUsername(const QString &username, QString &errorMessage) const
{
    if (username.isEmpty()) {
        errorMessage = QStringLiteral("Имя пользователя не может быть пустым");
        return false;
    }

    if (username.length() < MIN_USERNAME_LENGTH) {
        errorMessage = QString::asprintf("Имя должно содержать минимум %d символов", MIN_USERNAME_LENGTH);
        return false;
    }

    if (username.length() > MAX_USERNAME_LENGTH) {
        errorMessage = QString::asprintf("Имя не должно превышать %d символов", MAX_USERNAME_LENGTH);
        return false;
    }

    return true;
}
