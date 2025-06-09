#ifndef FFT_H
#define FFT_H

#include <QVector>
#include <span>

class FFT {
public:
    static QVector<double> computeDFT(const std::span<double>& signal,
                                      const std::span<double>& frequencies);

private:
    constexpr static int SAMPLE_RATE = 44100;
};

#endif // FFT_H

