#include "frequencyplot.h"
#include "audioconverter.h"
#include <algorithm>
#include "fourier.h"

namespace {

constexpr static int kChannels{ 2 };
constexpr static double kMinFrequency{ 20.0 };
constexpr static double kMaxFrequency{ 20'000.0 };
constexpr static int kFrequencyBufferSize{ 4096 };

}

FrequencyPlot::FrequencyPlot() noexcept
    : buffer_(kFrequencyBufferSize)
    , axisX_(kFrequencyBufferSize, 0)
    , axisY_(kFrequencyBufferSize, 0)
{}

void FrequencyPlot::initialize(QCustomPlot* parent)
{
    plot_ = parent;

    plot_->xAxis->setRange(kMinFrequency, kMaxFrequency);
    plot_->yAxis->setRange(-80.0, 0.0);
    plot_->addGraph();

    initializeAxisX();
    initializeAxisY();
}

void FrequencyPlot::addData(std::span<const float> source)
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
    if (buffer_.size() > 0)
    {
        updateAxisY();
        plot_->graph(0)->setData(axisX_, axisY_);
        plot_->graph(0)->addData(0.0, -80.0);
        plot_->replot();
    }
}

void FrequencyPlot::initializeAxisX(){
    constexpr double exponent = 0.75;

    std::ranges::generate(axisX_, [i=0]() mutable {
        double normalized = static_cast<double>(i) / (kFrequencyBufferSize - 1);
        double adjusted = std::pow(normalized, exponent);
        double result = kMinFrequency * std::pow(kMaxFrequency / kMinFrequency, adjusted);
        ++i;
        return result;
    });
}

void FrequencyPlot::initializeAxisY()
{
    axisY_.resize(kFrequencyBufferSize, 0);
}

void FrequencyPlot::updateAxisY()
{
    const int size = std::min(static_cast<int>(buffer_.size()), kFrequencyBufferSize);
    QVector<double> frequency_buffer(size);
    for (int i = 0; i < size; ++i){
        frequency_buffer[i] = buffer_.front();
        buffer_.pop();
    }
    // //QVector<double> result = fourier::dft(frequency_buffer, m_axisX, 48000);

    // std::ranges::transform(result, result.begin(), [](const double magnitude){
    //     double dbValue = 20.0 * std::log10(std::max(magnitude, 1e-12));
    //     return std::clamp(dbValue, -80.0, 0.0);
    // });

    // m_axisY = std::move(result);

    buffer_.clear();
};
