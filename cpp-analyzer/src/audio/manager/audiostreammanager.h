#pragma once

#include "../session/audiosessionconfig.h"
#include "../../media/players/audioplayer.h"
#include "interfaces/iaudiosource.h"

#include <QObject>
#include <atomic>
#include <memory>
#include <thread>
#include <vector>

class AudioStreamManager : public QObject
{
    Q_OBJECT

  public:
    explicit AudioStreamManager(QObject *parent = nullptr);
    ~AudioStreamManager();

    void start(const AudioSessionConfig &config);
    void stop();

    bool isRunning() const;

  signals:
    void volumeChanged(double volume);
    void frameReady(const std::vector<double> &input, const std::vector<double> &output);
    void errorOccurred(const QString &message);
    void finished();

  private:
    void run(AudioSessionConfig config, std::stop_token stopToken);

  private:
    std::atomic_bool isRunning_{false};
    std::jthread workerThread_;

    std::unique_ptr<Media::IAudioSource> audioDeviceRecorder_{nullptr};
    std::unique_ptr<Media::IAudioSource> audioFileRecorder_{nullptr};
    std::unique_ptr<Media::AudioPlayer> audioPlayer_{nullptr};
};
