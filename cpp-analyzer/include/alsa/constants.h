#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <vector>

namespace Alsa {

using Sample = int16_t;
using Buffer = std::vector<Sample>;

constexpr int kChannels = 1;
constexpr int kSampleRate = 44100;
constexpr int kBufferSize = 256;

} // namespace Alsa

#endif // CONSTANTS_H
