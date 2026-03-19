#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "audioconverter.h"
#include "plugins/pipeline.h"

#include <QTimer>

#include <algorithm>

namespace
{

using namespace std::chrono_literals;

constexpr auto kFPS = 60;
constexpr auto kPlotUpdateInterval = 1s / kFPS;

} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    audioPlayer_ = std::make_unique<Alsa::AudioPlayer>();
    audioRecorder_ = std::make_unique<Alsa::AudioRecorder>();
    amplitudePlot_ = std::make_unique<Plot::AmplitudePlot>(ui->amplitudePlot);
    frequencyPlot_ = std::make_unique<Plot::FrequencyPlot>(ui->frequencyPlot);

    connect(ui->recordingButton, &QPushButton::toggled, this, &MainWindow::onRecordingButtonToggledSlot);
    connect(ui->deviceComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onDeviceChangedSlot);

    plotTimer_ = new QTimer(this);
    connect(plotTimer_, &QTimer::timeout, this, [this]() {
        std::lock_guard lock(mutex_);
        amplitudePlot_->update();
        frequencyPlot_->update();
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
}

void MainWindow::onRecordingButtonToggledSlot(const bool checked)
{
    if (checked)
    {
        startRecording();
        ui->recordingButton->setText("Stop");
    }
    else
    {
        stopRecording();
        ui->recordingButton->setText("Start");
    }
}

void MainWindow::startRecording()
{
    if (isRunning_)
        return;

    workerThread_ = std::jthread{[this]() {
        Plugins::Pipeline pipeline;

        audioRecorder_->start();
        audioPlayer_->start();

        isRunning_ = true;

        while (isRunning_)
        {
            auto data = audioRecorder_->read();
            auto convertedData = AudioConverter::toDoubleVector(data);
            convertedData = pipeline.process(std::move(convertedData));
            auto convertedIntData = AudioConverter::toIntVector(convertedData);
            audioPlayer_->write(convertedIntData.data(), convertedIntData.size());

            {
                std::lock_guard lock(mutex_);
                frequencyPlot_->addData(convertedData);
                amplitudePlot_->addData(convertedData);
            }
        }

        audioRecorder_->stop();
        audioPlayer_->stop();
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
    audioRecorder_->setDevice(device.toStdString());

    ui->recordingButton->setEnabled(true);
}
