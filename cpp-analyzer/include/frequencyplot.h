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
    QVector<double> m_axisX, m_axisY;
    RingBuffer<double> m_buffer;
};

#endif // FREQUENCYPLOT_H
