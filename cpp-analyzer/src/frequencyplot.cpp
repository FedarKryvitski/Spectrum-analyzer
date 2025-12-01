#include "frequencyplot.h"
#include "fourier.h"

#include <algorithm>

namespace {

constexpr double kMinFrequency{ 20.0 };
constexpr double kMaxFrequency{ 20'000.0 };
constexpr size_t kBufferSize{ 512 };
constexpr size_t kSampleRate{ 44100 };

} // namespace

FrequencyPlot::FrequencyPlot() noexcept
    : buffer_(kBufferSize)
    , axisX_(kBufferSize, 0)
    , axisY_(kBufferSize, 0)
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

void FrequencyPlot::addData(std::span<const double> source)
{
    std::ranges::for_each(source, [this](double elem){
        buffer_.push(elem);
    });
}

void FrequencyPlot::update()
{
    if (buffer_.isEmpty())
        return;

    updateAxisY();
    plot_->graph(0)->setData(axisX_, axisY_);
    plot_->graph(0)->addData(0.0, -80.0);
    plot_->replot();
}

void FrequencyPlot::initializeAxisX(){
    constexpr double exponent{ 0.75 };

    std::ranges::generate(axisX_, [i=0]() mutable {
        double normalized = static_cast<double>(i++) / (kBufferSize - 1);
        double adjusted = std::pow(normalized, exponent);
        double result = kMinFrequency * std::pow(kMaxFrequency / kMinFrequency, adjusted);
        return result;
    });
}

void FrequencyPlot::initializeAxisY()
{
    axisY_.resize(kBufferSize, 0);
}

void FrequencyPlot::updateAxisY()
{
    auto size = std::min(buffer_.size(), kBufferSize);

    QVector<double> frequency_buffer(size);
    for (int i = 0; i < size; ++i){
        frequency_buffer[i] = buffer_.front();
        buffer_.pop();
    }

    auto complexVec = fourier::dft(frequency_buffer, axisX_, kSampleRate);
    QVector<double> amplitudeVec(complexVec.size());

    std::ranges::transform(complexVec, amplitudeVec.begin(), [](const auto& elem){
        double magnitude = std::abs(elem);
        double dbValue = 20.0 * std::log10(std::max(magnitude, 1e-12));

        return std::clamp(dbValue, -80.0, 0.0);
    });

    axisY_ = std::move(amplitudeVec);

    buffer_.clear();
};
