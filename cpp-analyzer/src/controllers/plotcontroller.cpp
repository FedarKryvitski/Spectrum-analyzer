#include "plotcontroller.h"

#include "widgets/amplitudeplot.h"
#include "widgets/frequencyplot.h"

namespace
{
using namespace std::chrono_literals;

constexpr auto kFPS = 60;
constexpr auto kPlotUpdateInterval = 1000ms / kFPS;
}

namespace Plot
{

PlotController::PlotController(QCustomPlot *inputAmplitudeWidget, QCustomPlot *inputFrequencyWidget,
                               QCustomPlot *outputAmplitudeWidget, QCustomPlot *outputFrequencyWidget, QObject *parent)
    : QObject(parent)
{
    inputAmplitudePlot_ = std::make_unique<AmplitudePlot>(inputAmplitudeWidget);
    inputFrequencyPlot_ = std::make_unique<FrequencyPlot>(inputFrequencyWidget);
    outputAmplitudePlot_ = std::make_unique<AmplitudePlot>(outputAmplitudeWidget);
    outputFrequencyPlot_ = std::make_unique<FrequencyPlot>(outputFrequencyWidget);

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
    auto plots = { inputAmplitudePlot_.get(), outputAmplitudePlot_.get(),
                  inputFrequencyPlot_.get(), outputFrequencyPlot_.get() };

    std::ranges::for_each(plots, [](auto* plot) {
        plot->clear();
        plot->update();
    });
}

void PlotController::onFrameReady(std::vector<double> input, std::vector<double> output)
{
    latestFrame_ = PlotFrame{std::move(input), std::move(output)};
}

void PlotController::updatePlots()
{
    if (!latestFrame_)
        return;

    PlotFrame frame = std::move(*latestFrame_);
    latestFrame_.reset();

    inputAmplitudePlot_->addData(frame.input);
    outputAmplitudePlot_->addData(frame.output);
    inputFrequencyPlot_->addData(frame.input);
    outputFrequencyPlot_->addData(frame.output);

    inputAmplitudePlot_->update();
    outputAmplitudePlot_->update();
    inputFrequencyPlot_->update();
    outputFrequencyPlot_->update();
}

} // namespace Plot
