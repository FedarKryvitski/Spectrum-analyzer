#include "pluginwidgetfactory.h"

#include "base/plugin.h"
#include "gainerwidget.h"
#include "limiterwidget.h"

namespace Plugins
{

IPluginWidget *createPluginWidget(IPlugin *plugin, QWidget *parent)
{
    const auto type = plugin->getName();

    if (type == "Gainer")
        return new GainerWidget(plugin, parent);
    else if (type == "Limiter")
        return new LimiterWidget(plugin, parent);

    return nullptr;
}

} // namespace Plugins
