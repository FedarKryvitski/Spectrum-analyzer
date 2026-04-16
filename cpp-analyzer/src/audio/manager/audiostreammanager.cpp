#include "audiostreammanager.h"

#include "sources/audiodevicesource.h"
#include "sources/audiofilesource.h"

#include "converter/audioconverter.h"
#include "pipeline/pipeline.h"

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

void AudioStreamManager::start(const AudioSessionConfig &config, Plugins::Pipeline *pipeline)
{
    if (isRunning_)
        return;

    workerThread_ =
        std::jthread([this, pipeline, config](std::stop_token stopToken) { run(config, pipeline, stopToken); });
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

void AudioStreamManager::run(AudioSessionConfig config, Plugins::Pipeline *pipeline, std::stop_token stopToken)
{
    auto &recorder = config.inputType == InputType::Microphone ? audioDeviceRecorder_ : audioFileRecorder_;

    recorder->open(config.source.toStdString());
    audioPlayer_->start();
    isRunning_ = true;

    while (!stopToken.stop_requested() && isRunning_)
    {
        auto data = recorder->read();
        if (data.empty())
            continue;

        auto inputData = AudioConverter::toDoubleVector(data);
        auto outputData = pipeline->process(inputData);
        auto outputIntData = AudioConverter::toIntVector(outputData);

        emit frameReady(inputData, outputData);
        emit inputVolumeChanged(pipeline->getInputVolume());
        emit outputVolumeChanged(pipeline->getOutputVolume());

        audioPlayer_->write(outputIntData.data(), outputIntData.size());
    }

    recorder->close();
    audioPlayer_->stop();

    isRunning_ = false;
    emit finished();
}
