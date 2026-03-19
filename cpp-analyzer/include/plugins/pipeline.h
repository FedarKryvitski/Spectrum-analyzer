#ifndef PIPELINE_H
#define PIPELINE_H

#include "plugin.h"
#include "pipelinesink.h"
#include "pipelinesource.h"

#include <vector>
#include <memory>

namespace Plugins {

class Pipeline {
  public:
    Pipeline() noexcept;
    virtual ~Pipeline() = default;

    Buffer process(Buffer input);

  private:
    std::vector<std::shared_ptr<IPlugin>> plugins_;

    PipelineSink sink_;
    PipelineSource source_;
};

} // namespace Plugins

#endif
