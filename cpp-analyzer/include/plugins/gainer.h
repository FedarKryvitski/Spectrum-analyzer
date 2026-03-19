#ifndef GAINER_H
#define GAINER_H

#include "plugin.h"
#include "types.h"

namespace Plugins
{

class Gainer : public IPlugin
{
  public:
    Gainer() = default;
    ~Gainer() override = default;

  public:
    void setGainLevel(const double gainLevel) { gainLevel_ = gainLevel; }

    double getGainLevel() const { return gainLevel_; }

    ComplexBuffer process(ComplexBuffer buffer) override;

  protected:
    double gainLevel_{1.0};
};

} // namespace Plugins

#endif // GAINER_H
