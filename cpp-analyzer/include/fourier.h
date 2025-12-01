#ifndef FOURIER_H
#define FOURIER_H

#include <span>
#include <vector>
#include <complex>

namespace fourier {

using complex = std::complex<float>;

constexpr float kDefaultSampleRate = 48000;

std::vector<complex> dft(std::span<const float> input,
                         std::span<const float> bins,
                         float sampleRate);

std::vector<float> idft(std::span<const complex> input,
                        std::span<const float> bins,
                        float sampleRate,
                        size_t size);

std::vector<complex> fft(std::span<const float> input);
std::vector<float> ifft(std::span<const complex> input);
std::vector<float> fftFreqs(float sampleRate, size_t size);

}

#endif //FOURIER_H
