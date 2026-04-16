#include "analyzerform.h"
#include "pluginslistform.h"
#include "ui_analyzerform.h"

#include "controllers/plotcontroller.h"
#include "controllers/plugincontroller.h"
#include "manager/audiostreammanager.h"

#include <QAbstractItemView>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSignalBlocker>

namespace
{
constexpr auto kDefaultDeviceName = "default";
}

AnalyzerForm::AnalyzerForm(QWidget *parent)
    : QWidget(parent), ui(new Ui::AnalyzerForm), inputType_(InputType::Microphone)
{
    ui->setupUi(this);

    pluginController_ = std::make_unique<PluginController>(this);

    audioStreamManager_ = std::make_unique<AudioStreamManager>(this);

    pluginController_ = std::make_unique<PluginController>(this);

    plotController_ = std::make_unique<Plot::PlotController>(ui->inputAmplitudePlot, ui->inputFrequencyPlot,
                                                             ui->outputAmplitudePlot, ui->outputFrequencyPlot, this);

    pluginsListForm_ = new PluginsListForm(this);
    pluginsListForm_->setController(pluginController_.get());

    connect(pluginsListForm_, &PluginsListForm::backRequested, this, &AnalyzerForm::onBackRequestedSlot);
    ui->stackedWidget->addWidget(pluginsListForm_);

    connectUi();
    connectAudio();
    init();
}

AnalyzerForm::~AnalyzerForm()
{
    if (audioStreamManager_)
        audioStreamManager_->stop();

    delete ui;
}

void AnalyzerForm::connectUi()
{
    connect(ui->recordingButton, &QPushButton::toggled, this, &AnalyzerForm::onRecordingButtonToggledSlot);
    connect(ui->deviceComboBox, &QComboBox::currentTextChanged, this, &AnalyzerForm::onDeviceChangedSlot);
    connect(ui->microphoneRadioButton, &QRadioButton::toggled, this, &AnalyzerForm::onInputTypeButtonSlot);
    connect(ui->selectFileButton, &QPushButton::clicked, this, &AnalyzerForm::onFileDialogButtonSlot);
    connect(ui->backToWelcomeButton, &QPushButton::clicked, this, &AnalyzerForm::onExitPressedSlot);
    connect(ui->openListPageButton, &QPushButton::clicked, this, &AnalyzerForm::onOpenPluginsButtonSlot);
}

void AnalyzerForm::connectAudio()
{
    connect(audioStreamManager_.get(), &AudioStreamManager::inputVolumeChanged, this,
            &AnalyzerForm::onInputVolumeChangedSlot);
    connect(audioStreamManager_.get(), &AudioStreamManager::outputVolumeChanged, this,
            &AnalyzerForm::onOutputVolumeChangedSlot);

    connect(audioStreamManager_.get(), &AudioStreamManager::frameReady, plotController_.get(),
            &Plot::PlotController::onFrameReady, Qt::QueuedConnection);

    connect(audioStreamManager_.get(), &AudioStreamManager::errorOccurred, this, [this](const QString &message) {
        QMessageBox::warning(this, "Error", message);

        {
            QSignalBlocker blocker(ui->recordingButton);
            ui->recordingButton->setChecked(false);
        }

        ui->recordingButton->setText("Start");
        updateControlsState(false);
        plotController_->stop();
    });

    connect(audioStreamManager_.get(), &AudioStreamManager::finished, this, &AnalyzerForm::onStreamFinishedSlot,
            Qt::QueuedConnection);
}

void AnalyzerForm::init()
{
    ui->deviceComboBox->addItem(kDefaultDeviceName);
    selectedDevice_ = kDefaultDeviceName;

    ui->microphoneRadioButton->setChecked(true);

    ui->deviceComboBox->setVisible(true);
    ui->selectFileButton->setVisible(false);
    ui->selectedFileLabel->setVisible(false);

    ui->recordingButton->setEnabled(true);
    ui->recordingButton->setText("Start");
}

AudioSessionConfig AnalyzerForm::buildSessionConfig() const
{
    AudioSessionConfig config;
    config.inputType = inputType_;
    config.source = inputType_ == InputType::Microphone ? selectedDevice_ : selectedFilePath_;
    return config;
}

bool AnalyzerForm::validateConfig(const AudioSessionConfig &config)
{
    if (config.source.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please select input.");
        return false;
    }

    return true;
}

void AnalyzerForm::onRecordingButtonToggledSlot(bool checked)
{
    if (checked)
    {
        const auto config = buildSessionConfig();
        if (!validateConfig(config))
        {
            QSignalBlocker blocker(ui->recordingButton);
            ui->recordingButton->setChecked(false);
            return;
        }

        plotController_->start();
        audioStreamManager_->start(config, pluginController_->getPipeline());

        ui->recordingButton->setText("Stop");
    }
    else
    {
        audioStreamManager_->stop();
        plotController_->stop();

        ui->recordingButton->setText("Start");
    }

    updateControlsState(checked);
}

void AnalyzerForm::onDeviceChangedSlot(const QString &device)
{
    selectedDevice_ = device;

    if (inputType_ == InputType::Microphone)
    {
        ui->recordingButton->setEnabled(!selectedDevice_.isEmpty());
    }
}

void AnalyzerForm::onInputTypeButtonSlot(bool checked)
{
    inputType_ = checked ? InputType::Microphone : InputType::File;

    ui->selectFileButton->setVisible(!checked);
    ui->selectedFileLabel->setVisible(!checked);
    ui->deviceComboBox->setVisible(checked);

    if (inputType_ == InputType::Microphone)
    {
        ui->recordingButton->setEnabled(!selectedDevice_.isEmpty());
    }
    else
    {
        ui->recordingButton->setEnabled(!selectedFilePath_.isEmpty());
    }

    plotController_->clear();
}

void AnalyzerForm::onFileDialogButtonSlot()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select audio file", "",
                                                    "Audio Files (*.wav *.mp3 *.flac *.ogg);;All Files (*.*)");

    if (filePath.isEmpty())
        return;

    selectedFilePath_ = filePath;

    QFileInfo fileInfo(filePath);
    ui->selectedFileLabel->setText(fileInfo.fileName());

    if (inputType_ == InputType::File)
    {
        ui->recordingButton->setEnabled(true);
    }
}

void AnalyzerForm::onInputVolumeChangedSlot(const double volume)
{
    ui->inputVolumeBar->setValue(static_cast<int>(std::round(volume)));
}

void AnalyzerForm::onOutputVolumeChangedSlot(const double volume)
{
    ui->outputVolumeBar->setValue(static_cast<int>(std::round(volume)));
}

void AnalyzerForm::onStreamFinishedSlot()
{
    plotController_->stop();

    if (ui->recordingButton->isChecked())
    {
        QSignalBlocker blocker(ui->recordingButton);
        ui->recordingButton->setChecked(false);
    }

    ui->recordingButton->setText("Start");
    updateControlsState(false);
}

void AnalyzerForm::updateControlsState(bool isRecording)
{
    ui->microphoneRadioButton->setEnabled(!isRecording);
    ui->fileRadioButton->setEnabled(!isRecording);
    ui->deviceComboBox->setEnabled(!isRecording);
    ui->selectFileButton->setEnabled(!isRecording);

    ui->inputVolumeBar->setValue(0);
    ui->outputVolumeBar->setValue(0);
}

void AnalyzerForm::onOpenPluginsButtonSlot()
{
    ui->stackedWidget->setCurrentWidget(pluginsListForm_);
}

void AnalyzerForm::onBackRequestedSlot()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AnalyzerForm::onExitPressedSlot()
{
    audioStreamManager_->stop();
    plotController_->stop();

    emit backRequested();
}
