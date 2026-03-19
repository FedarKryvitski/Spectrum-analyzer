#include "plugins/pipelinesink.h"

#include <algorithm>

namespace Plugins {

Buffer PipelineSink::read(ComplexBuffer buffer)
{
    Buffer output;

    // TODO idft
    std::ranges::transform(buffer, std::back_inserter(output), [](const Complex element){
        const auto amplitude = element.real();
        return amplitude;
    });

    return output;
}

} // namespace Plugins
