#pragma once

#include "audiosessionconfig.h"
#include "audiostreammanager.h"
#include "plotcontroller.h"

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
    void onRecordingButtonToggledSlot(bool checked);
    void onDeviceChangedSlot(const QString &device);
    void onInputTypeButtonSlot(bool checked);
    void onFileDialogButtonSlot();

    void onVolumeChangedSlot(double volume);
    void onStreamFinishedSlot();

    void onOpenListPageSlot();
    void onBackToAnalyzerSlot();
    void onAddListItemSlot();
    void onRemoveListItemSlot();

  private:
    void init();
    void connectUi();
    void connectAudio();

    AudioSessionConfig buildSessionConfig() const;
    bool validateConfig(const AudioSessionConfig &config);
    void updateControlsState(bool isRecording);

  private:
    Ui::MainWindow *ui{nullptr};

    std::unique_ptr<AudioStreamManager> audioStreamManager_{nullptr};
    std::unique_ptr<Plot::PlotController> plotController_{nullptr};

    QString selectedFilePath_;
    QString selectedDevice_;
    InputType inputType_{InputType::Microphone};
};
