#include "widgets/frequencyplot.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <span>
#include "fft/fft.h"

namespace {

constexpr size_t kBufferSize = 4096;
constexpr int kSampleRate = 44100;
constexpr double kMinFrequency = 20.0;
constexpr double kMaxFrequency = 20000.0;
constexpr size_t kMaxPoints = 800;
constexpr double kNormFactor = 2.0 / kBufferSize;
constexpr double kMinDB = -100.0;

}

namespace Plot {

FrequencyPlot::FrequencyPlot(QCustomPlot* parent) noexcept
    : buffer_(kBufferSize, 0.0)
    , processedData_(kBufferSize / 2, kMinDB)
    , fft_ip(2 + static_cast<int>(std::sqrt(kBufferSize / 2)), 0)
    , fft_w(kBufferSize / 2, 0.0)
    , fft_window(kBufferSize, 0.0)
    , frequency_axis(kBufferSize / 2, 0.0)
{
    plot_ = parent;

    init();

    plot_->xAxis->setRange(kMinFrequency, kMaxFrequency);
    plot_->xAxis->setScaleType(QCPAxis::stLogarithmic);
    plot_->yAxis->setRange(-60.0, 0.0);

    plot_->addGraph();
}

void FrequencyPlot::addData(std::span<const double> data)
{
    if (data.size() >= kBufferSize) {
        std::ranges::copy(data.end() - kBufferSize, data.end(), buffer_.begin());
    } else {
        std::shift_left(buffer_.begin(), buffer_.end(), data.size());
        std::ranges::copy(data, buffer_.end() - data.size());
    }

    processData();
}

void FrequencyPlot::init()
{
    std::ranges::generate(fft_window, [i=0, this]() mutable {
        return 0.5 * (1.0 - std::cos(2.0 * M_PI * i++ / (kBufferSize - 1)));
    });

    bool firstFound = false;
    for (size_t i = 0; i < kBufferSize / 2; i++) {
        double freq = static_cast<double>(i) * kSampleRate / kBufferSize;

        if (freq >= kMinFrequency && !firstFound) {
            firstBin = i;
            firstFound = true;
        }

        if (freq <= kMaxFrequency) {
            frequency_axis[i] = freq;
            lastBin = i;
        } else {
            frequency_axis[i] = 0.0;
        }
    }

    fft_ip[0] = 0;
}

void FrequencyPlot::processData()
{
    if (buffer_.empty()) return;

    std::vector<double> fft_input = buffer_;
    for (size_t i = 0; i < kBufferSize; i++) {
        fft_input[i] *= fft_window[i];
    }

    rdft(kBufferSize, 1, fft_input.data(), fft_ip.data(), fft_w.data());

    for (size_t k = 0; k < kBufferSize / 2; k++) {
        double amp;
        if (k == 0) {
            amp = std::abs(fft_input[0]) * kNormFactor;
        } else {
            double re = fft_input[2*k];
            double im = fft_input[2*k + 1];
            amp = std::sqrt(re*re + im*im) * kNormFactor;
        }

        double db = kMinDB;
        if (amp > 1e-10) {
            db = 20.0 * std::log10(amp);
        }
        processedData_[k] = std::max(kMinDB, std::min(0.0, db));
    }
}

void FrequencyPlot::update()
{
    if (processedData_.empty()) return;

    QVector<double> xData;
    QVector<double> yData;

    size_t totalBins = lastBin - firstBin + 1;
    size_t step = 1;

    if (totalBins > kMaxPoints) {
        step = totalBins / kMaxPoints;
        if (step < 1) step = 1;
    }

    for (size_t i = firstBin; i <= lastBin && i < processedData_.size(); i += step) {
        if (frequency_axis[i] >= kMinFrequency && frequency_axis[i] <= kMaxFrequency) {
            xData.push_back(frequency_axis[i]);
            yData.push_back(processedData_[i]);
        }
    }

    plot_->graph(0)->setData(xData, yData);
    plot_->replot();
}

void FrequencyPlot::clear()
{
    std::fill(buffer_.begin(), buffer_.end(), 0.0);
    std::fill(processedData_.begin(), processedData_.end(), kMinDB);
    update();
}

}
