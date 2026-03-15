#include "fourier.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <numeric>

namespace fourier
{

namespace
{

constexpr auto pi = std::numbers::pi;

int upper_log2(const int x)
{
    constexpr int upperBound{30};

    int i = 0;
    while (i < upperBound)
    {
        const int current = 1 << i;
        if (current >= x)
            return i;

        ++i;
    }
    return upperBound;
}

const std::vector<complex> phase_vec(const int len)
{
    constexpr float radius{1.f};

    std::vector<complex> res(len);
    std::generate(res.begin(), res.end(), [&, i = 0]() mutable {
        const float phase = -2.f * pi * i++ / len;
        return std::polar(radius, phase);
    });

    return res;
}

void forward(std::vector<complex> &prev, std::vector<complex> &temp, const std::vector<complex> &phases, const int turn,
             const int n_bits)
{
    if (turn == n_bits)
        return;

    const int group_size = 1 << (turn + 1);
    const int num_groups = prev.size() / group_size;
    const int phase_angular_freq = num_groups;

    for (int i_group = 0; i_group < num_groups; ++i_group)
    {
        const int base_index = i_group * group_size;
        for (int j = 0; j < group_size / 2; ++j)
        {
            const int x0_index = base_index + j;
            const int x1_index = base_index + group_size / 2 + j;

            prev[x1_index] *= phases[j * phase_angular_freq];
            temp[x0_index] = prev[x0_index] + prev[x1_index];
            temp[x1_index] = prev[x0_index] - prev[x1_index];
        }
    }
}

void bit_reversal_permutation(std::vector<complex> &vec, const int n_bits)
{
    if (vec.size() <= 2)
        return;

    if (vec.size() == 4)
    {
        std::swap(vec[1], vec[3]);
        return;
    }

    std::vector<int> bit_rerversal(vec.size());

    bit_rerversal[0] = 0;
    bit_rerversal[1] = 1 << (n_bits - 1);
    bit_rerversal[2] = 1 << (n_bits - 2);
    bit_rerversal[3] = bit_rerversal[1] + bit_rerversal[2];

    for (int k = 3; k <= n_bits; ++k)
    {
        const int nk = (1 << k) - 1;
        const int n_km1 = (1 << (k - 1)) - 1;
        bit_rerversal[nk] = bit_rerversal[n_km1] + (1 << (n_bits - k));
        for (int i = 1; i <= n_km1; ++i)
        {
            bit_rerversal[nk - i] = bit_rerversal[nk] - bit_rerversal[i];
        }
    }

    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (bit_rerversal[i] > i)
            std::swap(vec[i], vec[bit_rerversal[i]]);
    }
}

std::vector<complex> fftImpl(std::span<const complex> inputs)
{
    if (inputs.empty())
        return {};

    const int n_bits = upper_log2(inputs.size());
    const int length = 1 << n_bits;
    const std::vector<complex> phases = phase_vec(length);

    std::vector<complex> prev(length);
    std::vector<complex> temp(length);
    std::copy(inputs.begin(), inputs.end(), prev.begin());

    bit_reversal_permutation(prev, n_bits);
    for (int turn = 0; turn < n_bits; ++turn)
    {
        forward(prev, temp, phases, turn, n_bits);
        std::swap(prev, temp);
    }
    return prev;
}

std::vector<complex> ifftImpl(std::span<const complex> inputs)
{
    std::vector<complex> reverse_freq_spectrum(inputs.begin(), inputs.end());
    std::reverse(std::next(reverse_freq_spectrum.begin()), reverse_freq_spectrum.end());

    return fftImpl(reverse_freq_spectrum);
}

} // namespace

std::vector<complex> dft(std::span<const float> input, std::span<const float> bins, float sampleRate)
{
    std::vector<complex> output(bins.size());

    for (int i = 0; i < bins.size(); ++i)
    {
        const auto frequency = bins[i];
        complex sum(0, 0);

        for (size_t n = 0; n < input.size(); ++n)
        {
            const float angle = -2.f * pi * frequency * n / sampleRate;
            sum += input[n] * complex(std::cos(angle), std::sin(angle));
        }
        output[i] = sum;
    }

    return output;
}

std::vector<float> idft(std::span<const complex> amplitudes, std::span<const float> bins, float sampleRate,
                        const size_t size)
{
    if (amplitudes.size() != bins.size())
        throw std::runtime_error("Amplitudes and bins sizes aren't equal");

    std::vector<float> output(size);
    for (size_t i = 0; i < size; ++i)
    {
        complex sum{};
        for (size_t j = 0; j < bins.size(); ++j)
        {
            const float angle = 2.f * pi * i * j / sampleRate;
            sum += amplitudes[i] * complex(std::cos(angle), std::sin(angle));
        }
        output[i] = sum.real() / size;
    }
    return output;
}

std::vector<complex> fft(std::span<const float> input)
{
    std::vector<complex> data(input.size());
    std::ranges::transform(input, data.begin(), [](const auto &num) { return std::complex(num, 0.f); });

    return fftImpl(data);
}

std::vector<float> ifft(std::span<const complex> input)
{
    const int inputSize = input.size();
    const auto data = ifftImpl(input);

    std::vector<float> result(data.size());
    std::ranges::transform(data, result.begin(), [inputSize](const auto &num) { return num.real() / inputSize; });

    return result;
}

std::vector<float> fftFreqs(float sampleRate, size_t size)
{
    std::vector<float> freqs(size);
    for (size_t i = 0; i < size; ++i)
    {
        if (i <= size / 2)
            freqs[i] = static_cast<float>(i) * sampleRate / size;
        else
            freqs[i] = static_cast<float>(i - size) * sampleRate / size;
    }
    return freqs;
}

} // namespace fourier
