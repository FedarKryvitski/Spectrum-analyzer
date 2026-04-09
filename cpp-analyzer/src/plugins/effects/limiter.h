#pragma once

#include "base/plugin.h"
#include "base/types.h"

namespace Plugins
{

class Limiter : public IPlugin
{
  public:
    Limiter() = default;
    ~Limiter() override = default;

  public:
    void setLimitLevel(const double limitLevel) { limitLevel_ = limitLevel; }

    double getLimitLevel() const { return limitLevel_; }

    ComplexBuffer process(ComplexBuffer buffer) override;

  protected:
    double limitLevel_{1.0};
};

} // namespace Plugins
