#pragma once

#include <QWidget>

namespace Ui
{
class PluginsListForm;
}

namespace Plugins
{
class IPlugin;
} // namespace Plugins

class PluginItemWidget;
class PluginController;

class PluginsListForm : public QWidget
{
    Q_OBJECT

  public:
    explicit PluginsListForm(QWidget *parent = nullptr);
    ~PluginsListForm();

    void setController(PluginController *controller);

  signals:
    void backRequested();

  private slots:
    void onAddItemSlot();
    void updateListView();

  private:
    void init();
    void clearLayout();
    void openPluginDialog(Plugins::IPlugin *plugin);
    PluginItemWidget *createItem(Plugins::IPlugin *plugin, const int index);

  private:
    Ui::PluginsListForm *ui;
    PluginController *controller_{nullptr};
};
