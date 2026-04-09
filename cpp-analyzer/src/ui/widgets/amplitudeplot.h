#pragma once

#include "plot.h"

namespace Plot
{

class AmplitudePlot final : public IPlot
{
  public:
    AmplitudePlot(QCustomPlot *parent) noexcept;

    void addData(Data data) override;
    void update() override;
    void clear() override;

  private:
    QVector<double> axisX_, axisY_;
};

} // namespace Plot
