#ifndef AMPLITUDEPLOT_H
#define AMPLITUDEPLOT_H

#include "plot.h"
#include "ringbuffer.h"

class AmplitudePlot final : public IPlot
{
public:
    AmplitudePlot() noexcept;

    void init(QCustomPlot* parent) override;
    void addData(std::span<const double> source) override;
    void update() override;

private:
    void initializeAxisX();
    void initializeAxisY();

    void updateAxisY();

private:
    QVector<double> axisX_, axisY_;
    RingBuffer<double> buffer_;
};

#endif // AMPLITUDEPLOT_H
