#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QByteArray>
#include <alsa/asoundlib.h>
#include <span>

class AudioPlayer
{
public:
    AudioPlayer() noexcept = default;
    virtual ~AudioPlayer();

    void start();
    void stop();
    void playSound(const std::span<float>& data);
    void setDevice(const std::string& device);

private:
    snd_pcm_t *m_handle{nullptr};
    std::string m_device{"default"};
    bool m_isPlaying{false};
};

#endif // AUDIOPLAYER_H
