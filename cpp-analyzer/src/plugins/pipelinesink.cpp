#include "plugins/pipelinesink.h"

#include <algorithm>

namespace Pipeline {

Buffer PipelineSink::read(ComplexBuffer buffer)
{
    Buffer output;

    // TODO idft
    std::ranges::transform(buffer, output.begin(), [](const Complex element){
        const auto amplitude = element.real();
        return amplitude;
    });

    return output;
}

} // namespace Pipeline
