#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioSource>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QIODevice>
#include <QMessageBox>
#include <QTimer>

namespace {

using namespace std::chrono_literals;

constexpr auto kFPS{ 60 };
constexpr auto kPlotUpdateInterval{ 1s / kFPS };

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plotTimer(this)
    , m_isRunning(false)
{
    ui->setupUi(this);

    m_frequencyPlot.initialize(ui->frequencyPlot);
    m_amplitudePlot.initialize(ui->amplitudePlot);

    connect(&m_plotTimer, &QTimer::timeout, this, [this](){
        std::lock_guard<std::mutex> lock(m_plotMutex);
        m_amplitudePlot.updatePlot();
        m_frequencyPlot.updatePlot();
    });
}

MainWindow::~MainWindow()
{
    stopRecording();
    delete ui;
}

void MainWindow::startRecording()
{
    if (m_isRunning)
        return;

    m_isRunning = true;
    m_soundThread.reset(new std::jthread([this](){
        m_recorder.start();
        m_player.start();

        while (m_isRunning)
        {
            std::vector<float> data = m_recorder.getData();
            {
                std::lock_guard<std::mutex> lock(m_plotMutex);
                m_frequencyPlot.addData(data);
                m_amplitudePlot.addData(data);
            }
            m_player.playSound(data);
        }

        m_recorder.stop();
        m_player.stop();
    }));

    m_plotTimer.start(kPlotUpdateInterval);
}

void MainWindow::stopRecording()
{
    if (!m_isRunning)
        return;

    m_isRunning = false;
    m_soundThread.reset();
    m_plotTimer.stop();
}

void MainWindow::on_buttonStart_clicked()
{
    startRecording();
}

void MainWindow::on_buttonStop_clicked()
{
    stopRecording();
}

