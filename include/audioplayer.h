#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QByteArray>
#include <QAudioSink>
#include <QBuffer>
#include <span>

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayer(QObject* parent = nullptr) noexcept;
    virtual ~AudioPlayer() = default;

    void setDevice(const std::string& device);
    void startPlaying();
    void stopPlaying();
    void playSound(const std::span<float>& data);

private:
    std::unique_ptr<QAudioSink> m_sink;
    QBuffer m_buffer;
    QAudioDevice m_device;
    QAudioFormat m_format;
    bool m_isPlaying;
};

#endif // AUDIOPLAYER_H
