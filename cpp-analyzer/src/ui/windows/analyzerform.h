#pragma once

#include <QWidget>

#include <memory>

class AudioStreamManager;

namespace Plot
{
class PlotController;
}

namespace Ui
{
class AnalyzerForm;
}

enum class InputType;
struct AudioSessionConfig;
class PluginsListForm;
class PluginController;

class AnalyzerForm : public QWidget
{
    Q_OBJECT

  public:
    explicit AnalyzerForm(QWidget *parent = nullptr);
    ~AnalyzerForm();

  signals:
    void backRequested();

  private slots:
    void onRecordingButtonToggledSlot(bool checked);
    void onDeviceChangedSlot(const QString &device);
    void onInputTypeButtonSlot(bool checked);
    void onFileDialogButtonSlot();
    void onInputVolumeChangedSlot(double volume);
    void onOutputVolumeChangedSlot(double volume);
    void onStreamFinishedSlot();
    void onOpenPluginsButtonSlot();

    void onExitPressedSlot();
    void onBackRequestedSlot();

  private:
    void connectUi();
    void connectAudio();
    void init();
    void updateControlsState(bool isRecording);
    AudioSessionConfig buildSessionConfig() const;
    bool validateConfig(const AudioSessionConfig &config);

  private:
    Ui::AnalyzerForm *ui;
    PluginsListForm *pluginsListForm_;

    std::unique_ptr<PluginController> pluginController_;
    std::unique_ptr<AudioStreamManager> audioStreamManager_;
    std::unique_ptr<Plot::PlotController> plotController_;

    InputType inputType_;
    QString selectedDevice_;
    QString selectedFilePath_;
};
