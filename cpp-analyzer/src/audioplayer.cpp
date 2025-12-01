#include "audioplayer.h"

#include <QDebug>
#include <vector>

namespace {

constexpr size_t kChannels{ 2 };
constexpr size_t kSampleRate{ 44100 };
constexpr snd_pcm_format_t kFormat{ SND_PCM_FORMAT_FLOAT };

} // namespace

AudioPlayer::~AudioPlayer()
{
    stop();
}

void AudioPlayer::setDevice(const std::string& device)
{
    device_ = device;
}

void AudioPlayer::start()
{
    if (isPlaying_)
        return;

    int err = snd_pcm_open (&handle_, device_.c_str(),
                           SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        qDebug() << "cannot open audio device " << device_ << snd_strerror(err);
        return;
    }

    err = snd_pcm_set_params(handle_, kFormat, SND_PCM_ACCESS_RW_INTERLEAVED,
                             kChannels, kSampleRate, 1, 50000);
    if (err < 0) {
        qDebug() << "Playback open error: " << snd_strerror(err);
        return;
    }

    snd_pcm_prepare(handle_);
    isPlaying_ = true;
}

void AudioPlayer::stop()
{
    if (!isPlaying_)
        return;

    snd_pcm_close(handle_);
    isPlaying_ = false;
}

void AudioPlayer::playSound(std::span<const double> data)
{
    std::vector<float> buffer(data.size());
    std::ranges::transform(data, buffer.begin(), [](const auto& elem){
        return static_cast<float>(elem);
    });

    int err = snd_pcm_writei(handle_, buffer.data(), buffer.size() / kChannels);
    if (err < 0) {
        if (err == -EPIPE) {
            qDebug() << "XRUN (переполнение буфера), восстанавливаем...";
            snd_pcm_prepare(handle_);
        } else {
            qDebug() << "write to audio interface failed" << snd_strerror(err);
        }
    }
}
