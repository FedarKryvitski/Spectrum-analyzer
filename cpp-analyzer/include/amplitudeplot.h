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
    QVector<double> m_axisX, m_axisY;
    RingBuffer<float> m_buffer;
};

#endif // AMPLITUDEPLOT_H
