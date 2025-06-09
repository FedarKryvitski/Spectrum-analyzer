#include "audiorecorder.h"
#include "audioconverter.h"
#include <QMediaDevices>
#include <algorithm>
#include <QDebug>
#include <QThread>

namespace {

constexpr auto SAMPLE_FORMAT = QAudioFormat::Int16;
constexpr uint32_t SAMPLE_RATE = 44100;
constexpr uint32_t CHANNELS = 2;
constexpr uint32_t BUFFER_MS = 100;
constexpr uint32_t SAMPLES_PER_BUFFER = (SAMPLE_RATE * BUFFER_MS) / 1000;

} // namespace

AudioRecorder::AudioRecorder(QObject* parent) noexcept
    : QObject(parent)
    , m_isRecording(false)
{
    m_format.setSampleFormat(SAMPLE_FORMAT);
    m_format.setSampleRate(SAMPLE_RATE);
    m_format.setChannelCount(CHANNELS);

    m_device = QMediaDevices::defaultAudioInput();

    m_source.reset(new QAudioSource(m_device, m_format, this));
    m_source->setBufferSize(SAMPLES_PER_BUFFER * sizeof(int16_t) * CHANNELS);

    connect(&m_buffer, &IOBuffer::dataReceived, this, &AudioRecorder::onDataReceived);
}

void AudioRecorder::onDataReceived()
{
    emit dataReceived();
}

void AudioRecorder::setDevice(const std::string& device)
{
    const auto inputDevices = QMediaDevices::audioInputs();

    auto it = std::find_if(inputDevices.constBegin(), inputDevices.constEnd(),
                           [&](const QAudioDevice& currentDevice) {
                               return currentDevice.description().toStdString() == device;
                           });

    if (it != inputDevices.end())
    {
        m_device = *it;
    }
    else
    {
        m_device = QMediaDevices::defaultAudioInput();
    }

    m_source.reset(new QAudioSource(m_device, m_format, this));
    m_source->setBufferSize(SAMPLES_PER_BUFFER * sizeof(int16_t) * CHANNELS);
}

void AudioRecorder::startRecording()
{
    if (m_isRecording)
        return;

    m_buffer.open(QIODevice::WriteOnly);
    m_source->start(&m_buffer);
    m_isRecording = true;
}

void AudioRecorder::stopRecording()
{
    if (!m_isRecording)
        return;

    m_source->stop();
    m_buffer.close();
    m_isRecording = false;
}

std::vector<float> AudioRecorder::getData()
{
    const QByteArray rawData = m_buffer.getData();
    std::vector<float> data = AudioConverter::fromByteArray(rawData);

    return data;
}
