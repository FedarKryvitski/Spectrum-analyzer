#include "widgets/frequencyplot.h"
// #include "fourier.h"
#include "audioconverter.h"

#include <algorithm>

namespace {

constexpr static int kChannels = 2;
constexpr static double kMinFrequency = 20.0;
constexpr static double kMaxFrequency = 20'000.0;
constexpr static int kFrequencyBufferSize = 4096;

} // namespace

namespace Plot {

FrequencyPlot::FrequencyPlot(QCustomPlot* parent) noexcept
    : buffer_(kFrequencyBufferSize)
    , axisX_(kFrequencyBufferSize, 0)
    , axisY_(kFrequencyBufferSize, 0)
{
    plot_ = parent;

    plot_->xAxis->setRange(kMinFrequency, kMaxFrequency);
    plot_->yAxis->setRange(-80.0, 0.0);
    plot_->addGraph();

    init();
}

void FrequencyPlot::init()
{
    constexpr double exponent{ 0.75 };

    std::ranges::generate(axisX_, [i=0]() mutable {
        double normalized = static_cast<double>(i) / (kFrequencyBufferSize - 1);
        double adjusted = std::pow(normalized, exponent);
        double result = kMinFrequency * std::pow(kMaxFrequency / kMinFrequency, adjusted);
        ++i;
        return result;
    });

    axisY_.resize(kFrequencyBufferSize, 0);
}

void FrequencyPlot::addData(std::span<const double> source)
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

void FrequencyPlot::update()
{
    const int size = std::min(static_cast<int>(buffer_.size()), kFrequencyBufferSize);
    std::vector<float> frequency_buffer(size);
    for (int i = 0; i < size; ++i){
        frequency_buffer[i] = buffer_.front();
        buffer_.pop();
    }
    //QVector<double> result = fourier::dft(frequency_buffer, axisX_, 48000);

    plot_->graph(0)->setData(axisX_, axisY_);
    plot_->graph(0)->addData(0.0, -60.0);
    plot_->replot();
}

} // namespace Plot
