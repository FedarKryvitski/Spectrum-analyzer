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

class AnalyzerForm : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyzerForm(QWidget *parent = nullptr);
    ~AnalyzerForm();

signals:
    void openListRequested();

private slots:
    void onRecordingButtonToggledSlot(bool checked);
    void onDeviceChangedSlot(const QString &device);
    void onInputTypeButtonSlot(bool checked);
    void onFileDialogButtonSlot();
    void onVolumeChangedSlot(double volume);
    void onStreamFinishedSlot();

private:
    void connectUi();
    void connectAudio();
    void init();
    void updateControlsState(bool isRecording);
    AudioSessionConfig buildSessionConfig() const;
    bool validateConfig(const AudioSessionConfig& config);

private:
    Ui::AnalyzerForm *ui;
    std::unique_ptr<AudioStreamManager> audioStreamManager_;
    std::unique_ptr<Plot::PlotController> plotController_;

    InputType inputType_;
    QString selectedDevice_;
    QString selectedFilePath_;
};
