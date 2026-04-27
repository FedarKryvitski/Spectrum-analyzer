#include "gainerwidget.h"
#include "ui_gainerwidget.h"

#include "base/plugin.h"

#include <QSignalBlocker>
#include <QTimer>

GainerWidget::GainerWidget(Plugins::IPlugin *plugin, QWidget *parent)
    : IPluginWidget(plugin, parent), ui(new Ui::GainerWidget)
{
    ui->setupUi(this);

    connect(ui->dial, &QDial::valueChanged, this, [this](int value) {
        gainValue_ = static_cast<double>(value) / 100.0;
        applyToPlugin();
        update();
    });

    connect(ui->spinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        gainValue_ = value;
        applyToPlugin();
        update();
    });

    QTimer::singleShot(0, this, [this]() {
        updateFromPlugin();
        update();
    });
}

GainerWidget::~GainerWidget()
{
    delete ui;
}

void GainerWidget::updateFromPlugin()
{
    const auto settings = plugin_->getSettings();
    gainValue_ = settings["gainLevel"];
}

void GainerWidget::applyToPlugin()
{
    const auto settings = nlohmann::json{{"gainLevel", gainValue_}};

    plugin_->setSettings(settings);
}

void GainerWidget::update()
{
    QSignalBlocker dialBlocker(ui->dial);
    QSignalBlocker spinBoxBlocker(ui->spinBox);

    ui->dial->setValue(static_cast<int>(gainValue_ * 100));
    ui->spinBox->setValue(gainValue_);
}
