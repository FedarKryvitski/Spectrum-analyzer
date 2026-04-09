#pragma once

#include "media/sources/audiosource.h"

#include <alsa/asoundlib.h>

namespace Media {

class AudioDeviceSource final : public AudioSource
{
  public:
    AudioDeviceSource() noexcept = default;
    ~AudioDeviceSource() override;

    void open(const std::string &device) override;
    void close() override;

    Buffer read() override;

  private:
    snd_pcm_t* handle_{};
    bool isRecording_{false};
};

} // namespace Media
