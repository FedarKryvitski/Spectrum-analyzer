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

    double getVolume() const;

  private:
    std::vector<std::shared_ptr<IPlugin>> plugins_;

    PipelineSink sink_;
    PipelineSource source_;

    double volume_{0};
};

} // namespace Plugins
