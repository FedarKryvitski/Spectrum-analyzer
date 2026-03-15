#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <alsa/asoundlib.h>
#include <string>

namespace Alsa
{

class AudioPlayer final
{
  public:
    AudioPlayer() noexcept = default;

    virtual ~AudioPlayer();

    void start();

    void stop();

    void setDevice(const std::string &device);

    void write(const void* buffer, size_t size);

  private:
    snd_pcm_t *handle_{nullptr};
    std::string device_{"default"};
    bool isPlaying_{false};
};

} // namespace Alsa

#endif // AUDIOPLAYER_H
