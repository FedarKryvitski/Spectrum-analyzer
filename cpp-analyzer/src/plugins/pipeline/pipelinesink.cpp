#include "pipeline/pipelinesink.h"

#include <fft/fft.h>

namespace Plugins {

Buffer PipelineSink::read(ComplexBuffer buffer)
{
    if (buffer.empty())
        return {};

    const int n = (buffer.size() - 1) * 2;

    std::vector<double> a(n, 0.0);

    a[0] = buffer[0].real();
    a[1] = buffer.back().real();

    for (int k = 1; k < n / 2; ++k)
    {
        a[2 * k]     = buffer[k].real();
        a[2 * k + 1] = buffer[k].imag();
    }

    std::vector<int> ip(2 + static_cast<int>(std::sqrt(n)));
    std::vector<double> w(n / 2);
    ip[0] = 0;

    rdft(n, -1, a.data(), ip.data(), w.data());

    for (auto& v : a)
        v /= n;

    Buffer output(a.begin(), a.end());

    return output;
}

} // namespace Plugins
