#pragma once

#include <QWidget>

namespace Ui {
class PluginItemWidget;
}

namespace Plugins {
class IPlugin;
}

class PluginItemWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit PluginItemWidget(Plugins::IPlugin* plugin, QWidget *parent = nullptr);
    ~PluginItemWidget();

    QString getName() const;
    bool isPluginEnabled() const;

  signals:
    void moveUpRequested(PluginItemWidget* item);
    void moveDownRequested(PluginItemWidget* item);
    void removeRequested(PluginItemWidget* item);
    void toggled(PluginItemWidget* item, bool checked);
    void clicked(PluginItemWidget* item);

  protected:
    void mousePressEvent(QMouseEvent *event) override;

  private:
    void setPluginEnabled(const bool enabled);

  private:
    Ui::PluginItemWidget *ui;
};
