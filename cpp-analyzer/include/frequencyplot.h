#ifndef FREQUENCYPLOT_H
#define FREQUENCYPLOT_H

#include "plot.h"
#include "ringbuffer.h"

class FrequencyPlot final : public IPlot
{
public:
    FrequencyPlot() noexcept;

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

#endif // FREQUENCYPLOT_H
