#include "plugins/pipeline.h"
#include "plugins/gainer.h"
#include "plugins/limiter.h"

#include <algorithm>

#include <QDebug>

namespace Plugins {

Pipeline::Pipeline() noexcept
{
    auto gainer = std::make_shared<Gainer>();
    gainer->setGainLevel(2.0);

    auto limiter = std::make_shared<Limiter>();
    limiter->setLimitLevel(0.5);

    plugins_.push_back(gainer);
    plugins_.push_back(limiter);
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

    return buffer;
}

} // namespace Plugins
