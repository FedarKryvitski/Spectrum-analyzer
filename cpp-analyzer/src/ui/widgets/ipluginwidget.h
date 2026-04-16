#pragma once

#include <QWidget>

namespace Plugins
{
class IPlugin;
} // namespace Plugins

class IPluginWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit IPluginWidget(Plugins::IPlugin *plugin, QWidget *parent = nullptr) : QWidget(parent), plugin_(plugin)
    {
    }

    virtual ~IPluginWidget() = default;

    virtual void updateFromPlugin() = 0;
    virtual void applyToPlugin() = 0;

  protected:
    Plugins::IPlugin *plugin_;
};
