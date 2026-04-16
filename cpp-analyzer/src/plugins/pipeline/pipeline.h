#pragma once

#include "base/plugin.h"
#include "pipelinesink.h"
#include "pipelinesource.h"

#include <memory>
#include <mutex>
#include <vector>

namespace Plugins
{

class Pipeline
{
  public:
    Pipeline() noexcept;
    virtual ~Pipeline() = default;

    Buffer process(Buffer input);

    void addPlugin(std::shared_ptr<IPlugin> plugin);
    void removePlugin(int index);
    void movePlugin(int oldIndex, int newIndex);
    void togglePlugin(int index, bool enabled);
    void clear();

    std::vector<std::shared_ptr<IPlugin>> getPlugins() const;

    double getInputVolume() const;
    double getOutputVolume() const;

  private:
    mutable std::mutex mutex_;
    std::vector<std::shared_ptr<IPlugin>> plugins_;

    PipelineSink sink_;
    PipelineSource source_;

    double inputVolume_{}, outputVolume_{};
};

} // namespace Plugins
