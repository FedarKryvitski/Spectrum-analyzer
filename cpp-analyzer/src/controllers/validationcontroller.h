#ifndef VALIDATIONCONTROLLER_H
#define VALIDATIONCONTROLLER_H

#include <QString>

class ValidationController
{
public:
    ValidationController() = default;

    bool validateEmail(const QString &email, QString &errorMessage) const;
    bool validatePassword(const QString &password, QString &errorMessage) const;
    bool validatePasswordMatch(const QString &password, const QString &passwordConfirm, QString &errorMessage) const;
    bool validateUsername(const QString &username, QString &errorMessage) const;

private:
    static constexpr int MIN_PASSWORD_LENGTH = 6;
    static constexpr int MAX_PASSWORD_LENGTH = 50;
    static constexpr int MIN_USERNAME_LENGTH = 2;
    static constexpr int MAX_USERNAME_LENGTH = 30;
};

#endif // VALIDATIONCONTROLLER_H