#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Media
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
} // namespace Media

#endif // UTILS_H
