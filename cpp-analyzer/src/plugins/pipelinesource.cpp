#include "plugins/pipelinesource.h"

#include <algorithm>

namespace Plugins {

ComplexBuffer PipelineSource::write(Buffer buffer)
{
    ComplexBuffer output;

    std::ranges::transform(buffer.cbegin(), buffer.cend(), std::back_inserter(output), [](const Sample& sample){
        return Complex{sample, 0.0};
    });

    return output;
}

} // namespace Plugins
