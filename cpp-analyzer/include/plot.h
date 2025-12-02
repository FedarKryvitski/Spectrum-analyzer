#ifndef PLOT_H
#define PLOT_H

#include <qcustomplot.h>
#include <span>

class IPlot
{
public:
    IPlot() noexcept = default;
    virtual ~IPlot() = default;

    virtual void init(QCustomPlot* parent) = 0;
    virtual void update() = 0;
    virtual void addData(std::span<const double> source) = 0;

protected:
    QCustomPlot* plot_;
};

#endif // PLOT_H
