#include "audioplayer.h"
#include <QDebug>

namespace {

constexpr static uint32_t CHANNELS = 2;
constexpr static uint32_t RATE = 44100;
constexpr static snd_pcm_format_t FORMAT = SND_PCM_FORMAT_FLOAT;

}

AudioPlayer::~AudioPlayer()
{
    stop();
}

void AudioPlayer::setDevice(const std::string& device)
{
    device_ = device;
}

void AudioPlayer::start()
{
    if (isPlaying_)
        return;

    int err = snd_pcm_open (&handle_, device_.c_str(),
                           SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        qDebug() << "cannot open audio device " << device_ << snd_strerror(err);
        return;
    }

    err = snd_pcm_set_params(handle_, FORMAT, SND_PCM_ACCESS_RW_INTERLEAVED,
                             CHANNELS, RATE, 1, 50000);
    if (err < 0) {
        qDebug() << "Playback open error: " << snd_strerror(err);
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

void AudioPlayer::playSound(std::span<const float> data)
{
    int err = snd_pcm_writei(handle_, data.data(), data.size() / CHANNELS);
    if (err < 0) {
        if (err == -EPIPE) {
            qDebug() << "XRUN (переполнение буфера), восстанавливаем...";
            snd_pcm_prepare(handle_);
        } else {
            qDebug() << "write to audio interface failed" << snd_strerror(err);
        }
    }
}
