#include "widgets/amplitudeplot.h"

#include <algorithm>

namespace {

using namespace std::chrono_literals;

constexpr static size_t kPlotSampleStep{ 128 };
constexpr static size_t kPlotRangeX{ 3 * 44'100 / kPlotSampleStep };

} // namespace

namespace Plot {

AmplitudePlot::AmplitudePlot(QCustomPlot* parent) noexcept
    : axisX_(kPlotRangeX)
    , axisY_(kPlotRangeX)
{
    plot_ = parent;
    plot_->xAxis->setRange(0, kPlotRangeX);
    plot_->yAxis->setRange(-1, 1);
    plot_->addGraph();

    std::iota(axisX_.begin(), axisX_.end(), 0);
    std::fill(axisY_.begin(), axisY_.end(), 0);
}

void AmplitudePlot::addData(Data data)
{
    std::vector<double> decimatedData;
    decimatedData.reserve(data.size() / kPlotSampleStep + 1);

    for (size_t i = kPlotSampleStep - 1; i < data.size(); i += kPlotSampleStep) {
        decimatedData.push_back(data[i]);
    }

    const size_t newDataSize = decimatedData.size();

    if (newDataSize == 0)
        return;

    if (newDataSize >= kPlotRangeX)
    {
        std::ranges::copy(decimatedData.end() - kPlotRangeX,
                          decimatedData.end(),
                          axisY_.begin());
    }
    else
    {
        std::shift_left(axisY_.begin(), axisY_.end(), newDataSize);

        std::ranges::copy(decimatedData, axisY_.end() - newDataSize);
    }
}

void AmplitudePlot::update()
{
    plot_->graph(0)->setData(axisX_, axisY_);
    plot_->replot();
}

void AmplitudePlot::clear()
{
    std::fill(axisY_.begin(), axisY_.end(), 0);
}

} // namespace Plot
