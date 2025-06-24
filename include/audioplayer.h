#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QByteArray>
#include <alsa/asoundlib.h>
#include <span>

class AudioPlayer {
public:
    AudioPlayer() noexcept = default;

    void setDevice(const std::string& device);
    void start();
    void stop();
    void playSound(const std::span<float>& data);

private:
    snd_pcm_t *playback_handle;
    std::string m_device{"default"};
};

#endif // AUDIOPLAYER_H
