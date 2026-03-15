#ifndef PLUGIN_H
#define PLUGIN_H

#include <complex>
#include <vector>

namespace Plugins
{

class IPlugin
{
  public:
    using Sample = double;
    using Complex = std::complex<Sample>;
    using Buffer = std::vector<Complex>;

  public:
    IPlugin() noexcept = default;
    virtual ~IPlugin() = default;

    virtual Buffer process(Buffer input) = 0;
    // virtual void setSink(IPipelineSink);
    // virtual void setSource(IPipelineSource);
};

} // namespace Plugins

#endif // PLUGIN_H
