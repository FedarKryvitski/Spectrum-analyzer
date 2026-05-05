#ifndef AUTHORIZATIONCONTROLLER_H
#define AUTHORIZATIONCONTROLLER_H

#include <QObject>
#include "validationcontroller.h"

class AuthorizationForm;

class AuthorizationController : public QObject
{
    Q_OBJECT

public:
    explicit AuthorizationController(AuthorizationForm *form, QObject *parent = nullptr);
    ~AuthorizationController();

signals:
    void authorizationSuccess();
    void authorizationFailed(const QString &errorMessage);

private slots:
    void onAuthorizationSubmitted(const QString &email, const QString &password);

private:
    bool authorizeUser(const QString &email, const QString &password);

    AuthorizationForm *authForm_;
    ValidationController validator_;
};

#endif // AUTHORIZATIONCONTROLLER_H