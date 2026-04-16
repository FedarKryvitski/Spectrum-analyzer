#pragma once

#include "ipluginwidget.h"

namespace Plugins
{

IPluginWidget *createPluginWidget(IPlugin *plugin, QWidget *parent = nullptr);

} // namespace Plugins
