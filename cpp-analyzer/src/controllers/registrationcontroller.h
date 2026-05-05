#ifndef REGISTRATIONCONTROLLER_H
#define REGISTRATIONCONTROLLER_H

#include <QObject>
#include "validationcontroller.h"

class RegistrationForm;

class RegistrationController : public QObject
{
    Q_OBJECT

public:
    explicit RegistrationController(RegistrationForm *form, QObject *parent = nullptr);
    ~RegistrationController();

signals:
    void registrationSuccess();
    void registrationFailed(const QString &errorMessage);

private slots:
    void onRegistrationSubmitted(const QString &login,
                                 const QString &password,
                                 const QString &passwordConfirm,
                                 const QString &username,
                                 bool premiumUser);

private:
    bool registerUser(const QString &login,
                      const QString &password,
                      const QString &passwordConfirm,
                      const QString &username,
                      bool premiumUser);

    RegistrationForm *registrationForm_;
    ValidationController validator_;
};

#endif // REGISTRATIONCONTROLLER_H