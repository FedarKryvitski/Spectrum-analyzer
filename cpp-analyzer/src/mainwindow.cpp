#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QMediaDevices>
#include <QAudioDevice>
#include <algorithm>

namespace {

constexpr auto kFps{ 60.f };
constexpr auto kUpdateIntervalMsec{ 1000 / kFps };

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , plotTimer_(this)
{
    ui->setupUi(this);

    init();

    connect(&plotTimer_, &QTimer::timeout, this, [this](){
        amplitudePlot_.update();
        frequencyPlot_.update();
    });

    connect(&recorder_, &AudioRecorder::readyRead, this, &MainWindow::onReadyRead);
}

MainWindow::~MainWindow()
{
    stopRecording();

    delete ui;
}

void MainWindow::init()
{
    frequencyPlot_.init(ui->frequencyPlot);
    amplitudePlot_.init(ui->amplitudePlot);

    QStringList deviceNames{};
    auto devices = QMediaDevices::audioInputs();

    std::ranges::transform(devices, std::back_inserter(deviceNames), [](const QAudioDevice& device){
        return device.description();
    });

    ui->comboBox->addItems(deviceNames);

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioInput();
    QString defaultDeviceName = defaultDevice.description();

    recorder_.setDevice(defaultDeviceName);
}

void MainWindow::startRecording()
{
    if (isRunning_)
        return;

    recorder_.start();
    player_.start();
    plotTimer_.start(kUpdateIntervalMsec);

    isRunning_ = true;
}

void MainWindow::stopRecording()
{
    if (!isRunning_)
        return;

    recorder_.stop();
    player_.stop();
    plotTimer_.stop();

    isRunning_ = false;
}

void MainWindow::onReadyRead()
{
    auto data = recorder_.data();

    frequencyPlot_.addData(data);
    amplitudePlot_.addData(data);
    player_.playSound(data);
}

void MainWindow::on_buttonStart_clicked()
{
    startRecording();
}

void MainWindow::on_buttonStop_clicked()
{
    stopRecording();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    auto device = ui->comboBox->currentText();
    recorder_.setDevice(device);
}

