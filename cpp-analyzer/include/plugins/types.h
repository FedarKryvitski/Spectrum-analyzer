#ifndef TYPES_H
#define TYPES_H

#include <complex>
#include <vector>

namespace Plugins
{

using Sample = double;
using Complex = std::complex<Sample>;

using Buffer = std::vector<Sample>;
using ComplexBuffer = std::vector<Complex>;

} // namespace Plugins

#endif // TYPES_H
