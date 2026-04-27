#pragma once

#include "ui_gainerwidget.h"

#include "widgets/ipluginwidget.h"

class GainerWidget : public IPluginWidget
{
    Q_OBJECT

  public:
    explicit GainerWidget(Plugins::IPlugin *plugin, QWidget *parent = nullptr);
    ~GainerWidget() override;

    void updateFromPlugin() override;
    void applyToPlugin() override;

  protected:
    void update() override;

  private:
    Ui::GainerWidget *ui;

    double gainValue_{};
};
