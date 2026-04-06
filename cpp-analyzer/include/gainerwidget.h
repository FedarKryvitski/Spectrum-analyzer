#pragma once

#include <QWidget>

namespace Ui
{
class GainerWidget;
}

class GainerWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit GainerWidget(QWidget *parent = nullptr);
    ~GainerWidget();

  private:
    Ui::GainerWidget *ui;
};
