#include "widgets/amplitudeplot.h"

#include <algorithm>

namespace {

using namespace std::chrono_literals;

constexpr static int kSampleRate{ 44100 };
constexpr static double kPlotDurationSec{ 3.f };
constexpr static size_t kSampleStep{ 10 };
constexpr static size_t kBufferSize = kPlotDurationSec * kSampleRate / kSampleStep;

} // namespace

AmplitudePlot::AmplitudePlot(QCustomPlot* parent) noexcept
    : buffer_(kBufferSize)
{
    plot_ = parent;
    plot_->xAxis->setRange(0, kPlotDurationSec);
    plot_->yAxis->setRange(-1, 1);
    plot_->addGraph();

    initializeAxisX();
    initializeAxisY();
}

void AmplitudePlot::addData(std::span<const double> source)
{
    std::ranges::for_each(source, [this](double elem){
        buffer_.push(elem);
    });
}

void AmplitudePlot::update()
{
    if (buffer_.isEmpty())
        return;

    updateAxisY();
    buffer_.clear();

    plot_->graph(0)->setData(axisX_, axisY_);
    plot_->replot();
}

void AmplitudePlot::initializeAxisX()
{
    constexpr double h = kPlotDurationSec / kBufferSize;
    
    QVector<double> result(kBufferSize);
    std::ranges::generate(result.begin(), result.end(), [i=0]() mutable {
        return i++ * h;
    });

    axisX_ = std::move(result);
}

void AmplitudePlot::initializeAxisY()
{
    axisY_.resize(kBufferSize, 0);
}

void AmplitudePlot::updateAxisY()
{
    if (buffer_.isEmpty())
        return;

    QVector<double> vec;
    auto size = buffer_.size();
    auto copySize = size / kSampleStep;

    for (size_t i = 0; i < size; ++i)
    {
        if (i % kSampleStep == kSampleStep - 1)
        {
            vec.push_back(buffer_.front());
        }
        buffer_.pop();
    }

    std::copy(axisY_.begin() + copySize,
              axisY_.end(),
              axisY_.begin());

    std::copy(vec.begin(), vec.end(),
              axisY_.end() - copySize);
}
