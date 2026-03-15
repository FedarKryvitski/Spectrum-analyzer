#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <alsa/asoundlib.h>
#include "alsa/constants.h"

#include <string>

namespace Alsa {

class AudioRecorder final
{
  public:
    AudioRecorder() noexcept = default;

    virtual ~AudioRecorder();

    void start();
    void stop();

    void setDevice(const std::string &device);

    Buffer read();

  private:
    snd_pcm_t* handle_{};
    std::string deviceName_;
    bool isRecording_{false};
};

} // namespace Alsa

#endif // AUDIORECORDER_H
