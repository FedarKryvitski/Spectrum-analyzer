#pragma once

#include <QWidget>

namespace Ui
{
class PluginsListForm;
}

class PluginController;

class PluginsListForm : public QWidget
{
    Q_OBJECT

  public:
    explicit PluginsListForm(QWidget *parent = nullptr);
    ~PluginsListForm();

    void setController(PluginController* controller);

  signals:
    void backRequested();

  private slots:
    void onAddItemSlot();
    void updateListView();

  private:
    void init();
    void showPluginWidget(QWidget* widget);

  private:
    Ui::PluginsListForm *ui;
    PluginController* controller_{nullptr};
};
