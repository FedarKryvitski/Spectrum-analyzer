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
    void setSettings(const nlohmann::json &settings) override;

    nlohmann::json getSettings() const override;

    std::string getName() const override;

    ComplexBuffer process(ComplexBuffer buffer) override;

  protected:
    double gainLevel_{1.0};
};

} // namespace Plugins
