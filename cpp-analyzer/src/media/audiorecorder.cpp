#include "media/audiorecorder.h"

#include <iostream>

namespace Media {

namespace {

} // namespace

AudioRecorder::~AudioRecorder()
{
    stop();
}

void AudioRecorder::setDevice(const std::string &device)
{
    deviceName_ = device;
}

void AudioRecorder::start()
{
    if (isRecording_)
        return;

    int err;
    if ((err = snd_pcm_open(&handle_, deviceName_.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        std::cerr << "cannot open audio device" << deviceName_ << ":" << snd_strerror(err) << std::endl;
    }

    if ((err = snd_pcm_set_params(handle_, SND_PCM_FORMAT_S16_LE,  SND_PCM_ACCESS_RW_INTERLEAVED, kChannels, kSampleRate, 1, 500000)) < 0) {
        std::cerr << "capture open error:" << snd_strerror(err) << std::endl;
    }

    isRecording_ = true;
}

void AudioRecorder::stop()
{
    if (!isRecording_)
        return;

    snd_pcm_close(handle_);
    isRecording_ = false;
}

Buffer AudioRecorder::read()
{
    if (!isRecording_)
        return {};

    Buffer data(kBufferSize * kChannels);

    int err;
    if ((err = snd_pcm_readi(handle_, data.data(), kBufferSize)) != kBufferSize) {
        std::cerr << "read from audio interface failed: " << snd_strerror (err) << std::endl;
    }

    return data;
}

} // namespace Media
