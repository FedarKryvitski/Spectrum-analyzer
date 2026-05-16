#ifndef PLOTSFORM_H
#define PLOTSFORM_H

#include <QWidget>

namespace Ui
{
class PlotsForm;
}

class PlotsForm : public QWidget
{
    Q_OBJECT

  public:
    explicit PlotsForm(QWidget *parent = nullptr);
    ~PlotsForm();

  private:
    Ui::PlotsForm *ui;
};

#endif // PLOTSFORM_H
