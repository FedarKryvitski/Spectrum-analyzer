#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class AnalyzerForm;
class PluginsListForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    void init();
    void connectPages();

  private:
    Ui::MainWindow *ui;

    AnalyzerForm *analyzerForm_;
    PluginsListForm *pluginsListForm_;
};
