#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAudioSource>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QIODevice>
#include <QMessageBox>
#include <QTimer>
#include <span>

using namespace std::chrono_literals;

namespace {

constexpr static uint32_t AMPLITUDE_PLOT_UPDATE_MS = 1000 / 70;
constexpr static uint32_t FREQUENCY_PLOT_UPDATE_MS = 1000 / 10;

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_amplitudeTimer(this)
    , m_frequencyTimer(this)
    , m_isRunning(false)
{
    ui->setupUi(this);

    m_frequencyPlot.initialize(ui->frequencyPlot);
    m_amplitudePlot.initialize(ui->amplitudePlot);

    connect(&m_amplitudeTimer, &QTimer::timeout, this, [this](){
        m_amplitudePlot.updatePlot();
    });

    connect(&m_frequencyTimer, &QTimer::timeout, this, [this](){
        m_frequencyPlot.updatePlot();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBytesRead()
{
    std::vector<float> data = m_recorder.getData();

    m_volume.processAudio(data);
    m_frequencyPlot.addData(data);
    m_amplitudePlot.addData(data);
    m_player.playSound(data);
}

void MainWindow::startRecording()
{
    if (m_isRunning)
        return;

    connect(&m_recorder, &AudioRecorder::dataReceived, this, &MainWindow::onBytesRead);

    m_recorder.startRecording();
    m_player.startPlaying();
    m_amplitudeTimer.start(AMPLITUDE_PLOT_UPDATE_MS);
    m_frequencyTimer.start(FREQUENCY_PLOT_UPDATE_MS);

    m_isRunning = true;
}

void MainWindow::stopRecording()
{
    if (!m_isRunning)
        return;

    disconnect(&m_recorder, &AudioRecorder::dataReceived, this, &MainWindow::onBytesRead);

    m_recorder.stopRecording();
    m_player.stopPlaying();
    m_amplitudeTimer.stop();
    m_frequencyTimer.stop();

    m_isRunning = false;
}

void MainWindow::onVolumeChange(int value)
{
    float sliderValue = ui->sliderVolume->value() / 100.f;
    m_volume.setVolume(sliderValue);
}

void MainWindow::on_buttonStart_clicked()
{
    if (m_isRunning)
        stopRecording();

    startRecording();
}

void MainWindow::on_buttonStop_clicked()
{
    stopRecording();
}

