#ifndef FOURIER_H
#define FOURIER_H

#include <span>
#include <vector>
#include <complex>

namespace fourier {

using complex = std::complex<double>;

constexpr size_t kSampleRate{ 44100 };

std::vector<complex> dft(std::span<const double> input,
                         std::span<const double> bins,
                         double sampleRate);

std::vector<double> idft(std::span<const complex> input,
                        std::span<const double> bins,
                        double sampleRate,
                        size_t size);

std::vector<complex> fft(std::span<const double> input);
std::vector<double> ifft(std::span<const complex> input);
std::vector<double> fftFreqs(double sampleRate, size_t size);

}

#endif //FOURIER_H
