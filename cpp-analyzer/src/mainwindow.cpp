#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

// TODO delete
#include <algorithm>

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
    static const QString defaultDeviceName{ "default" };
    static const QStringList deviceNames{ defaultDeviceName };

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

    workerThread_ = std::jthread{[this](){
        audioRecorder_->start();
        audioPlayer_->start();

        isRunning_ = true;

        while (isRunning_)
        {
            auto data = audioRecorder_->read();
            audioPlayer_->write(data.data(), data.size());

            QVector<double> vec;
            std::ranges::transform(data, std::back_inserter(vec), [](const auto& elem){
                return static_cast<double>(elem) / std::numeric_limits<int16_t>::max();
            });

            {
                std::lock_guard lock(mutex_);
                frequencyPlot_->addData(vec);
                amplitudePlot_->addData(vec);
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
