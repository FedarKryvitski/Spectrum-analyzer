#include "effects/gainer.h"

#include <QDebug>
#include <algorithm>

namespace Plugins
{

ComplexBuffer Gainer::process(ComplexBuffer buffer)
{
    std::ranges::transform(buffer.begin(), buffer.end(), buffer.begin(), [this](const Complex &element) {
        const auto amplitude = element.real();
        const auto phase = element.imag();
        return Complex{amplitude * gainLevel_, phase};
    });

    return buffer;
}

void Gainer::setSettings(const nlohmann::json &settings)
{
    gainLevel_ = settings["gainLevel"];
}

nlohmann::json Gainer::getSettings() const
{
    return {{"gainLevel", gainLevel_}};
}

std::string Gainer::getName() const
{
    return "Gainer";
}

} // namespace Plugins
