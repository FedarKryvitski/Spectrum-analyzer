#include "audioconverter.h"

#include <algorithm>
#include <limits>

namespace AudioConverter
{

std::vector<double> toDoubleVector(std::span<const int16_t> data)
{
    std::vector<double> result;
    std::ranges::transform(data, std::back_inserter(result), [](const auto &sample) {
        return static_cast<double>(sample) / std::numeric_limits<int16_t>::max();
    });

    return result;
}

std::vector<int16_t> toIntVector(std::span<const double> data)
{
    std::vector<int16_t> result;
    std::ranges::transform(data, std::back_inserter(result), [](const auto &sample) {
        return static_cast<int16_t>(sample * std::numeric_limits<int16_t>::max());
    });

    return result;
}

} // namespace AudioConverter
