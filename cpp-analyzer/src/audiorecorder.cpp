#include "audiorecorder.h"
#include <QDebug>

namespace {

constexpr static uint32_t CHANNELS = 2;
constexpr static uint32_t RATE = 44100;
constexpr static snd_pcm_format_t format = SND_PCM_FORMAT_FLOAT;

}

AudioRecorder::~AudioRecorder()
{
    stop();
}

void AudioRecorder::setDevice(const std::string& device)
{
    m_device = device;
}

void AudioRecorder::start()
{
    if (m_isRecording)
        return;

    int err{};
    if ((err = snd_pcm_open (&m_handle, m_device.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        qDebug() << "Cannot open audio device " << m_device << snd_strerror(err);
        return;
    }

    if ((err = snd_pcm_set_params(m_handle, format,  SND_PCM_ACCESS_RW_INTERLEAVED, CHANNELS, RATE, 1, 50000)) < 0) {   /* 50 ms */
        qDebug() << "Capture open error: " << snd_strerror(err);
        return;
    }

    m_isRecording = true;
}

std::vector<float> AudioRecorder::getData()
{
    std::vector<float> data(m_bufFrames * CHANNELS);
    int err = snd_pcm_readi(m_handle, data.data(), m_bufFrames);
    if (err < 0) {
        qDebug() << "read from audio interface failed" << snd_strerror(err);
        return {};
    }
    return data;
}

void AudioRecorder::stop()
{
    if (!m_isRecording)
        return;

    snd_pcm_close(m_handle);
    m_isRecording = false;
}
