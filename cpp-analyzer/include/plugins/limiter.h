#ifndef LIMITER_H
#define LIMITER_H

#include "plugin.h"
#include "types.h"

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

#endif // LIMITER_H
