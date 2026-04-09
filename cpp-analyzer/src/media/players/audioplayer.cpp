#include "media/players/audioplayer.h"
#include "media/constants.h"

#include <iostream>

namespace Media
{

AudioPlayer::~AudioPlayer()
{
    stop();
}

void AudioPlayer::setDevice(const std::string &device)
{
    device_ = device;
}

void AudioPlayer::start()
{
    if (isPlaying_)
        return;

    int err = snd_pcm_open(&handle_, device_.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0)
    {
        std::cout << "Cannot open audio device " << device_ << snd_strerror(err);
        return;
    }

    err = snd_pcm_set_params(handle_, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, kChannels, kSampleRate, 1, 50000);
    if (err < 0)
    {
        std::cout << "Playback open error: " << snd_strerror(err);
        return;
    }

    snd_pcm_prepare(handle_);
    isPlaying_ = true;
}

void AudioPlayer::stop()
{
    if (!isPlaying_)
        return;

    snd_pcm_close(handle_);
    isPlaying_ = false;
}

void AudioPlayer::write(const void* buffer, size_t size)
{
    if (!isPlaying_)
        return;

    int err = snd_pcm_writei(handle_, buffer, size / kChannels);
    if (err < 0)
    {
        if (err == -EPIPE)
        {
            std::cerr << "buffer overflow" << std::endl;
            snd_pcm_prepare(handle_);
        }
        else
        {
            std::cerr << "write to audio interface failed" << snd_strerror(err) << std::endl;
        }
    }
}

} // namespace Media
