#ifndef PLOT_H
#define PLOT_H

#include <qcustomplot.h>
#include <QByteArray>
#include <span>

class Plot
{
public:
    Plot() noexcept = default;
    virtual ~Plot() = default;

    virtual void initialize(QCustomPlot* parent) = 0;
    virtual void updatePlot() = 0;
    virtual void addData(const std::span<float>& source) = 0;

protected:
    QCustomPlot* m_plot;
};

#endif // PLOT_H
