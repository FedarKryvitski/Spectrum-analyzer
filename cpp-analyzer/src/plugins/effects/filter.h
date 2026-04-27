#pragma once

#include "base/plugin.h"
#include "base/types.h"

namespace Plugins
{

class Filter : public IPlugin
{
  public:
    Filter() = default;
    ~Filter() override = default;

  public:
    void setSettings(const nlohmann::json &settings) override;

    nlohmann::json getSettings() const override;

    std::string getName() const override;

    ComplexBuffer process(ComplexBuffer buffer) override;

  protected:
    double lowCutoffFrequency_{0.0};
    double highCutoffFrequency_{20000.0};
};

} // namespace Plugins
