#pragma once

#include <QWidget>

namespace Ui
{
class AuthorizationForm;
}

class AuthorizationForm : public QWidget
{
    Q_OBJECT

public:
    explicit AuthorizationForm(QWidget *parent = nullptr);
    ~AuthorizationForm();

    void setErrorMessage(const QString &message);
    void clearErrorMessage();

signals:
    void authorizationSubmitted(const QString &login, const QString &password);
    void backRequested();

private slots:
    void onLoginButtonClicked();
    void onBackButtonClicked();
    void onTogglePasswordVisibility();

private:
    Ui::AuthorizationForm *ui;
};
