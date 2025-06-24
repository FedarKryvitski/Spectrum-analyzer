#include "audiorecorder.h"
#include "audioconverter.h"
#include <QDebug>

namespace {

constexpr static uint32_t CHANNELS = 2;
constexpr static uint32_t RATE = 44100;
constexpr static snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

}

void AudioRecorder::setDevice(const std::string& device)
{
    m_device = device;
}

void AudioRecorder::start()
{
    int err{};
    if ((err = snd_pcm_open (&m_captureHandle, m_device.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        qDebug() << "Cannot open audio device " << m_device << snd_strerror(err);
        return;
    }

    if ((err = snd_pcm_set_params(m_captureHandle, format,  SND_PCM_ACCESS_RW_INTERLEAVED, CHANNELS, RATE, 1, 50000)) < 0) {   /* 50 ms */
        qDebug() << "Capture open error: " << snd_strerror(err);
        return;
    }
}

std::vector<float> AudioRecorder::getData(int samples)
{
    QByteArray buffer(samples * sizeof(int16_t) * CHANNELS, 0);
    int err = snd_pcm_readi(m_captureHandle, buffer.data(), samples);
    if (err < 0) {
        qDebug() << "read from audio interface failed" << snd_strerror(err);
        return {};
    }
    return AudioConverter::fromByteArray(buffer);
}

void AudioRecorder::stop()
{
    snd_pcm_close(m_captureHandle);
}
