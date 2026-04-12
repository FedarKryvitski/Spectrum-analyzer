#pragma once

#include <QWidget>

namespace Ui {
class PluginItemWidget;
}

class PluginItemWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit PluginItemWidget(const QString &name, QWidget *parent = nullptr);
    ~PluginItemWidget();

    QString getName() const;

  signals:
    void moveUpRequested(PluginItemWidget* item);
    void moveDownRequested(PluginItemWidget* item);
    void removeRequested(PluginItemWidget* item);
    void clicked(PluginItemWidget* item);

  protected:
    void mousePressEvent(QMouseEvent *event) override;

  private:
    Ui::PluginItemWidget *ui;
};
