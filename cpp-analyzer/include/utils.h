#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

namespace Alsa {
namespace Utils{

struct DeviceInfo {
    std::string name;
    std::string displayName;
};

std::vector<DeviceInfo> getAudioInputDevices();

std::vector<DeviceInfo> getAudioOutputDevices();

}
}

#endif // UTILS_H
