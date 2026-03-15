#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audioplayer.h"
#include "audiorecorder.h"
#include "widgets/amplitudeplot.h"
#include "widgets/frequencyplot.h"

#include <QMainWindow>

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

    void onDataSlot();

  private:
    void init();

    void startRecording();

    void stopRecording();

  private:
    Ui::MainWindow *ui;

    // TODO make ALSA
    AudioRecorder *audioRecorder_{nullptr};
    std::unique_ptr<Alsa::AudioPlayer> audioPlayer_{nullptr};

    std::unique_ptr<Plot::FrequencyPlot> frequencyPlot_{nullptr};
    std::unique_ptr<Plot::AmplitudePlot> amplitudePlot_{nullptr};

    QTimer *plotTimer_;

    bool isRunning_{false};
};

#endif // MAINWINDOW_H
