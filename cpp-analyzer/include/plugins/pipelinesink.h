#ifndef PIPELINESINK_H
#define PIPELINESINK_H

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

#endif // PIPELINESINK_H
