#include "effects/limiter.h"

#include <algorithm>

namespace Plugins {

ComplexBuffer Limiter::process(ComplexBuffer buffer)
{
    std::ranges::transform(buffer, buffer.begin(), [this](const Complex& elem){
        const auto amplitude = elem.real();
        const auto phase = elem.imag();
        const auto limitedAmplitude = std::clamp(amplitude, -limitLevel_, limitLevel_);

        return Complex{limitedAmplitude, phase};
    });

    return buffer;
}

} // namespace Plugins
