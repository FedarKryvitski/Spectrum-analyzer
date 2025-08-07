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
    m_device = device;
}

void AudioPlayer::start()
{
    if (m_isPlaying)
        return;

    int err = snd_pcm_open (&m_handle, m_device.c_str(),
                           SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        qDebug() << "cannot open audio device " << m_device << snd_strerror(err);
        return;
    }

    err = snd_pcm_set_params(m_handle, FORMAT, SND_PCM_ACCESS_RW_INTERLEAVED,
                             CHANNELS, RATE, 1, 50000);
    if (err < 0) {
        qDebug() << "Playback open error: " << snd_strerror(err);
        return;
    }

    snd_pcm_prepare(m_handle);
    m_isPlaying = true;
}

void AudioPlayer::stop()
{
    if (!m_isPlaying)
        return;

    snd_pcm_close(m_handle);
    m_isPlaying = false;
}

void AudioPlayer::playSound(const std::span<float> &data)
{
    int err = snd_pcm_writei(m_handle, data.data(), data.size() / CHANNELS);
    if (err < 0) {
        if (err == -EPIPE) {
            qDebug() << "XRUN (переполнение буфера), восстанавливаем...";
            snd_pcm_prepare(m_handle);
        } else {
            qDebug() << "write to audio interface failed" << snd_strerror(err);
        }
    }
}
