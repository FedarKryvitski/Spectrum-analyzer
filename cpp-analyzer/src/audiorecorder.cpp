#include "audiorecorder.h"
#include <QDebug>
#include <vector>

namespace {

constexpr uint32_t kChannels{ 2 };
constexpr uint32_t kSampleRate{ 44100 };
constexpr snd_pcm_format_t kFormat{ SND_PCM_FORMAT_FLOAT };

} // namespace

AudioRecorder::~AudioRecorder()
{
    stop();
}

void AudioRecorder::setDevice(const std::string& device)
{
    device_ = device;
}

void AudioRecorder::setBufferSize(size_t size)
{
    bufFrames_ = size;
}

size_t AudioRecorder::getBufferSize() const
{
    return bufFrames_;
}

void AudioRecorder::start()
{
    if (isRecording_)
        return;

    int err{};
    if ((err = snd_pcm_open (&handle_, device_.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        qDebug() << "Cannot open audio device " << device_ << snd_strerror(err);
        return;
    }

    if ((err = snd_pcm_set_params(handle_, kFormat,  SND_PCM_ACCESS_RW_INTERLEAVED, kChannels, kSampleRate, 1, 50000)) < 0) {   /* 50 ms */
        qDebug() << "Capture open error: " << snd_strerror(err);
        return;
    }

    isRecording_ = true;
}

std::vector<double> AudioRecorder::data()
{
    std::vector<float> buffer(bufFrames_ * kChannels);
    int err = snd_pcm_readi(handle_, buffer.data(), bufFrames_);
    if (err < 0) {
        qDebug() << "read from audio interface failed" << snd_strerror(err);
        return {};
    }

    std::vector<double> data(buffer.size());
    std::ranges::transform(buffer, data.begin(), [](const auto& elem){
        return static_cast<double>(elem);
    });

    return data;
}

void AudioRecorder::stop()
{
    if (!isRecording_)
        return;

    snd_pcm_close(handle_);
    isRecording_ = false;
}
