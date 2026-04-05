#include "widgets/amplitudeplot.h"

#include <algorithm>

namespace {

using namespace std::chrono_literals;

constexpr static size_t kPlotDecimationStep{ 32 };
constexpr static size_t kPlotRangeX{ 1024 };

template<typename T>
std::vector<T> decimate(std::span<const T>& source, size_t decimationStep)
{
    std::vector<T> result;
    result.reserve(source.size() / decimationStep + 1);

    for (size_t i = 0; i < source.size(); i += decimationStep)
    {
        result.push_back(source[i]);
    }

    return result;
}

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

    plot_->setOpenGl(true);
    plot_->setNoAntialiasingOnDrag(true);
    plot_->setInteractions(QCP::iNone);
    plot_->graph(0)->setLineStyle(QCPGraph::lsLine);
    plot_->graph(0)->setAntialiased(false);

    std::iota(axisX_.begin(), axisX_.end(), 0);
    std::fill(axisY_.begin(), axisY_.end(), 0);
}

void AmplitudePlot::addData(Data inputData)
{
    auto decimatedData = decimate(inputData, kPlotDecimationStep);
    if (decimatedData.empty())
        return;

    const size_t newDataSize = decimatedData.size();

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
    plot_->graph(0)->setData(axisX_, axisY_, true);
    plot_->replot();
}

void AmplitudePlot::clear()
{
    std::fill(axisY_.begin(), axisY_.end(), 0);
}

} // namespace Plot
