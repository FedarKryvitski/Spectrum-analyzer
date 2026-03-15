#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Alsa
{
namespace Utils
{

struct DeviceInfo
{
    std::string name;
    std::string displayName;
};

std::vector<DeviceInfo> getAudioInputDevices();

std::vector<DeviceInfo> getAudioOutputDevices();

} // namespace Utils
} // namespace Alsa

#endif // UTILS_H
