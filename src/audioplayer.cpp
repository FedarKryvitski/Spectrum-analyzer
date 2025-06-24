#include "audioplayer.h"
#include "audioconverter.h"
#include <QDebug>

namespace {

constexpr static uint32_t CHANNELS = 2;
constexpr static uint32_t BUF_SAMPLES = 128;
constexpr static uint32_t BUF_BYTES = BUF_SAMPLES * sizeof(int16_t) * CHANNELS;
constexpr static uint32_t RATE = 44100;
constexpr static snd_pcm_format_t FORMAT = SND_PCM_FORMAT_S16_LE;

}

void AudioPlayer::setDevice(const std::string& device)
{
    m_device = device;
}

void AudioPlayer::start()
{
    int err = snd_pcm_open (&playback_handle, m_device.c_str(),
                           SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        qDebug() << "cannot open audio device " << m_device << snd_strerror(err);
        return;
    }

    err = snd_pcm_set_params(playback_handle, FORMAT, SND_PCM_ACCESS_RW_INTERLEAVED,
                             CHANNELS, RATE, 1, 50000);
    if (err < 0) {
        qDebug() << "Playback open error: " << snd_strerror(err);
        return;
    }

    snd_pcm_prepare(playback_handle);
}

void AudioPlayer::stop()
{
    snd_pcm_close(playback_handle);
}

void AudioPlayer::playSound(const std::span<float> &data)
{
    QByteArray buffer = AudioConverter::fromFloatArray(data);
    int err = snd_pcm_writei(playback_handle, buffer.constData(),
                             data.size() / CHANNELS);
    if (err < 0) {
        if (err == -EPIPE) {
            qDebug() << "XRUN (переполнение буфера), восстанавливаем...";
            snd_pcm_prepare(playback_handle);
        } else {
            qDebug() << "write to audio interface failed" << snd_strerror(err);
        }
    }
}
