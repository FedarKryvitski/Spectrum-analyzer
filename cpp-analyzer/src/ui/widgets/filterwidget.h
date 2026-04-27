#pragma once

#include "ui_filterwidget.h"
#include "widgets/ipluginwidget.h"

class FilterWidget : public IPluginWidget
{
    Q_OBJECT

  public:
    explicit FilterWidget(Plugins::IPlugin *plugin, QWidget *parent = nullptr);
    ~FilterWidget() override;

    void updateFromPlugin() override;
    void applyToPlugin() override;

  protected:
    void update() override;

  private:
    Ui::FilterWidget *ui;

    double lowCutoffFrequency_{};
    double highCutoffFrequency_{};
};
