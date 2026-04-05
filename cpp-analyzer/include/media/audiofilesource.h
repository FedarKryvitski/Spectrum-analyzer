#ifndef AUDIOFILESOURCE_H
#define AUDIOFILESOURCE_H

#include "media/audiosource.h"

namespace Media {

class AudioFileSource final : public AudioSource
{
  public:
    AudioFileSource() noexcept = default;
    ~AudioFileSource() override;

    void open(const std::string &device) override;
    void close() override;

    Buffer read() override;
};

} // namespace Media

#endif // AUDIOFILESOURCE_H
