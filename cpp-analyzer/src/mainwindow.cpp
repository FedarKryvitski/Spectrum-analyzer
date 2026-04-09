#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include <QAbstractItemView>
#include <QInputDialog>
#include <QListWidgetItem>

namespace
{
constexpr auto kDefaultDeviceName = "default";
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    audioStreamManager_ = std::make_unique<AudioStreamManager>(this);

    plotController_ = std::make_unique<Plot::PlotController>(ui->amplitudePlot, ui->frequencyPlot, ui->amplitudePlot_2,
                                                             ui->frequencyPlot_2, this);

    connectUi();
    connectAudio();
    init();
}

MainWindow::~MainWindow()
{
    if (audioStreamManager_)
        audioStreamManager_->stop();

    delete ui;
}

void MainWindow::connectUi()
{
    connect(ui->recordingButton, &QPushButton::toggled, this, &MainWindow::onRecordingButtonToggledSlot);

    connect(ui->deviceComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onDeviceChangedSlot);

    connect(ui->microphoneRadioButton, &QRadioButton::toggled, this, &MainWindow::onInputTypeButtonSlot);

    connect(ui->selectFileButton, &QPushButton::clicked, this, &MainWindow::onFileDialogButtonSlot);

    connect(ui->openListPageButton, &QPushButton::clicked, this, &MainWindow::onOpenListPageSlot);

    connect(ui->backToAnalyzerButton, &QPushButton::clicked, this, &MainWindow::onBackToAnalyzerSlot);

    connect(ui->addItemButton, &QPushButton::clicked, this, &MainWindow::onAddListItemSlot);

    connect(ui->removeItemButton, &QPushButton::clicked, this, &MainWindow::onRemoveListItemSlot);
}

void MainWindow::connectAudio()
{
    connect(audioStreamManager_.get(), &AudioStreamManager::volumeChanged, this, &MainWindow::onVolumeChangedSlot);

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

    connect(audioStreamManager_.get(), &AudioStreamManager::finished, this, &MainWindow::onStreamFinishedSlot,
            Qt::QueuedConnection);
}

void MainWindow::init()
{
    ui->deviceComboBox->addItem(kDefaultDeviceName);
    selectedDevice_ = kDefaultDeviceName;

    ui->microphoneRadioButton->setChecked(true);

    ui->deviceComboBox->setVisible(true);
    ui->selectFileButton->setVisible(false);
    ui->selectedFileLabel->setVisible(false);

    ui->recordingButton->setEnabled(true);
    ui->recordingButton->setText("Start");

    ui->stackedWidget->setCurrentWidget(ui->analyzerPage);

    ui->editableListWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
}

AudioSessionConfig MainWindow::buildSessionConfig() const
{
    AudioSessionConfig config;
    config.inputType = inputType_;
    config.source = inputType_ == InputType::Microphone ? selectedDevice_ : selectedFilePath_;
    return config;
}

bool MainWindow::validateConfig(const AudioSessionConfig &config)
{
    if (config.source.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please select input.");
        return false;
    }

    return true;
}

void MainWindow::onRecordingButtonToggledSlot(bool checked)
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
        audioStreamManager_->start(config);

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

void MainWindow::onDeviceChangedSlot(const QString &device)
{
    selectedDevice_ = device;

    if (inputType_ == InputType::Microphone)
    {
        ui->recordingButton->setEnabled(!selectedDevice_.isEmpty());
    }
}

void MainWindow::onInputTypeButtonSlot(bool checked)
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
}

void MainWindow::onFileDialogButtonSlot()
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

void MainWindow::onVolumeChangedSlot(double volume)
{
    ui->progressBar->setValue(static_cast<int>(volume));
}

void MainWindow::onStreamFinishedSlot()
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

void MainWindow::updateControlsState(bool isRecording)
{
    ui->microphoneRadioButton->setEnabled(!isRecording);
    ui->fileRadioButton->setEnabled(!isRecording);
    ui->deviceComboBox->setEnabled(!isRecording);
    ui->selectFileButton->setEnabled(!isRecording);

    ui->progressBar->setValue(0);
}

void MainWindow::onOpenListPageSlot()
{
    ui->stackedWidget->setCurrentWidget(ui->listPage);
}

void MainWindow::onBackToAnalyzerSlot()
{
    ui->stackedWidget->setCurrentWidget(ui->analyzerPage);
}

void MainWindow::onAddListItemSlot()
{
    bool ok = false;

    const QString text = QInputDialog::getText(this, "Add item", "Item name:", QLineEdit::Normal, "", &ok);

    if (ok && !text.trimmed().isEmpty())
    {
        auto *item = new QListWidgetItem(text.trimmed());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->editableListWidget->addItem(item);
    }
}

void MainWindow::onRemoveListItemSlot()
{
    auto *item = ui->editableListWidget->currentItem();

    if (!item)
    {
        QMessageBox::information(this, "Info", "Select an item first.");
        return;
    }

    delete item;
}
