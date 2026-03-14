#ifndef PLUGIN_H
#define PLUGIN_H

#include <vector>
#include <complex>

namespace Plugins {

class IPlugin {
  public:
    using Sample = double;
    using Complex = std::complex<Sample>;
    using Buffer = std::vector<Complex>;

  public:
    IPlugin() noexcept = default;
    virtual ~IPlugin() = default;

    Buffer process(Buffer input);
};

} // namespace Plugins

#endif // PLUGIN_H
