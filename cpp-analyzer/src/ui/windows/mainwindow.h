#pragma once

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class AnalyzerForm;
class RegistrationForm;
class AuthorizationForm;
class RegistrationController;
class AuthorizationController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onBackFromAnalyzer();
    void onBackFromAuthOrRegistration();
    void onAuthorizationSubmitted(const QString &email, const QString &password);
    void onRegistrationSubmitted(const QString &email,
                                 const QString &password,
                                 const QString &passwordConfirm,
                                 const QString &username,
                                 bool premiumUser);
    void onRegistrationSuccess();
    void onAuthorizationSuccess();

  private:
    Ui::MainWindow *ui;

    AnalyzerForm *analyzerForm_{nullptr};
    AuthorizationForm *authForm_{nullptr};
    RegistrationForm *registrationForm_{nullptr};
    std::unique_ptr<RegistrationController> registrationController_;
    std::unique_ptr<AuthorizationController> authorizationController_;
};
