#pragma once

#include <cstdint>
#include <vector>

namespace Media
{

using Sample = int16_t;
using Buffer = std::vector<Sample>;

constexpr int kChannels = 1;
constexpr int kSampleRate = 44100;
constexpr int kBufferSize = 512;

} // namespace Media
