#pragma once

#include "iaudiosource.h"

namespace Media {

class AudioSource : public IAudioSource
{
  public:
    AudioSource() noexcept = default;
    ~AudioSource() override = default;

    void setBufferSize(const size_t size) override;
    size_t getBufferSize() const override;

  protected:
    size_t bufferSize_{ Media::kBufferSize };
};

} // namespace Media
