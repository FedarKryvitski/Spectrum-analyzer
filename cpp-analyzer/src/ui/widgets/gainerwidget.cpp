#include "gainerwidget.h"
#include "ui_gainerwidget.h"

#include <QSignalBlocker>

GainerWidget::GainerWidget(Plugins::IPlugin* plugin, QWidget* parent)
    : IPluginWidget(plugin, parent)
      , ui(new Ui::GainerWidget)
{
    ui->setupUi(this);

    ui->dial->setRange(0, 200);
    ui->dial->setNotchesVisible(true);

    ui->spinBox->setRange(0.0, 2.0);
    ui->spinBox->setDecimals(2);
    ui->spinBox->setSingleStep(0.01);

    connect(ui->dial, &QDial::valueChanged, this, [this](int value)
            {
                QSignalBlocker b(ui->spinBox);
                ui->spinBox->setValue(value / 100.0);
                applyToPlugin();
            });

    connect(ui->spinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value)
            {
                QSignalBlocker b(ui->dial);
                ui->dial->setValue(static_cast<int>(value * 100));
                applyToPlugin();
            });

    updateFromPlugin();
}

GainerWidget::~GainerWidget()
{
    delete ui;
}
