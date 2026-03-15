#include "plugins/pipelinesource.h"

#include <algorithm>

namespace Pipeline {

ComplexBuffer PipelineSource::write(Buffer buffer)
{
    ComplexBuffer output;

    // TODO dft
    std::ranges::transform(buffer.cbegin(), buffer.cend(), output.begin(), [](const Sample sample){
        return Complex{sample, 0.0};
    });

    return output;
}

} // namespace Pipeline
