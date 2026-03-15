#ifndef FREQUENCYPLOT_H
#define FREQUENCYPLOT_H

#include "plot.h"
#include "ringbuffer.h"

namespace Plot {

class FrequencyPlot final : public IPlot
{
public:
    FrequencyPlot(QCustomPlot* parent) noexcept;

    void addData(std::span<const double> source) override;
    void update() override;
    void clear() override {};

private:
    void init();

private:
    QVector<double> axisX_, axisY_;
    RingBuffer<double> buffer_;
};

} // namespace Plot

#endif // FREQUENCYPLOT_H
