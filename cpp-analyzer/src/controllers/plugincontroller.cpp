#include "plugincontroller.h"

#include "effects/gainer.h"
#include "effects/limiter.h"

PluginController::PluginController(QObject *parent)
    : QObject(parent)
      , pipeline_(std::make_unique<Plugins::Pipeline>())
{
}

void PluginController::addPlugin(const QString& type)
{
    std::shared_ptr<Plugins::IPlugin> plugin = nullptr;

    if (type == "Gain") {
        plugin = std::make_shared<Plugins::Gainer>();
    }
    else if (type == "Limiter") {
        plugin = std::make_shared<Plugins::Limiter>();
    }

    if (plugin) {
        pipeline_->addPlugin(std::move(plugin));
        emit pipelineChanged();
    }
}

void PluginController::removePlugin(int index)
{
    pipeline_->removePlugin(index);
    emit pipelineChanged();
}

void PluginController::movePlugin(int oldIndex, int newIndex)
{
    pipeline_->movePlugin(oldIndex, newIndex);
    emit pipelineChanged();
}

void PluginController::togglePlugin(int index, bool enabled)
{
    pipeline_->togglePlugin(index, enabled);
    emit pipelineChanged();
}

QStringList PluginController::getAvailablePlugins() const
{
    return { "Gain", "Limiter" };
}
