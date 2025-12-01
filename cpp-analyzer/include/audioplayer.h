#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <alsa/asoundlib.h>
#include <span>
#include <string>

class AudioPlayer
{
public:
    AudioPlayer() noexcept = default;
    virtual ~AudioPlayer();

    void start();
    void stop();

    void setDevice(const std::string& device);

    void playSound(std::span<const float> data);

private:
    snd_pcm_t *handle_{ nullptr };
    std::string device_{ "default" };
    bool isPlaying_{ false };
};

#endif // AUDIOPLAYER_H
