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

void AnalyzerForm::setInputDevices(const QStringList &devices)
{
    ui->inputDeviceComboBox->clear();
    ui->inputDeviceComboBox->addItems(devices);
    if (!devices.isEmpty()) {
        selectedInputDevice_ = devices.first();
    }
}

void AnalyzerForm::setOutputDevices(const QStringList &devices)
{
    ui->outputDeviceComboBox->clear();
    ui->outputDeviceComboBox->addItems(devices);
    if (!devices.isEmpty()) {
        selectedOutputDevice_ = devices.first();
    }
}

void AnalyzerForm::connectUi()
{
    connect(ui->recordingButton, &QPushButton::toggled, this, &AnalyzerForm::onRecordingButtonToggledSlot);
    connect(ui->inputDeviceComboBox, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &AnalyzerForm::onInputDeviceChangedSlot);
    connect(ui->outputDeviceComboBox, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &AnalyzerForm::onOutputDeviceChangedSlot);
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
        QMessageBox::warning(this, QStringLiteral("Ошибка"), message);

        {
            QSignalBlocker blocker(ui->recordingButton);
            ui->recordingButton->setChecked(false);
        }

        ui->recordingButton->setText(QStringLiteral("Старт"));
        updateControlsState(false);
        plotController_->stop();
    });

    connect(audioStreamManager_.get(), &AudioStreamManager::finished, this, &AnalyzerForm::onStreamFinishedSlot,
            Qt::QueuedConnection);
}

void AnalyzerForm::init()
{
    ui->inputDeviceComboBox->addItem(kDefaultDeviceName);
    selectedInputDevice_ = kDefaultDeviceName;

    ui->outputDeviceComboBox->addItem(kDefaultDeviceName);
    selectedOutputDevice_ = kDefaultDeviceName;

    ui->microphoneRadioButton->setChecked(true);

    ui->inputDeviceComboBox->setVisible(true);
    ui->outputDeviceComboBox->setVisible(true);
    ui->selectFileButton->setVisible(false);
    ui->selectedFileLabel->setVisible(false);

    ui->recordingButton->setEnabled(true);
    ui->recordingButton->setText(QStringLiteral("Старт"));
}

AudioSessionConfig AnalyzerForm::buildSessionConfig() const
{
    AudioSessionConfig config;
    config.inputType = inputType_;
    config.source = inputType_ == InputType::Microphone ? selectedInputDevice_ : selectedFilePath_;
    return config;
}

bool AnalyzerForm::validateConfig(const AudioSessionConfig &config)
{
    if (config.source.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Пожалуйста, выберите источник ввода."));
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

        ui->recordingButton->setText(QStringLiteral("Стоп"));
    }
    else
    {
        audioStreamManager_->stop();
        plotController_->stop();

        ui->recordingButton->setText(QStringLiteral("Старт"));
    }

    updateControlsState(checked);
}

void AnalyzerForm::onInputDeviceChangedSlot(const QString &device)
{
    selectedInputDevice_ = device;

    if (inputType_ == InputType::Microphone)
    {
        ui->recordingButton->setEnabled(!selectedInputDevice_.isEmpty());
    }
}

void AnalyzerForm::onOutputDeviceChangedSlot(const QString &device)
{
    selectedOutputDevice_ = device;
}

void AnalyzerForm::onInputTypeButtonSlot(bool checked)
{
    inputType_ = checked ? InputType::Microphone : InputType::File;

    ui->selectFileButton->setVisible(!checked);
    ui->selectedFileLabel->setVisible(!checked);
    ui->inputDeviceComboBox->setVisible(checked);

    if (inputType_ == InputType::Microphone)
    {
        ui->recordingButton->setEnabled(!selectedInputDevice_.isEmpty());
    }
    else
    {
        ui->recordingButton->setEnabled(!selectedFilePath_.isEmpty());
    }

    plotController_->clear();
}

void AnalyzerForm::onFileDialogButtonSlot()
{
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("Выберите аудиофайл"), "",
                                                    QStringLiteral("Аудиофайлы (*.wav *.mp3 *.flac *.ogg);;Все файлы (*.*)"));

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

    ui->recordingButton->setText(QStringLiteral("Старт"));
    updateControlsState(false);
}

void AnalyzerForm::updateControlsState(bool isRecording)
{
    ui->microphoneRadioButton->setEnabled(!isRecording);
    ui->fileRadioButton->setEnabled(!isRecording);
    ui->inputDeviceComboBox->setEnabled(!isRecording);
    ui->outputDeviceComboBox->setEnabled(!isRecording);
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
