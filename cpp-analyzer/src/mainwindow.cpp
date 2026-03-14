#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QMediaDevices>
#include <QAudioDevice>
#include <algorithm>

// #include "utils.h"

using namespace std::chrono_literals;

namespace {

constexpr auto kFPS = 60;
constexpr auto kPlotUpdateInterval = 1s / kFPS;

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , plotTimer_(this)
{
    ui->setupUi(this);

    init();

    amplitudePlot_ = std::make_unique<AmplitudePlot>(ui->amplitudePlot);
    frequencyPlot_ = std::make_unique<FrequencyPlot>(ui->frequencyPlot);

    connect(ui->buttonStart, &QPushButton::clicked, this, &MainWindow::onStartSlot);
    connect(ui->buttonStop, &QPushButton::clicked, this, &MainWindow::onStopSlot);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::onDeviceChangedSlot);
    connect(&recorder_, &AudioRecorder::readyRead, this, &MainWindow::onDataSlot);

    connect(&plotTimer_, &QTimer::timeout, this, [this](){
        amplitudePlot_->update();
        frequencyPlot_->update();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
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

void MainWindow::onStartSlot()
{
    if (isRunning_)
        return;

    recorder_.start();
    player_.start();

    while (isRunning_)
    {
      // std::vector<float> data = recorder_.getData();
      // {
      //     std::lock_guard<std::mutex> lock(m_plotMutex);
      //     frequencyPlot_.addData(data);
      //     amplitudePlot_.addData(data);
      // }
      // player_.playSound(data);
    }

    plotTimer_.start(kPlotUpdateInterval);

    isRunning_ = true;
}

void MainWindow::onStopSlot()
{
    if (!isRunning_)
        return;

    recorder_.stop();
    player_.stop();
    plotTimer_.stop();

    isRunning_ = false;
}

void MainWindow::onDeviceChangedSlot(const QString& device)
{
    recorder_.setDevice(device);
}

void MainWindow::onDataSlot()
{
    auto data = recorder_.data();

    frequencyPlot_->addData(data);
    amplitudePlot_->addData(data);
    player_.playSound(data);
}


