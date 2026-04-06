#pragma once

#include "plot.h"

namespace Plot
{

class FrequencyPlot final : public IPlot
{
  public:
    FrequencyPlot(QCustomPlot *parent) noexcept;

    void addData(std::span<const double> source) override;
    void update() override;
    void clear() override;

  private:
    void init();
    void processData();

  private:
    std::vector<double> buffer_, processedData_;

    std::vector<int> fft_ip;
    std::vector<double> fft_w, fft_window, frequency_axis;
    size_t firstBin = 0;
    size_t lastBin = 0;
};

} // namespace Plot
