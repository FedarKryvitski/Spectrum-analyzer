#pragma once

#include "media/audiosource.h"

struct AVFormatContext;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;
struct SwrContext;

namespace Media {

class AudioFileSource final : public AudioSource
{
  public:
    AudioFileSource() noexcept = default;
    ~AudioFileSource() override;

    void open(const std::string &file) override;
    void close() override;

    Buffer read() override;

  private:
    bool initResampler();

  private:
    AVFormatContext* formatContext_ = nullptr;
    AVCodecContext* codecContext_ = nullptr;
    AVFrame* frame_ = nullptr;
    AVPacket* packet_ = nullptr;
    SwrContext* swrContext_ = nullptr;

    int audioStreamIndex_ = -1;
    bool endOfFile_ = false;
    bool isRecording_ = false;
};

} // namespace Media
