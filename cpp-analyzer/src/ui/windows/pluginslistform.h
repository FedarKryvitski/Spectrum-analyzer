#pragma once

#include <QWidget>

namespace Ui
{
class PluginsListForm;
}

class PluginsListForm : public QWidget
{
    Q_OBJECT

  public:
    explicit PluginsListForm(QWidget *parent = nullptr);
    ~PluginsListForm();

  signals:
    void backRequested();

  private slots:
    void onAddItemSlot();

  private:
    void init();
    void addNewPlugin(const QString &pluginName);
    void showPluginWidget(QWidget* widget);

  private:
    Ui::PluginsListForm *ui;
};
