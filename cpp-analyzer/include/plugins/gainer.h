#ifndef GAINER_H
#define GAINER_H

#include "plugin.h"

namespace Plugins {

class Gainer : public IPlugin
{
  public:
    Gainer() = default;
    ~Gainer() override = default;

  public:
    void setGainLevel(const double gainLevel) { gainLevel_ = gainLevel; }
    double getGainLevel() const { return gainLevel_; }

    Buffer process(Buffer buffer);

  protected:
    double gainLevel_{1.0};
};

}

#endif // GAINER_H
