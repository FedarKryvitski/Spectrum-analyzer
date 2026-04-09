#include "media/sources/audiofilesource.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
}

#include <iostream>

namespace Media
{

AudioFileSource::~AudioFileSource()
{
    close();
}

void AudioFileSource::open(const std::string &file)
{
    if (isRecording_)
        return;

    if (avformat_open_input(&formatContext_, file.c_str(), nullptr, nullptr) < 0)
    {
        std::cerr << "Failed to open audio file: " << file << std::endl;
        return;
    }

    if (avformat_find_stream_info(formatContext_, nullptr) < 0)
    {
        std::cerr << "Failed to find stream info" << std::endl;
        return;
    }

    audioStreamIndex_ = av_find_best_stream(formatContext_, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (audioStreamIndex_ < 0)
    {
        std::cerr << "No audio stream found" << std::endl;
        return;
    }

    AVStream *stream = formatContext_->streams[audioStreamIndex_];
    const AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (!codec)
    {
        std::cerr << "Decoder not found" << std::endl;
        return;
    }

    codecContext_ = avcodec_alloc_context3(codec);
    if (!codecContext_)
    {
        std::cerr << "Failed to allocate codec context" << std::endl;
        return;
    }

    if (avcodec_parameters_to_context(codecContext_, stream->codecpar) < 0)
    {
        std::cerr << "Failed to copy codec parameters" << std::endl;
        return;
    }

    if (avcodec_open2(codecContext_, codec, nullptr) < 0)
    {
        std::cerr << "Failed to open codec" << std::endl;
        return;
    }

    frame_ = av_frame_alloc();
    packet_ = av_packet_alloc();

    if (!initResampler())
    {
        std::cerr << "Failed to initialize resampler" << std::endl;
        return;
    }

    endOfFile_ = false;
    isRecording_ = true;
}

bool AudioFileSource::initResampler()
{
    if (!codecContext_)
        return false;

    AVChannelLayout outLayout;
    av_channel_layout_default(&outLayout, kChannels);

    int result =
        swr_alloc_set_opts2(&swrContext_, &outLayout, AV_SAMPLE_FMT_S16, kSampleRate, &codecContext_->ch_layout,
                            codecContext_->sample_fmt, codecContext_->sample_rate, 0, nullptr);

    if (!swrContext_)
    {
        std::cerr << "Failed to init swrContext, error code: " << result;
        return false;
    }

    if (swr_init(swrContext_) < 0)
        return false;

    return true;
}

void AudioFileSource::close()
{
    if (!isRecording_)
        return;

    if (swrContext_)
    {
        swr_free(&swrContext_);
        swrContext_ = nullptr;
    }

    if (codecContext_)
    {
        avcodec_free_context(&codecContext_);
        codecContext_ = nullptr;
    }

    if (formatContext_)
    {
        avformat_close_input(&formatContext_);
        formatContext_ = nullptr;
    }

    if (frame_)
        av_frame_unref(frame_);

    if (packet_)
        av_packet_unref(packet_);

    audioStreamIndex_ = -1;
    endOfFile_ = false;
    isRecording_ = false;
}

Buffer AudioFileSource::read()
{
    if (!codecContext_ || !formatContext_ || !frame_ || !packet_ || endOfFile_)
        return {};

    Buffer outBuffer;

    while (true)
    {
        int ret = av_read_frame(formatContext_, packet_);
        if (ret < 0)
        {
            endOfFile_ = true;
            break;
        }

        if (packet_->stream_index != audioStreamIndex_)
        {
            av_packet_unref(packet_);
            continue;
        }

        ret = avcodec_send_packet(codecContext_, packet_);
        av_packet_unref(packet_);

        if (ret < 0)
            continue;

        while (avcodec_receive_frame(codecContext_, frame_) == 0)
        {
            const int64_t delay = swr_get_delay(swrContext_, codecContext_->sample_rate);
            const int outSamples =
                av_rescale_rnd(delay + frame_->nb_samples, kSampleRate, codecContext_->sample_rate, AV_ROUND_UP);

            const int totalOut = outSamples * kChannels;
            size_t oldSize = outBuffer.size();
            outBuffer.resize(oldSize + totalOut);

            uint8_t *outPtr[] = {reinterpret_cast<uint8_t *>(outBuffer.data() + oldSize)};

            const int converted = swr_convert(swrContext_, outPtr, outSamples, (const uint8_t **)frame_->extended_data,
                                              frame_->nb_samples);

            if (converted < 0)
            {
                std::cerr << "Error converting audio frame" << std::endl;
                return {};
            }

            outBuffer.resize(oldSize + converted * kChannels);
            return outBuffer;
        }
    }

    return outBuffer;
}

} // namespace Media
