#pragma once

#include <alsa/asoundlib.h>
#include <string>

namespace Media
{

class AudioPlayer final
{
  public:
    AudioPlayer() noexcept = default;

    virtual ~AudioPlayer();

    void start();

    void stop();

    void setDevice(const std::string &device);

    void write(const void *buffer, size_t size);

  private:
    snd_pcm_t *handle_{nullptr};
    std::string device_{"default"};
    bool isPlaying_{false};
};

} // namespace Media
