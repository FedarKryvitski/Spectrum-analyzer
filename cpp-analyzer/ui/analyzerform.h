#ifndef ANALYZERFORM_H
#define ANALYZERFORM_H

#include <QWidget>

namespace Ui
{
class AnalyzerForm;
}

class AnalyzerForm : public QWidget
{
    Q_OBJECT

  public:
    explicit AnalyzerForm(QWidget *parent = nullptr);
    ~AnalyzerForm();

  private:
    Ui::AnalyzerForm *ui;
};

#endif // ANALYZERFORM_H
