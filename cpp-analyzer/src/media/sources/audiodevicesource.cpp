#include "media/sources/audiodevicesource.h"

#include <iostream>

namespace Media {

AudioDeviceSource::~AudioDeviceSource()
{
    close();
}

void AudioDeviceSource::open(const std::string &device)
{
    if (isRecording_)
        return;

    int err;
    if ((err = snd_pcm_open(&handle_, device.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        std::cerr << "cannot open audio device" << device << ":" << snd_strerror(err) << std::endl;
    }

    if ((err = snd_pcm_set_params(handle_, SND_PCM_FORMAT_S16_LE,  SND_PCM_ACCESS_RW_INTERLEAVED, kChannels, kSampleRate, 1, 500000)) < 0) {
        std::cerr << "capture open error:" << snd_strerror(err) << std::endl;
    }

    isRecording_ = true;
}

void AudioDeviceSource::close()
{
    if (!isRecording_)
        return;

    snd_pcm_close(handle_);
    isRecording_ = false;
}

Buffer AudioDeviceSource::read()
{
    if (!isRecording_)
        return {};

    Buffer data(bufferSize_ * kChannels);

    int err;
    if ((err = snd_pcm_readi(handle_, data.data(), bufferSize_)) != bufferSize_) {
        std::cerr << "read from audio interface failed: " << snd_strerror (err) << std::endl;
    }

    return data;
}

} // namespace Media
