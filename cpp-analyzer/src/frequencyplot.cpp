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
    : m_buffer(kFrequencyBufferSize)
    , m_axisX(kFrequencyBufferSize, 0)
    , m_axisY(kFrequencyBufferSize, 0)
{}

void FrequencyPlot::initialize(QCustomPlot* parent)
{
    m_plot = parent;

    m_plot->xAxis->setRange(kMinFrequency, kMaxFrequency);
    m_plot->yAxis->setRange(-80.0, 0.0);
    m_plot->addGraph();

    initializeAxisX();
    initializeAxisY();
}

void FrequencyPlot::addData(const std::span<float>& source)
{
    if constexpr (kChannels == 2) {
        auto monoSource = AudioConverter::createMono(source);
        std::ranges::for_each(monoSource, [this](float elem){
            m_buffer.push(elem);
        });
    }
    else {
        std::ranges::for_each(source, [this](float elem){
            m_buffer.push(elem);
        });
    }
}


void FrequencyPlot::updatePlot()
{
    auto updateAxisY = [this]() -> void {
        const int size = std::min(static_cast<int>(m_buffer.size()), kFrequencyBufferSize);
        QVector<double> frequency_buffer(size);
        for (int i = 0; i < size; ++i){
            frequency_buffer[i] = m_buffer.front();
            m_buffer.pop();
        }
        // //QVector<double> result = fourier::dft(frequency_buffer, m_axisX, 48000);

        // std::ranges::transform(result, result.begin(), [](const double magnitude){
        //     double dbValue = 20.0 * std::log10(std::max(magnitude, 1e-12));
        //     return std::clamp(dbValue, -80.0, 0.0);
        // });

        // m_axisY = std::move(result);
    };

    bool needsUpdate = false;
    {
        needsUpdate = m_buffer.size() > 0;
        if (needsUpdate){
            updateAxisY();
            m_buffer.clear();
        }
    }

    if (needsUpdate) {
        m_plot->graph(0)->setData(m_axisX, m_axisY);
        m_plot->graph(0)->addData(0.0, -80.0);
        m_plot->replot();
    }
}

void FrequencyPlot::initializeAxisX(){
    constexpr double exponent = 0.75;

    std::ranges::generate(m_axisX, [i=0]() mutable {
        double normalized = static_cast<double>(i) / (kFrequencyBufferSize - 1);
        double adjusted = std::pow(normalized, exponent);
        double result = kMinFrequency * std::pow(kMaxFrequency / kMinFrequency, adjusted);
        ++i;
        return result;
    });
}

void FrequencyPlot::initializeAxisY(){
    m_axisY.resize(kFrequencyBufferSize, 0);
}
