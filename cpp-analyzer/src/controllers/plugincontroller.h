#pragma once

#include <QObject>
#include <memory>
#include "pipeline/pipeline.h"

class PluginController : public QObject {
    Q_OBJECT
  public:
    explicit PluginController(QObject *parent = nullptr);

    Plugins::Pipeline* getPipeline() const { return pipeline_.get(); }

    void addPlugin(const QString& type);
    void removePlugin(int index);
    void movePlugin(int oldIndex, int newIndex);
    void togglePlugin(int index, bool enabled);

    QStringList getAvailablePlugins() const;

  signals:
    void pipelineChanged();

  private:
    std::unique_ptr<Plugins::Pipeline> pipeline_;
};
