#include "plotcontroller.h"

namespace
{
using namespace std::chrono_literals;

constexpr auto kFPS = 60;
constexpr auto kPlotUpdateInterval = 1s / kFPS;
} // namespace

namespace Plot
{

PlotController::PlotController(QCustomPlot *inputAmplitudeWidget, QCustomPlot *inputFrequencyWidget,
                               QCustomPlot *outputAmplitudeWidget, QCustomPlot *outputFrequencyWidget, QObject *parent)
    : QObject(parent)
{
    amplitudePlot_ = std::make_unique<AmplitudePlot>(inputAmplitudeWidget);
    frequencyPlot_ = std::make_unique<FrequencyPlot>(inputFrequencyWidget);
    amplitudePlot2_ = std::make_unique<AmplitudePlot>(outputAmplitudeWidget);
    frequencyPlot2_ = std::make_unique<FrequencyPlot>(outputFrequencyWidget);

    connect(&plotTimer_, &QTimer::timeout, this, &PlotController::updatePlots);
}

void PlotController::start()
{
    latestFrame_.reset();
    clear();
    plotTimer_.start(kPlotUpdateInterval);
}

void PlotController::stop()
{
    plotTimer_.stop();
    latestFrame_.reset();
    clear();
}

void PlotController::clear()
{
    amplitudePlot_->clear();
    amplitudePlot2_->clear();
    frequencyPlot_->clear();
    frequencyPlot2_->clear();

    amplitudePlot_->update();
    amplitudePlot2_->update();
    frequencyPlot_->update();
    frequencyPlot2_->update();
}

void PlotController::onFrameReady(const std::vector<double> &input, const std::vector<double> &output)
{
    latestFrame_ = PlotFrame{input, output};
}

void PlotController::updatePlots()
{
    if (!latestFrame_.has_value())
        return;

    amplitudePlot_->addData(latestFrame_->input);
    amplitudePlot2_->addData(latestFrame_->output);
    frequencyPlot_->addData(latestFrame_->input);
    frequencyPlot2_->addData(latestFrame_->output);

    amplitudePlot_->update();
    amplitudePlot2_->update();
    frequencyPlot_->update();
    frequencyPlot2_->update();

    latestFrame_.reset();
}

} // namespace Plot
