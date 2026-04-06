#pragma once

#include "types.h"

namespace Plugins {

class PipelineSink final
{
  public:
    PipelineSink() noexcept = default;
    virtual ~PipelineSink() = default;

    Buffer read(ComplexBuffer buffer);
};

} // namespace Plugins
