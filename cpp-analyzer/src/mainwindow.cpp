#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

namespace {

using namespace std::chrono_literals;

constexpr auto kFps{ 60.f };
constexpr auto kUpdateIntervalMsec{ 1000 / kFps };

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , plotTimer_(this)
    , isRunning_(false)
{
    ui->setupUi(this);

    frequencyPlot_.initialize(ui->frequencyPlot);
    amplitudePlot_.initialize(ui->amplitudePlot);

    connect(&plotTimer_, &QTimer::timeout, this, [this](){
        std::lock_guard lock(plotMutex_);
        amplitudePlot_.update();
        frequencyPlot_.update();
    });
}

MainWindow::~MainWindow()
{
    stopRecording();
    delete ui;
}

void MainWindow::startRecording()
{
    if (isRunning_)
        return;

    isRunning_ = true;

    soundThread_.reset(new std::jthread([this](){
        recorder_.start();
        player_.start();

        while (isRunning_)
        {
            auto data = recorder_.data();
            {
                std::lock_guard lock(plotMutex_);
                frequencyPlot_.addData(data);
                amplitudePlot_.addData(data);
            }
            player_.playSound(data);
        }

        recorder_.stop();
        player_.stop();
    }));

    plotTimer_.start(kUpdateIntervalMsec);
}

void MainWindow::stopRecording()
{
    if (!isRunning_)
        return;

    isRunning_ = false;
    soundThread_.reset();
    plotTimer_.stop();
}

void MainWindow::on_buttonStart_clicked()
{
    startRecording();
}

void MainWindow::on_buttonStop_clicked()
{
    stopRecording();
}

