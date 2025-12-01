#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <alsa/asoundlib.h>
#include <vector>
#include <string>

class AudioRecorder
{
public:
    AudioRecorder() noexcept = default;
    virtual ~AudioRecorder();

    void start();
    void stop();

    void setDevice(const std::string& device);

    std::vector<float> data();

    void setBufferSize(size_t size);
    size_t getBufferSize() const;

private:
    snd_pcm_t *handle_{ nullptr };
    std::string device_{ "default" };
    size_t bufFrames_{ 256 };
    bool isRecording_{ false };
};

#endif // AUDIORECORDER_H

