#include "limiterwidget.h"

#include <QSignalBlocker>
#include <QTimer>

LimiterWidget::LimiterWidget(Plugins::IPlugin *plugin, QWidget *parent)
    : IPluginWidget(plugin, parent), ui(new Ui::LimiterWidget)
{
    ui->setupUi(this);

    connect(ui->thresholdSpin, &QDoubleSpinBox::valueChanged, this, [this](double) { applyToPlugin(); });

    connect(ui->ceilingSpin, &QDoubleSpinBox::valueChanged, this, [this](double) { applyToPlugin(); });

    connect(ui->releaseSpin, &QSpinBox::valueChanged, this, [this](int) { applyToPlugin(); });

    QTimer::singleShot(0, this, [this]() { updateFromPlugin(); });
}

LimiterWidget::~LimiterWidget()
{
    delete ui;
}

void LimiterWidget::updateFromPlugin()
{
}

void LimiterWidget::applyToPlugin()
{
}
