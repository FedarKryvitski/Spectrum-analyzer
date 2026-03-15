#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "alsa/audioplayer.h"
#include "alsa/audiorecorder.h"
#include "widgets/amplitudeplot.h"
#include "widgets/frequencyplot.h"

#include <QMainWindow>

#include <mutex>
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui
{
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
    void onRecordingButtonToggledSlot(const bool checked);

    void onDeviceChangedSlot(const QString &device);

  private:
    void init();

    void startRecording();

    void stopRecording();

  private:
    Ui::MainWindow *ui;

    std::unique_ptr<Alsa::AudioRecorder> audioRecorder_{nullptr};
    std::unique_ptr<Alsa::AudioPlayer> audioPlayer_{nullptr};

    std::unique_ptr<Plot::FrequencyPlot> frequencyPlot_{nullptr};
    std::unique_ptr<Plot::AmplitudePlot> amplitudePlot_{nullptr};

    QTimer *plotTimer_;

    std::jthread workerThread_;
    std::mutex mutex_;
    std::atomic_bool isRunning_{false};
};

#endif // MAINWINDOW_H
