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

constexpr uint32_t PLOT_UPDATE_MS = 1000 / 60;

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
            std::vector<float> data2 = m_recorder.getData(256);
            std::vector<float> data(256);
            static int c = 0;
            for (int i = 0; i < 256; ++i){
                data[i] = 0.3 * std::sin(c++ * 440.0 * 2 * M_PI / 44100);
            }
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

    m_plotTimer.start(PLOT_UPDATE_MS);
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

