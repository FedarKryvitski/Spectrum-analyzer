#ifndef AMPLITUDEPLOT_H
#define AMPLITUDEPLOT_H

#include "plot.h"
#include "ringbuffer.h"

class AmplitudePlot final : Plot
{
public:
    AmplitudePlot() noexcept;

    void initialize(QCustomPlot* parent) override;
    void addData(const std::span<float>& source) override;
    void updatePlot() override;

private:
    void initializeAxisX();
    void initializeAxisY();

private:
    constexpr static int FREQUENCY = 44100;
    constexpr static float PLOT_DURATION = 3.0;
    constexpr static int SAMPLE_STEP = 10;
    constexpr static int CHANNELS = 2;
    constexpr static int VEC_AMPLITUDE_SIZE = PLOT_DURATION * FREQUENCY / SAMPLE_STEP;

    QVector<double> m_axisX, m_axisY;
    RingBuffer<float> m_buffer;
    std::mutex m_bufferMutex;
};

#endif // AMPLITUDEPLOT_H
