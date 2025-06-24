#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <alsa/asoundlib.h>
#include <QAudioDevice>
#include <QByteArray>
#include <QObject>

class AudioRecorder {
public:
    AudioRecorder() noexcept = default;

    void setDevice(const std::string& device);
    void start();
    void stop();
    std::vector<float> getData(int samples);

private:
    snd_pcm_t *m_captureHandle;
    std::string m_device{"default"};
};

#endif // AUDIORECORDER_H

