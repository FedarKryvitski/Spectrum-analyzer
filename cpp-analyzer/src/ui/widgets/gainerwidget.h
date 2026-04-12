#pragma once

#include "ui_gainerwidget.h"

#include "widgets/ipluginwidget.h"
#include "effects/gainer.h"

class GainerWidget : public IPluginWidget
{
    Q_OBJECT

  public:
    explicit GainerWidget(Plugins::IPlugin* plugin, QWidget* parent = nullptr);
    ~GainerWidget() override;

    void updateFromPlugin() override {}
    void applyToPlugin() override {}

  private:
    Ui::GainerWidget* ui;
};
