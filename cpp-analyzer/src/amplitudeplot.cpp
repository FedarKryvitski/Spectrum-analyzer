#include "amplitudeplot.h"
#include "audioconverter.h"

#include <algorithm>

namespace {

using namespace std::chrono_literals;

constexpr static int kSampleRate{ 44100 };
constexpr static int kChannels{ 2 };
constexpr static float kPlotDurationSec{ 3.f };
constexpr static int kSampleStep{ 10 };
constexpr static int KBufferSize = kPlotDurationSec * kSampleRate / kSampleStep;

}

AmplitudePlot::AmplitudePlot() noexcept
    : buffer_(KBufferSize)
{}

void AmplitudePlot::initialize(QCustomPlot* parent)
{
    plot_ = parent;
    plot_->xAxis->setRange(0, kPlotDurationSec);
    plot_->yAxis->setRange(-1, 1);
    plot_->addGraph();

    initializeAxisX();
    initializeAxisY();
}

void AmplitudePlot::addData(std::span<const float> source)
{
    if constexpr (kChannels == 2) {
        auto monoSource = AudioConverter::createMono(source);
        std::ranges::for_each(monoSource, [this](float elem){
            buffer_.push(elem);
        });
    }
    else {
        std::ranges::for_each(source, [this](float elem){
            buffer_.push(elem);
        });
    }
}

void AmplitudePlot::update()
{
    if (buffer_.size() > 0)
    {
        updateAxisY();

        plot_->graph(0)->setData(axisX_, axisY_);
        plot_->replot();
    }
}

void AmplitudePlot::initializeAxisX()
{
    constexpr float h = kPlotDurationSec / KBufferSize;
    
    QVector<double> result(KBufferSize);
    std::ranges::generate(result.begin(), result.end(), [i=0]() mutable {
        return i++ * h;
    });

    axisX_ = std::move(result);
}

void AmplitudePlot::initializeAxisY()
{
    axisY_.resize(KBufferSize, 0);
}

void AmplitudePlot::updateAxisY()
{
    if (buffer_.isEmpty())
        return;

    QVector<double> vec;
    const int size = buffer_.size();
    const int copySize = size / kSampleStep;
    for (int i = 0; i < size; ++i){
        if (i % kSampleStep == kSampleStep - 1){
            vec.push_back(buffer_.front());
        }
        buffer_.pop();
    }
    std::copy(axisY_.begin() + copySize,
              axisY_.end(),
              axisY_.begin());

    std::copy(vec.begin(), vec.end(),
              axisY_.end() - copySize);

    buffer_.clear();
}
