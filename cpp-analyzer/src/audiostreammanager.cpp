#include "audiostreammanager.h"

#include "media/audiodevicesource.h"
#include "media/audiofilesource.h"

#include "audioconverter.h"
#include "plugins/pipeline.h"

#include <exception>

AudioStreamManager::AudioStreamManager(QObject *parent) : QObject(parent)
{
    audioPlayer_ = std::make_unique<Media::AudioPlayer>();
    audioDeviceRecorder_ = std::make_unique<Media::AudioDeviceSource>();
    audioFileRecorder_ = std::make_unique<Media::AudioFileSource>();
}

AudioStreamManager::~AudioStreamManager()
{
    stop();
}

void AudioStreamManager::start(const AudioSessionConfig &config)
{
    if (isRunning_)
        return;

    workerThread_ = std::jthread([this, config](std::stop_token stopToken) { run(config, stopToken); });
}

void AudioStreamManager::stop()
{
    if (!isRunning_ && !workerThread_.joinable())
        return;

    isRunning_ = false;

    if (workerThread_.joinable())
    {
        workerThread_.request_stop();
        workerThread_.join();
    }
}

bool AudioStreamManager::isRunning() const
{
    return isRunning_;
}

void AudioStreamManager::run(AudioSessionConfig config, std::stop_token stopToken)
{
    Plugins::Pipeline pipeline;

    auto &recorder = config.inputType == InputType::Microphone ? audioDeviceRecorder_ : audioFileRecorder_;

    try
    {
        recorder->open(config.source.toStdString());

        audioPlayer_->start();
        isRunning_ = true;

        while (!stopToken.stop_requested() && isRunning_)
        {
            auto data = recorder->read();
            if (data.empty())
                continue;

            auto inputData = AudioConverter::toDoubleVector(data);
            auto outputData = pipeline.process(inputData);
            auto outputIntData = AudioConverter::toIntVector(outputData);

            emit volumeChanged(pipeline.getVolume());
            emit frameReady(inputData, outputData);

            audioPlayer_->write(outputIntData.data(), outputIntData.size());
        }
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString::fromStdString(e.what()));
    }
    catch (...)
    {
        emit errorOccurred("Unknown audio processing error.");
    }

    recorder->close();
    audioPlayer_->stop();

    isRunning_ = false;
    emit finished();
}
