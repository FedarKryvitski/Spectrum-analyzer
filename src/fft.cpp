#include "fft.h"
#include <cmath>
#include <numbers>
#include <complex>
#include <span>
#include <QVector>

constexpr static double PI = std::numbers::pi_v<double>;

QVector<double> FFT::computeDFT(const std::span<double>& signal, const std::span<double>& frequencies) {
    constexpr static double twoPi = 2 * PI;

    const int signal_size = signal.size();
    const int result_size = frequencies.size();

    QVector<double> result(result_size, 0.0);

    for (int f = 0; f < result_size; ++f) {
        const double frequency = frequencies[f];
        const double coeff = twoPi * frequency / SAMPLE_RATE;

        std::complex<double> sum{0.0, 0.0};

        for (int i = 0; i < signal_size; ++i) {
            double angle = coeff * i;
            sum += std::complex<double>(signal[i] * std::cos(angle), -signal[i] * std::sin(angle));
        }

        result[f] = std::abs(sum) / signal_size;
    }

    return result;
}

