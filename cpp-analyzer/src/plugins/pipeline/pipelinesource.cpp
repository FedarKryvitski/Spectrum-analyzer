#include "pipeline/pipelinesource.h"

#include <fft/fft.h>

namespace Plugins {

ComplexBuffer PipelineSource::write(Buffer buffer)
{
    if (buffer.empty())
        return {};

    const int n = static_cast<int>(buffer.size());

    std::vector<double> a(buffer.begin(), buffer.end());
    std::vector<int> ip(2 + static_cast<int>(std::sqrt(n)));
    std::vector<double> w(n / 2);

    ip[0] = 0;

    rdft(n, 1, a.data(), ip.data(), w.data());

    ComplexBuffer output;
    output.reserve(n / 2 + 1);

    output.emplace_back(a[0], 0.0);

    for (int k = 1; k < n / 2; ++k)
    {
        output.emplace_back(a[2 * k], a[2 * k + 1]);
    }

    output.emplace_back(a[1], 0.0);

    return output;
}

} // namespace Plugins
