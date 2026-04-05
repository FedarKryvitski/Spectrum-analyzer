#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "media/audioplayer.h"
#include "media/audiorecorder.h"
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
    enum class InputType
    {
        kFile,
        kMicrophone
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void onRecordingButtonToggledSlot(const bool checked);

    void onDeviceChangedSlot(const QString &device);

    void onInputTypeButtonSlot(const bool checked);

    void onFileDialogButtonSlot();

  private:
    void init();

    void startRecording();

    void stopRecording();

  private:
    Ui::MainWindow *ui;

    std::unique_ptr<Media::AudioRecorder> audioRecorder_{nullptr};
    std::unique_ptr<Media::AudioPlayer> audioPlayer_{nullptr};

    std::unique_ptr<Plot::FrequencyPlot> frequencyPlot_{nullptr};
    std::unique_ptr<Plot::AmplitudePlot> amplitudePlot_{nullptr};
    std::unique_ptr<Plot::FrequencyPlot> frequencyPlot2_{nullptr};
    std::unique_ptr<Plot::AmplitudePlot> amplitudePlot2_{nullptr};

    QTimer *plotTimer_;

    std::jthread workerThread_;
    std::mutex mutex_;
    std::atomic_bool isRunning_{false};

    QString selectedFilePath_;
    QString selectedDevice_;

    InputType inputType_{InputType::kMicrophone};
};

#endif // MAINWINDOW_H
