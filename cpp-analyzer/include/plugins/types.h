#pragma once

#include <complex>
#include <vector>

namespace Plugins
{

using Sample = double;
using Complex = std::complex<Sample>;

using Buffer = std::vector<Sample>;
using ComplexBuffer = std::vector<Complex>;

} // namespace Plugins
