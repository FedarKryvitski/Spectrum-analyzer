#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QAudioSource>
#include <QAudioDevice>
#include <QBuffer>
#include <QByteArray>
#include <QObject>

#include "iobuffer.h"

class AudioRecorder : public QObject
{
    Q_OBJECT

public:
    explicit AudioRecorder(QObject* parent = nullptr) noexcept;

    void setDevice(const std::string& device);
    void startRecording();
    void stopRecording();
    std::vector<float> getData();

signals:
    void dataReceived();

public slots:
    void onDataReceived();

private:
    std::unique_ptr<QAudioSource> m_source;
    QAudioFormat m_format;
    QAudioDevice m_device;
    IOBuffer m_buffer;
    bool m_isRecording;
};

#endif // AUDIORECORDER_H

