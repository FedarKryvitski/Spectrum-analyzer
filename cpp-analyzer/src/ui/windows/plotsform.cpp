#include "plotsform.h"
#include "ui_plotsform.h"

PlotsForm::PlotsForm(QWidget *parent) : QWidget(parent), ui(new Ui::PlotsForm)
{
    ui->setupUi(this);
}

PlotsForm::~PlotsForm()
{
    delete ui;
}
