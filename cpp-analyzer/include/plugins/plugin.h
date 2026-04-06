#pragma once

#include "types.h"

namespace Plugins
{

class IPlugin
{
  public:
    IPlugin() noexcept = default;
    virtual ~IPlugin() = default;

    virtual ComplexBuffer process(ComplexBuffer input) = 0;

    virtual void reset() {}

    virtual void setEnabled(const bool enabled) { enabled_ = enabled; }

    virtual bool isEnabled() const { return enabled_; }

  protected:
    bool enabled_{true};
};

} // namespace Plugins
