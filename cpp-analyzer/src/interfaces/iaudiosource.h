#pragma once

#include "media/constants.h"

#include <string>

namespace Media
{

class IAudioSource
{
  public:
    IAudioSource() noexcept = default;
    virtual ~IAudioSource() = default;

    virtual void open(const std::string &device) = 0;
    virtual void close() = 0;

    virtual void setBufferSize(const size_t size) = 0;
    virtual size_t getBufferSize() const = 0;

    virtual Buffer read() = 0;
};

} // namespace Media
