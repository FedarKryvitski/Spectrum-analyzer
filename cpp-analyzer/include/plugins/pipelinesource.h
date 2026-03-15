#ifndef PIPELINESOURCE_H
#define PIPELINESOURCE_H

#include "types.h"

namespace Pipeline
{

class PipelineSource
{
  public:
    PipelineSource() noexcept = default;
    virtual ~PipelineSource() = default;

    ComplexBuffer write(Buffer buffer);
};

} // namespace Pipeline

#endif // PIPELINESOURCE_H
