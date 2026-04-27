#include "effects/filter.h"

namespace Plugins
{

ComplexBuffer Filter::process(ComplexBuffer buffer)
{
    return buffer;
}

void Filter::setSettings(const nlohmann::json &settings)
{
    lowCutoffFrequency_ = settings["lowCutoffFrequency"];
    highCutoffFrequency_ = settings["highCutoffFrequency"];
}

nlohmann::json Filter::getSettings() const
{
    return {{"lowCutoffFrequency", lowCutoffFrequency_}, {"highCutoffFrequency", highCutoffFrequency_}};
}

std::string Filter::getName() const
{
    return "Filter";
}

} // namespace Plugins
