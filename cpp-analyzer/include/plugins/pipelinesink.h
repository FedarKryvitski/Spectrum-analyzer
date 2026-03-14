#ifndef PIPELINESINK_H
#define PIPELINESINK_H

#include "plugins/types.h"

namespace Pipeline {
class PipelineSink
{
  public:
    PipelineSink() noexcept = default;
    virtual ~PipelineSink() = default;

    Buffer read(ComplexBuffer buffer);
};

} // namespace Pipeline

#endif // PIPELINESINK_H
