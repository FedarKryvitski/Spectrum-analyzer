#pragma once

#include "base/types.h"

namespace Plugins
{

class PipelineSource
{
  public:
    PipelineSource() noexcept = default;
    virtual ~PipelineSource() = default;

    ComplexBuffer write(Buffer buffer);
};

} // namespace Plugins
