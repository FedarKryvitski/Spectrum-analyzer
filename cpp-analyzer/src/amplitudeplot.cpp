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
    : m_buffer(KBufferSize)
{}

void AmplitudePlot::initialize(QCustomPlot* parent)
{
    m_plot = parent;
    m_plot->xAxis->setRange(0, kPlotDurationSec);
    m_plot->yAxis->setRange(-1, 1);
    m_plot->addGraph();

    initializeAxisX();
    initializeAxisY();
}

void AmplitudePlot::addData(const std::span<float>& source)
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

void AmplitudePlot::updatePlot()
{
    auto updateAxisY = [this]() -> void
    {
        QVector<double> vec;
        const int size = m_buffer.size();
        const int copySize = size / kSampleStep;
        for (int i = 0; i < size; ++i){
            if (i % kSampleStep == kSampleStep - 1){
                vec.push_back(m_buffer.front());
            }
            m_buffer.pop();
        }
        std::copy(m_axisY.begin() + copySize,
                  m_axisY.end(),
                  m_axisY.begin());

        std::copy(vec.begin(), vec.end(),
                  m_axisY.end() - copySize);
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
        m_plot->replot();
    }
}

void AmplitudePlot::initializeAxisX()
{
    constexpr float h = kPlotDurationSec / KBufferSize;
    
    std::vector<double> result(KBufferSize);
    std::iota(result.begin(), result.end(), 0);
    std::ranges::transform(result, result.begin(), [](const auto elem){
        return elem * h;
    });

    m_axisX = std::move(result);
}

void AmplitudePlot::initializeAxisY(){
    m_axisY.resize(KBufferSize, 0);
}
