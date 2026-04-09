#include "pipeline/pipeline.h"
#include "effects/gainer.h"
#include "effects/limiter.h"

#include <algorithm>

#include <QDebug>

namespace Plugins {

namespace {

static double linearToExponent(const double linear)
{
    constexpr double minDB = -60.0;

    if (linear <= 0.0)
        return 0.0;

    if (linear >= 1.0)
        return 100.0;

    double db = 20.0 * std::log10(linear);
    double percent = 100.0 * (db - minDB) / (0.0 - minDB);

    return std::clamp(percent, 0.0, 100.0);
}

static double calculateVolume(const Buffer& buffer)
{
    double result = 0.0;

    std::ranges::for_each(buffer, [&result](const auto& sample){
        result += std::abs(sample);
    });

    result /= buffer.size();

    return linearToExponent(result);
}

}

Pipeline::Pipeline() noexcept
{
    // auto gainer = std::make_shared<Gainer>();
    // gainer->setGainLevel(2.0);

    // auto limiter = std::make_shared<Limiter>();
    // limiter->setLimitLevel(0.5);

    // plugins_.push_back(gainer);
    // plugins_.push_back(limiter);
}

Buffer Pipeline::process(Buffer input)
{
    ComplexBuffer complexBuffer = source_.write(input);

    std::ranges::for_each(plugins_.begin(), plugins_.end(), [&](std::shared_ptr<IPlugin>& plugin){
        if (!plugin->isEnabled())
            return;

        complexBuffer = plugin->process(std::move(complexBuffer));
    });

    Buffer buffer = sink_.read(complexBuffer);

    volume_ = calculateVolume(buffer);

    return buffer;
}

double Pipeline::getVolume() const
{
    return volume_;
}

} // namespace Plugins
