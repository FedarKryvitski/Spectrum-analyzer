#pragma once

#include "ui_limiterwidget.h"
#include "widgets/ipluginwidget.h"

class LimiterWidget : public IPluginWidget
{
    Q_OBJECT

  public:
    explicit LimiterWidget(Plugins::IPlugin *plugin, QWidget *parent = nullptr);
    ~LimiterWidget() override;

    void updateFromPlugin() override;
    void applyToPlugin() override;

  private:
    Ui::LimiterWidget *ui;
};
