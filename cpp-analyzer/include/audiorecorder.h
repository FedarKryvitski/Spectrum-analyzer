#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QString>
#include <vector>
#include <memory>

QT_FORWARD_DECLARE_CLASS(QAudioSource)
QT_FORWARD_DECLARE_CLASS(QAudioDevice)
QT_FORWARD_DECLARE_CLASS(QIODevice)

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    explicit AudioRecorder(QObject *parent = nullptr) noexcept;
    ~AudioRecorder() override;

    void start();
    void stop();

    void setDevice(const QString &device);

    std::vector<double> data();

signals:
    void readyRead();

private:
    void reset();

private:
    std::unique_ptr<QAudioSource> audioSource_{ nullptr };
    QIODevice *IODevice_{ nullptr };

    QString deviceName_;
    bool isRecording_{ false };
};

#endif // AUDIORECORDER_H

