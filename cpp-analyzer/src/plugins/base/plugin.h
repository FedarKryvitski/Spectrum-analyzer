#pragma once

#include "types.h"
#include <nlohmann/json.hpp>

namespace Plugins
{

class IPlugin
{
  public:
    IPlugin() noexcept = default;
    virtual ~IPlugin() = default;

    virtual ComplexBuffer process(ComplexBuffer input) = 0;

    virtual void reset()
    {
    }

    virtual nlohmann::json getSettings() const = 0;

    virtual void setSettings(const nlohmann::json &settings) = 0;

    virtual std::string getName() const = 0;

    virtual void setEnabled(const bool enabled)
    {
        enabled_ = enabled;
    }

    virtual bool isEnabled() const
    {
        return enabled_;
    }

  protected:
    bool enabled_{true};
};

} // namespace Plugins
