#pragma once

#include "base/plugin.h"
#include "pipelinesink.h"
#include "pipelinesource.h"

#include <memory>
#include <vector>

namespace Plugins
{

class Pipeline
{
  public:
    Pipeline() noexcept;
    virtual ~Pipeline() = default;

    Buffer process(Buffer input);

    double getInputVolume() const;
    double getOutputVolume() const;

  private:
    std::vector<std::shared_ptr<IPlugin>> plugins_;

    PipelineSink sink_;
    PipelineSource source_;

    double inputVolume_{}, outputVolume_{};
};

} // namespace Plugins
