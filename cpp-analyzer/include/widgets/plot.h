#ifndef PLOT_H
#define PLOT_H

#include <qcustomplot.h>
#include <span>

namespace Plot {

using Sample = double;
using Data = std::span<const Sample>;

class IPlot
{
public:
    IPlot() noexcept = default;
    virtual ~IPlot() = default;

    virtual void update() = 0;
    virtual void clear() = 0;
    virtual void addData(Data data) = 0;

protected:
    QCustomPlot* plot_;
};

} // namespace Plot

#endif // PLOT_H
