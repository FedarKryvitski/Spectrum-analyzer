#pragma once

#include "base/plugin.h"
#include "base/types.h"

namespace Plugins
{

class Gainer : public IPlugin
{
  public:
    Gainer() = default;
    ~Gainer() override = default;

  public:
    void setGainLevel(const double gainLevel)
    {
        gainLevel_ = gainLevel;
    }

    double getGainLevel() const
    {
        return gainLevel_;
    }

    ComplexBuffer process(ComplexBuffer buffer) override;

  protected:
    double gainLevel_{1.0};
};

} // namespace Plugins
