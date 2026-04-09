#pragma once

#include "widgets/amplitudeplot.h"
#include "widgets/frequencyplot.h"

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
    void onFrameReady(const std::vector<double> &input, const std::vector<double> &output);

  private slots:
    void updatePlots();

  private:
    struct PlotFrame
    {
        std::vector<double> input;
        std::vector<double> output;
    };

  private:
    std::unique_ptr<AmplitudePlot> amplitudePlot_{nullptr};
    std::unique_ptr<FrequencyPlot> frequencyPlot_{nullptr};
    std::unique_ptr<AmplitudePlot> amplitudePlot2_{nullptr};
    std::unique_ptr<FrequencyPlot> frequencyPlot2_{nullptr};

    QTimer plotTimer_;
    std::optional<PlotFrame> latestFrame_;
};

} // namespace Plot
