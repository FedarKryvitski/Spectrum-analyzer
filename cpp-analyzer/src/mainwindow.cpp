#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioDevice>
#include <QMediaDevices>
#include <QTimer>

#include <algorithm>

// #include "utils.h"

using namespace std::chrono_literals;

namespace
{

constexpr auto kFPS = 60;
constexpr auto kPlotUpdateInterval = 1s / kFPS;

} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    audioPlayer_ = std::make_unique<Alsa::AudioPlayer>();
    amplitudePlot_ = std::make_unique<Plot::AmplitudePlot>(ui->amplitudePlot);
    frequencyPlot_ = std::make_unique<Plot::FrequencyPlot>(ui->frequencyPlot);

    connect(ui->recordingButton, &QPushButton::toggled, this, &MainWindow::onRecordingButtonToggledSlot);
    connect(ui->deviceComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onDeviceChangedSlot);

    audioRecorder_ = new AudioRecorder(this);
    connect(audioRecorder_, &AudioRecorder::readyRead, this, &MainWindow::onDataSlot);

    plotTimer_ = new QTimer(this);
    connect(plotTimer_, &QTimer::timeout, this, [this]() {
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
    QStringList deviceNames{};
    auto devices = QMediaDevices::audioInputs();

    std::ranges::transform(devices, std::back_inserter(deviceNames),
                           [](const QAudioDevice &device) { return device.description(); });

    ui->deviceComboBox->addItems(deviceNames);

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioInput();
    QString defaultDeviceName = defaultDevice.description();

    audioRecorder_->setDevice(defaultDeviceName);
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

    audioRecorder_->start();
    audioPlayer_->start();
    plotTimer_->start(kPlotUpdateInterval);

    isRunning_ = true;
}

void MainWindow::stopRecording()
{
    if (!isRunning_)
        return;

    audioRecorder_->stop();
    audioPlayer_->stop();
    plotTimer_->stop();

    amplitudePlot_->clear();
    amplitudePlot_->update();

    isRunning_ = false;
}

void MainWindow::onDeviceChangedSlot(const QString &device)
{
    audioRecorder_->setDevice(device);
}

void MainWindow::onDataSlot()
{
    auto data = audioRecorder_->data();

    frequencyPlot_->addData(data);
    amplitudePlot_->addData(data);
    audioPlayer_->playSound(data);
}
