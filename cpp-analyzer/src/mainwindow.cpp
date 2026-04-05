#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "audioconverter.h"
#include "plugins/pipeline.h"

#include <QTimer>

namespace
{

using namespace std::chrono_literals;

constexpr auto kFPS = 60;
constexpr auto kPlotUpdateInterval = 1s / kFPS;

} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    audioPlayer_ = std::make_unique<Media::AudioPlayer>();
    audioDeviceRecorder_ = std::make_unique<Media::AudioDeviceSource>();
    audioFileRecorder_ = std::make_unique<Media::AudioFileSource>();

    amplitudePlot_ = std::make_unique<Plot::AmplitudePlot>(ui->amplitudePlot);
    frequencyPlot_ = std::make_unique<Plot::FrequencyPlot>(ui->frequencyPlot);
    amplitudePlot2_ = std::make_unique<Plot::AmplitudePlot>(ui->amplitudePlot_2);
    frequencyPlot2_ = std::make_unique<Plot::FrequencyPlot>(ui->frequencyPlot_2);

    connect(ui->recordingButton, &QPushButton::toggled, this, &MainWindow::onRecordingButtonToggledSlot);
    connect(ui->deviceComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onDeviceChangedSlot);
    connect(ui->microphoneRadioButton, &QRadioButton::toggled, this, &MainWindow::onInputTypeButtonSlot);
    connect(ui->selectFileButton, &QPushButton::clicked, this, &MainWindow::onFileDialogButtonSlot);

    ui->selectFileButton->setVisible(false);
    ui->selectedFileLabel->setVisible(false);

    plotTimer_ = new QTimer(this);

    connect(plotTimer_, &QTimer::timeout, this, [this]() {
        std::lock_guard lock(mutex_);
        amplitudePlot_->update();
        amplitudePlot2_->update();
        frequencyPlot_->update();
        frequencyPlot2_->update();
    });

    init();
}

MainWindow::~MainWindow()
{
    stopRecording();

    delete ui;
}

void MainWindow::init()
{
    static const QString defaultDeviceName{"default"};
    static const QStringList deviceNames{defaultDeviceName};

    ui->deviceComboBox->addItems(deviceNames);

    selectedDevice_ = defaultDeviceName;

    ui->microphoneRadioButton->setChecked(true);

    ui->deviceComboBox->setVisible(true);
    ui->selectFileButton->setVisible(false);
    ui->selectedFileLabel->setVisible(false);

    ui->recordingButton->setEnabled(true);
}

void MainWindow::onRecordingButtonToggledSlot(const bool checked)
{
    if (checked)
    {
        if (inputType_ == InputType::kMicrophone && selectedDevice_.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please select input device.");
            ui->recordingButton->setChecked(false);
            return;
        }

        if (inputType_ == InputType::kFile && selectedFilePath_.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please select audio file.");
            ui->recordingButton->setChecked(false);
            return;
        }

        startRecording();
        ui->recordingButton->setText("Stop");
    }
    else
    {
        stopRecording();
        ui->recordingButton->setText("Start");

        std::lock_guard lock(mutex_);
        amplitudePlot_->clear();
        amplitudePlot2_->clear();
        frequencyPlot_->clear();
        frequencyPlot2_->clear();
        amplitudePlot_->update();
        amplitudePlot2_->update();
        frequencyPlot_->update();
        frequencyPlot2_->update();
    }

    ui->microphoneRadioButton->setEnabled(!checked);
    ui->fileRadioButton->setEnabled(!checked);
    ui->deviceComboBox->setEnabled(!checked);
    ui->selectFileButton->setEnabled(!checked);
}

void MainWindow::startRecording()
{
    if (isRunning_)
        return;

    workerThread_ = std::jthread{[this]() {
        Plugins::Pipeline pipeline;

        if (inputType_ == InputType::kMicrophone)
        {
            audioDeviceRecorder_->open(selectedDevice_.toStdString());
        }
        else
        {
            audioFileRecorder_->open(selectedFilePath_.toStdString());
        }

        audioPlayer_->start();

        isRunning_ = true;

        while (isRunning_)
        {
            Media::Buffer data;

            if (inputType_ == InputType::kMicrophone)
            {
                data = audioDeviceRecorder_->read();
            }
            else
            {
                data = audioFileRecorder_->read();
            }

            if (data.empty())
                continue;

            auto inputData = AudioConverter::toDoubleVector(data);
            // auto outputData = pipeline.process(inputData);
            // auto outputIntData = AudioConverter::toIntVector(outputData);

            audioPlayer_->write(data.data(), data.size());
            {
                std::lock_guard lock(mutex_);
                amplitudePlot_->addData(inputData);
                amplitudePlot2_->addData(inputData);
                frequencyPlot_->addData(inputData);
                frequencyPlot2_->addData(inputData);
            }
        }

        if (inputType_ == InputType::kMicrophone)
        {
            audioDeviceRecorder_->close();
        }
        else
        {
            audioFileRecorder_->close();
        }

        audioPlayer_->stop();

        isRunning_ = false;
    }};

    plotTimer_->start(kPlotUpdateInterval);
}

void MainWindow::stopRecording()
{
    if (!isRunning_)
        return;

    isRunning_ = false;

    workerThread_ = {};

    plotTimer_->stop();
    amplitudePlot_->clear();
    amplitudePlot_->update();
}

void MainWindow::onDeviceChangedSlot(const QString &device)
{
    selectedDevice_ = device;
    if (inputType_ == InputType::kMicrophone)
    {
        ui->recordingButton->setEnabled(!selectedDevice_.isEmpty());
    }
}

void MainWindow::onInputTypeButtonSlot(const bool checked)
{
    inputType_ = checked ? InputType::kMicrophone : InputType::kFile;

    ui->selectFileButton->setVisible(!checked);
    ui->selectedFileLabel->setVisible(!checked);
    ui->deviceComboBox->setVisible(checked);

    if (inputType_ == InputType::kMicrophone)
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

    if (inputType_ == InputType::kFile)
    {
        ui->recordingButton->setEnabled(true);
    }
}
