#pragma once

#include <QWidget>

namespace Ui
{
class PluginsListForm;
}

class PluginsListForm : public QWidget
{
    Q_OBJECT

  public:
    explicit PluginsListForm(QWidget *parent = nullptr);
    ~PluginsListForm();

  signals:
    void backRequested();

  private slots:
    void onAddItemSlot();
    void onRemoveItemSlot();

  private:
    void init();

  private:
    Ui::PluginsListForm *ui;
};
