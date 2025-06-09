#include "audioplayer.h"
#include "audioconverter.h"
#include <QMediaDevices>

namespace {

constexpr uint32_t BUF_BYTES = 256 * sizeof(int16_t);
constexpr uint32_t CHANNEL_COUNT = 2;
constexpr uint32_t BUF_FRAMES = BUF_BYTES / CHANNEL_COUNT / sizeof(int16_t);
constexpr uint32_t SAMPLE_RATE = 44100;

} // namespace

AudioPlayer::AudioPlayer(QObject* parent) noexcept
    : QObject(parent)
    , m_sink(new QAudioSink())
    , m_isPlaying(false)
    , m_device(QMediaDevices::defaultAudioOutput())
{
    m_format.setSampleFormat(QAudioFormat::Int16);
    m_format.setSampleRate(SAMPLE_RATE);
    m_format.setChannelCount(CHANNEL_COUNT);
}

void AudioPlayer::setDevice(const std::string& device)
{
    const auto audioDevices = QMediaDevices::audioOutputs();

    auto it = std::find_if(audioDevices.constBegin(), audioDevices.constEnd(),
                           [&](const QAudioDevice& currentDevice){
                               return currentDevice.description() == device;
    });

    if (it != audioDevices.end())
    {
        m_device = *it;
    }
    else
    {
        m_device = QMediaDevices::defaultAudioOutput();
    }

    m_sink.reset(new QAudioSink(m_device, m_format, this));
}

void AudioPlayer::startPlaying()
{
    if (m_isPlaying)
        return;

    m_buffer.open(QBuffer::ReadWrite);
    m_sink->start(&m_buffer);
    m_isPlaying = true;
}

void AudioPlayer::stopPlaying()
{
    if (!m_isPlaying)
        return;

    m_sink->stop();
    m_buffer.close();
    m_isPlaying = false;
}

void AudioPlayer::playSound(const std::span<float> &data) {
    if (!m_isPlaying)
        return;

    QByteArray byteArray = AudioConverter::fromFloatArray(data);
    m_buffer.write(byteArray);
}
