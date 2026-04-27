#include "effects/limiter.h"

#include <algorithm>

namespace Plugins
{

ComplexBuffer Limiter::process(ComplexBuffer buffer)
{
    std::ranges::transform(buffer, buffer.begin(), [this](const Complex &elem) {
        const auto amplitude = elem.real();
        const auto phase = elem.imag();
        const auto limitedAmplitude = amplitude; // std::clamp(amplitude, -limitLevel_, limitLevel_);

        return Complex{limitedAmplitude, phase};
    });

    return buffer;
}

void Limiter::setSettings(const nlohmann::json &settings)
{
    thresholdDb_ = settings["thresholdDb"];
    ceilingDb_ = settings["ceilingDb"];
    releaseMs_ = settings["releaseMs"];
}

nlohmann::json Limiter::getSettings() const
{
    return {
        {"thresholdDb", thresholdDb_},
        {"ceilingDb", ceilingDb_},
        {"releaseMs", releaseMs_},
    };
}

std::string Limiter::getName() const
{
    return "Limiter";
}

} // namespace Plugins
