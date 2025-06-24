#ifndef FFT_H
#define FFT_H

#include <QVector>
#include <span>

namespace FFT {

QVector<double> computeDFT(const std::span<double>& signal,
                                      const std::span<double>& frequencies);

constexpr int SAMPLE_RATE = 44100;

}

#endif // FFT_H

