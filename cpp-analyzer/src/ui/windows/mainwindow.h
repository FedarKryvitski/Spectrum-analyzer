#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class AnalyzerForm;
class RegistrationForm;
class AuthorizationForm;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void onStartClicked();
    void onLoginClicked();
    void onRegisterClicked();
    void onBackFromAnalyzer();
    void onBackFromAuthOrRegistration();
    void onAuthorizationSubmitted(const QString &login, const QString &password);
    void onRegistrationSubmitted(const QString &login,
                                 const QString &password,
                                 const QString &passwordConfirm,
                                 const QString &username,
                                 bool premiumUser);

  private:
    Ui::MainWindow *ui;

    AnalyzerForm *analyzerForm_{nullptr};
    AuthorizationForm *authForm_{nullptr};
    RegistrationForm *registrationForm_{nullptr};
};
