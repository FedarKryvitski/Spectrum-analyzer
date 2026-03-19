#ifndef PIPELINESOURCE_H
#define PIPELINESOURCE_H

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

#endif // PIPELINESOURCE_H
