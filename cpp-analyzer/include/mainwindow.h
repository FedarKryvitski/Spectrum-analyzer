#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioSink>
#include <QAudioSource>
#include <QTimer>
#include <thread>
#include "audiorecorder.h"
#include "audioplayer.h"
#include "frequencyplot.h"
#include "amplitudeplot.h"

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
    void on_buttonStart_clicked();
    void on_buttonStop_clicked();

private:
    Ui::MainWindow *ui;
    AudioRecorder m_recorder;
    AudioPlayer m_player;
    FrequencyPlot m_frequencyPlot;
    AmplitudePlot m_amplitudePlot;

    std::unique_ptr<std::jthread> m_soundThread;
    std::atomic<bool> m_isRunning;
    std::mutex m_plotMutex;
    QTimer m_plotTimer;

private:
    void startRecording();
    void stopRecording();
};

#endif // MAINWINDOW_H
