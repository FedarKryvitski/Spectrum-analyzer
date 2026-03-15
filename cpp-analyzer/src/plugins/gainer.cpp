#include "plugins/gainer.h"

#include <algorithm>

namespace Plugins {

Gainer::Buffer Gainer::process(Buffer buffer)
{
    if (buffer.size() == 0)
        return buffer;

    std::ranges::transform(buffer.begin(), buffer.end(), buffer.begin(), [this](const Complex& element){
        const auto amplitude = element.real();
        const auto phase = element.imag();
        return Complex{amplitude * gainLevel_, phase};
    });

    return buffer;
}

} // namespace Plugins
