#include "audiorecorder.h"

#include <QAudioDevice>
#include <QAudioFormat>
#include <QAudioSource>
#include <QByteArray>
#include <QDebug>
#include <QMediaDevices>

namespace
{

constexpr size_t kChannels{1};
constexpr size_t kSampleRate{44100};

} // namespace

AudioRecorder::AudioRecorder(QObject *parent) noexcept : QObject(parent)
{
}

AudioRecorder::~AudioRecorder()
{
    stop();
}

void AudioRecorder::setDevice(const QString &device)
{
    deviceName_ = device;
}

void AudioRecorder::start()
{
    if (isRecording_)
        return;

    if (deviceName_.isEmpty())
    {
        qWarning() << "[AudioRecorder::start()] Empty device name";
        return;
    }

    QAudioFormat format;
    format.setSampleRate(kSampleRate);
    format.setChannelCount(kChannels);
    format.setSampleFormat(QAudioFormat::Int16);

    auto audioDeviceList = QMediaDevices::audioInputs();
    auto deviceIt =
        std::ranges::find_if(audioDeviceList, [&](const auto &device) { return device.description() == deviceName_; });

    if (deviceIt == audioDeviceList.end())
    {
        qWarning() << "[AudioRecorder::start()] Device not found";
        return;
    }

    auto audioDevice = *deviceIt;
    if (!audioDevice.isFormatSupported(format))
    {
        qWarning() << "[AudioRecorder::start()] Format not supported";
        return;
    }

    reset();

    audioSource_ = std::make_unique<QAudioSource>(audioDevice, format, this);

    IODevice_ = audioSource_->start();
    if (!IODevice_)
    {
        qWarning() << "[AudioRecorder::start()] Failed to start";
        audioSource_.reset();
        return;
    }

    connect(IODevice_, &QIODevice::readyRead, this, [this]() { emit readyRead(); });

    isRecording_ = true;
}

std::vector<double> AudioRecorder::data()
{
    if (!isRecording_ || !IODevice_)
        return {};

    auto bytes = IODevice_->bytesAvailable();
    QByteArray byteArray = IODevice_->read(bytes);

    auto size = byteArray.size() / sizeof(int16_t);
    const int16_t *ptr = reinterpret_cast<const int16_t *>(byteArray.constData());

    std::vector<double> data;
    for (int i = 0; i < size; ++i)
    {
        auto value = static_cast<double>(ptr[i]) / 33'000;
        data.push_back(value);
    }

    return data;
}

void AudioRecorder::stop()
{
    if (!isRecording_)
        return;

    reset();
    isRecording_ = false;
}

void AudioRecorder::reset()
{
    if (audioSource_)
    {
        audioSource_->stop();
    }

    IODevice_ = nullptr;
    audioSource_.reset();
}
