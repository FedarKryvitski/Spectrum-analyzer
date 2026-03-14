#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <alsa/asoundlib.h>
#include <span>
#include <string>

namespace Alsa {

class AudioPlayer
{
public:
    AudioPlayer() noexcept = default;
    virtual ~AudioPlayer();

    void start();
    void stop();

    void setDevice(const std::string& device);

    void playSound(std::span<const double> data);

private:
    snd_pcm_t *handle_{ nullptr };
    std::string device_{ "default" };
    bool isPlaying_{ false };
};

} // namespace Alsa

#endif // AUDIOPLAYER_H
