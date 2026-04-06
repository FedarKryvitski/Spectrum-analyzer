#pragma once

#include "types.h"

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
