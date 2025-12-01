#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audiorecorder.h"
#include "audioplayer.h"
#include "frequencyplot.h"
#include "amplitudeplot.h"

#include <QMainWindow>
#include <QTimer>
#include <thread>

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
    void startRecording();
    void stopRecording();

private:
    Ui::MainWindow *ui;
    AudioRecorder recorder_;
    AudioPlayer player_;
    FrequencyPlot frequencyPlot_;
    AmplitudePlot amplitudePlot_;

    std::unique_ptr<std::jthread> soundThread_;
    std::atomic<bool> isRunning_;
    std::mutex plotMutex_;
    QTimer plotTimer_;
};

#endif // MAINWINDOW_H
