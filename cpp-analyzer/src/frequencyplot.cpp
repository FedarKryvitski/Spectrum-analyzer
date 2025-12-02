#include "frequencyplot.h"
#include "fourier.h"

#include <algorithm>

namespace {

constexpr double kMinFrequency{ 20.0 };
constexpr double kMaxFrequency{ 20'000.0 };
constexpr size_t kBufferSize{ 1024 };
constexpr size_t kFreqCount{ 256 };
constexpr size_t kSampleRate{ 44100 };

} // namespace

FrequencyPlot::FrequencyPlot() noexcept
    : buffer_(kBufferSize)
    , axisX_(kFreqCount, 0)
    , axisY_(kFreqCount, 0)
{}

void FrequencyPlot::init(QCustomPlot* parent)
{
    plot_ = parent;
    plot_->xAxis->setScaleType(QCPAxis::stLogarithmic);
    plot_->xAxis->setRange(kMinFrequency, kMaxFrequency);
    plot_->yAxis->setRange(-60.0, 0.0);
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
    if (!buffer_.isFull())
        return;

    updateAxisY();

    plot_->graph(0)->setData(axisX_, axisY_);
    plot_->graph(0)->addData(0.0, -60.0);
    plot_->replot();
}

void FrequencyPlot::initializeAxisX()
{
    constexpr double exponent{ 0.75 };

    std::ranges::generate(axisX_, [i=0]() mutable {
        double normalized = static_cast<double>(i++) / (kFreqCount - 1);
        double adjusted = std::pow(normalized, exponent);
        double result = kMinFrequency * std::pow(kMaxFrequency / kMinFrequency, adjusted);
        return result;
    });
}

void FrequencyPlot::initializeAxisY()
{
    axisY_.resize(kFreqCount, 0);
}

void FrequencyPlot::updateAxisY()
{
    auto size = std::min(buffer_.size(), kBufferSize);

    if (size == 0) 
        return;

    QVector<double> timeDomainData(size);
    for (int i = 0; i < size; ++i)
    {
        timeDomainData[i] = buffer_.front();
        buffer_.pop();
    }

    QVector<double> windowedData(size);
    for (int i = 0; i < size; ++i)
    {
        double window = 0.5 * (1 - std::cos(2.0 * M_PI * i / (size - 1)));
        windowedData[i] = timeDomainData[i] * window;
    }

    auto complexVec = fourier::dft(windowedData, axisX_, kSampleRate);
    QVector<double> amplitudeVec(complexVec.size());

    std::ranges::transform(complexVec, amplitudeVec.begin(), [size](const auto& elem){
        double magnitude = std::abs(elem) / size;
        double dbValue = 20.0 * std::log10(std::max(magnitude, 1e-12));

        return std::clamp(dbValue, -80.0, 0.0);
    });

    axisY_ = std::move(amplitudeVec);
}
