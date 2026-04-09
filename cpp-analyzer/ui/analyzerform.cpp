#include "analyzerform.h"
#include "ui_analyzerform.h"

AnalyzerForm::AnalyzerForm(QWidget *parent) : QWidget(parent), ui(new Ui::AnalyzerForm)
{
    ui->setupUi(this);
}

AnalyzerForm::~AnalyzerForm()
{
    delete ui;
}
