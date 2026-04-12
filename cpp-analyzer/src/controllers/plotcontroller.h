#pragma once

#include "widgets/plot.h"

#include <QObject>
#include <QTimer>
#include <optional>
#include <vector>

namespace Plot
{

class PlotController : public QObject
{
    Q_OBJECT

  public:
    explicit PlotController(QCustomPlot *inputAmplitudeWidget, QCustomPlot *inputFrequencyWidget,
                            QCustomPlot *outputAmplitudeWidget, QCustomPlot *outputFrequencyWidget,
                            QObject *parent = nullptr);

    void start();
    void stop();
    void clear();

  public slots:
    void onFrameReady(std::vector<double> input, std::vector<double> output);

  private slots:
    void updatePlots();

  private:
    struct PlotFrame
    {
        std::vector<double> input;
        std::vector<double> output;
    };

  private:
    std::unique_ptr<IPlot> inputAmplitudePlot_{nullptr};
    std::unique_ptr<IPlot> inputFrequencyPlot_{nullptr};
    std::unique_ptr<IPlot> outputAmplitudePlot_{nullptr};
    std::unique_ptr<IPlot> outputFrequencyPlot_{nullptr};

    QTimer plotTimer_;
    std::optional<PlotFrame> latestFrame_;
};

} // namespace Plot
