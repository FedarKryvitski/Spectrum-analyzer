#pragma once

#include <QWidget>

namespace Ui
{
class RegistrationForm;
}

class RegistrationForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationForm(QWidget *parent = nullptr);
    ~RegistrationForm();

    bool isPremiumUser() const;
    void setErrorMessage(const QString &message);
    void clearErrorMessage();

signals:
    void registrationSubmitted(const QString &login,
                               const QString &password,
                               const QString &passwordConfirm,
                               const QString &username,
                               bool premiumUser);
    void backRequested();

private slots:
    void onPremiumButtonClicked();
    void onRegisterButtonClicked();
    void onBackButtonClicked();
    void onTogglePasswordVisibility();
    void onTogglePasswordConfirmVisibility();

private:
    Ui::RegistrationForm *ui;
    bool premiumUser_{false};
};
