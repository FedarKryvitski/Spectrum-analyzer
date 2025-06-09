#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioSink>
#include <QAudioSource>
#include <QTimer>
#include "audiorecorder.h"
#include "audioplayer.h"
#include "frequencyplot.h"
#include "amplitudeplot.h"
#include "volumeprocessor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBytesRead();
    void on_buttonStart_clicked();
    void on_buttonStop_clicked();
    void onVolumeChange(int value);

private:
    Ui::MainWindow *ui;
    AudioRecorder m_recorder;
    AudioPlayer m_player;
    VolumeProcessor m_volume;
    FrequencyPlot m_frequencyPlot;
    AmplitudePlot m_amplitudePlot;

    QTimer m_amplitudeTimer, m_frequencyTimer;
    bool m_isRunning;

private:
    void startRecording();
    void stopRecording();
};

#endif // MAINWINDOW_H
