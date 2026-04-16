#include "gainerwidget.h"
#include "ui_gainerwidget.h"

#include <QSignalBlocker>
#include <QTimer>

GainerWidget::GainerWidget(Plugins::IPlugin *plugin, QWidget *parent)
    : IPluginWidget(plugin, parent), ui(new Ui::GainerWidget)
{
    ui->setupUi(this);

    connect(ui->dial, &QDial::valueChanged, this, [this](int value) {
        QSignalBlocker b(ui->spinBox);
        ui->spinBox->setValue(value / 100.0);
        applyToPlugin();
    });

    connect(ui->spinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        QSignalBlocker b(ui->dial);
        ui->dial->setValue(static_cast<int>(value * 100));
        applyToPlugin();
    });

    QTimer::singleShot(0, this, [this]() { updateFromPlugin(); });
}

GainerWidget::~GainerWidget()
{
    delete ui;
}

void GainerWidget::updateFromPlugin()
{
}

void GainerWidget::applyToPlugin()
{
}
