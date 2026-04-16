#include "pipeline/pipeline.h"

#include <algorithm>

namespace Plugins
{

namespace
{

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

static double calculateVolume(const Buffer &buffer)
{
    double result = 0.0;

    std::ranges::for_each(buffer, [&result](const auto &sample) { result += std::abs(sample); });

    result /= buffer.size();

    return linearToExponent(result);
}

} // namespace

Pipeline::Pipeline() noexcept
{
}

Buffer Pipeline::process(Buffer input)
{
    inputVolume_ = calculateVolume(input);
    ComplexBuffer complexBuffer = source_.write(std::move(input));

    {
        std::lock_guard lock(mutex_);

        for (auto &plugin : plugins_)
        {
            if (plugin && plugin->isEnabled())
            {
                complexBuffer = plugin->process(std::move(complexBuffer));
            }
        }
    }

    auto buffer = sink_.read(complexBuffer);
    outputVolume_ = calculateVolume(buffer);

    return buffer;
}

void Pipeline::addPlugin(std::shared_ptr<IPlugin> plugin)
{
    if (!plugin)
        return;

    std::lock_guard lock(mutex_);
    plugins_.push_back(std::move(plugin));
}

void Pipeline::removePlugin(int index)
{
    std::lock_guard lock(mutex_);

    if (index >= 0 && index < static_cast<int>(plugins_.size()))
    {
        plugins_.erase(plugins_.begin() + index);
    }
}

void Pipeline::movePlugin(int oldIndex, int newIndex)
{
    std::lock_guard lock(mutex_);

    int size = static_cast<int>(plugins_.size());
    if (oldIndex < 0 || oldIndex >= size || newIndex < 0 || newIndex >= size)
    {
        return;
    }

    std::swap(plugins_[oldIndex], plugins_[newIndex]);
}

void Pipeline::togglePlugin(int index, bool enabled)
{
    std::lock_guard lock(mutex_);

    int size = static_cast<int>(plugins_.size());
    if (index < 0 || index >= size)
    {
        return;
    }

    auto plugin = plugins_[index];
    plugin->setEnabled(enabled);
}

void Pipeline::clear()
{
    std::lock_guard lock(mutex_);
    plugins_.clear();
}

std::vector<std::shared_ptr<IPlugin>> Pipeline::getPlugins() const
{
    std::lock_guard lock(mutex_);
    return plugins_;
}

double Pipeline::getInputVolume() const
{
    return inputVolume_;
}

double Pipeline::getOutputVolume() const
{
    return outputVolume_;
}

} // namespace Plugins
