#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <alsa/asoundlib.h>
#include <QAudioDevice>
#include <QByteArray>

class AudioRecorder
{
public:
    AudioRecorder() noexcept = default;
    virtual ~AudioRecorder();

    void start();
    void stop();

    std::vector<float> getData();
    void setDevice(const std::string& device);

    void setBufferSize(uint32_t size) { m_bufFrames = size; }
    uint32_t getBufferSize() const { return m_bufFrames; }

private:
    snd_pcm_t *m_handle{nullptr};
    std::string m_device{"default"};
    uint32_t m_bufFrames{256};
    bool m_isRecording{false};
};

#endif // AUDIORECORDER_H

