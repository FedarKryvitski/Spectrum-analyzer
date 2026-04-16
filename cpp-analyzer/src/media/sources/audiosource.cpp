#include "media/sources/audiosource.h"

namespace Media
{

void AudioSource::setBufferSize(const size_t size)
{
    bufferSize_ = size;
}

size_t AudioSource::getBufferSize() const
{
    return bufferSize_;
}

} // namespace Media
