#ifndef FREQUENCYPLOT_H
#define FREQUENCYPLOT_H

#include "plot.h"
#include "ringbuffer.h"

class FrequencyPlot final : Plot
{
public:
    FrequencyPlot() noexcept;

    void initialize(QCustomPlot* parent) override;
    void addData(const std::span<float>& source) override;
    void updatePlot() override;

private:
    void initializeAxisX();
    void initializeAxisY();

private:
    constexpr static double MIN_FREQUENCY_X = 20.0;
    constexpr static int CHANNELS = 2;
    constexpr static double MAX_FREQUENCY_X = 20'000.0;
    constexpr static int FREQUENCY_BUFFER_SIZE = 4096;
    constexpr static int VEC_FREQUENCY_SIZE = 128;

    QVector<double> m_axisX, m_axisY;
    RingBuffer<double> m_buffer;
    std::mutex m_bufferMutex;
};

#endif // FREQUENCYPLOT_H
