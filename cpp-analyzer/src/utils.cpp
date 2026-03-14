#include "utils.h"

#include <alsa/asoundlib.h>
#include <alsa/control.h>

#include <QDebug>

namespace Alsa {
namespace Utils{

using namespace std::string_literals;

std::vector<DeviceInfo> getAudioInputDevices()
{
    std::vector<DeviceInfo> audioInputs;

    snd_ctl_t *ctl;
    snd_ctl_card_info_t *info;
    int rcard{ -1 };

    while(1)
    {
        snd_card_next(&rcard);
        if(rcard == -1)
            break;

        std::string name = "hw:"s + std::to_string(rcard);

        snd_ctl_card_info_alloca(&info);

        if (snd_ctl_open(&ctl, name.c_str(), 0) >= 0) {
            snd_ctl_card_info(ctl, info);
            printf("Card %d: %s - %s\n",
                   rcard,
                   snd_ctl_card_info_get_name(info),
                   snd_ctl_card_info_get_longname(info));
            snd_ctl_close(ctl);
        }

        // snd_ctl_card_info_free(info);
    }

    return audioInputs;
}

std::vector<DeviceInfo> getAudioOutputDevices()
{
    std::vector<DeviceInfo> audioOutputs;
    return audioOutputs;
}

} // namespace Utils
} // namespace Alsa
