#include "limiterwidget.h"

#include "base/plugin.h"

#include <QSignalBlocker>
#include <QTimer>

LimiterWidget::LimiterWidget(Plugins::IPlugin *plugin, QWidget *parent)
    : IPluginWidget(plugin, parent), ui(new Ui::LimiterWidget)
{
    ui->setupUi(this);

    connect(ui->thresholdSlider, &QSlider::valueChanged, this, [this](const int value) {
        thresholdDb_ = value / 10.0;
        applyToPlugin();
        update();
    });

    connect(ui->thresholdSpin, &QDoubleSpinBox::valueChanged, this, [this](const double value) {
        thresholdDb_ = value;
        applyToPlugin();
        update();
    });

    connect(ui->ceilingSlider, &QSlider::valueChanged, this, [this](const int value) {
        ceilingDb_ = value / 10.0;
        applyToPlugin();
        update();
    });

    connect(ui->ceilingSpin, &QDoubleSpinBox::valueChanged, this, [this](const double value) {
        ceilingDb_ = value;
        applyToPlugin();
        update();
    });

    connect(ui->releaseSlider, &QSlider::valueChanged, this, [this](const int value) {
        releaseMs_ = value;
        applyToPlugin();
        update();
    });

    connect(ui->releaseSpin, &QSpinBox::valueChanged, this, [this](const int value) {
        releaseMs_ = value;
        applyToPlugin();
        update();
    });

    QTimer::singleShot(0, this, [this]() {
        updateFromPlugin();
        update();
    });
}

LimiterWidget::~LimiterWidget()
{
    delete ui;
}

void LimiterWidget::updateFromPlugin()
{
    const auto settings = plugin_->getSettings();

    thresholdDb_ = settings["thresholdDb"];
    ceilingDb_ = settings["ceilingDb"];
    releaseMs_ = settings["releaseMs"];
}

void LimiterWidget::applyToPlugin()
{
    const auto settings =
        nlohmann::json{{"thresholdDb", thresholdDb_}, {"ceilingDb", ceilingDb_}, {"releaseMs", releaseMs_}};

    plugin_->setSettings(settings);
}

void LimiterWidget::update()
{
    {
        QSignalBlocker spinBlocker(ui->thresholdSpin);
        QSignalBlocker sliderBlocker(ui->thresholdSlider);

        ui->thresholdSpin->setValue(thresholdDb_);
        ui->thresholdSlider->setValue(static_cast<int>(thresholdDb_ * 10.0));
    }

    {
        QSignalBlocker spinBlocker(ui->ceilingSpin);
        QSignalBlocker sliderBlocker(ui->ceilingSlider);

        ui->ceilingSpin->setValue(ceilingDb_);
        ui->ceilingSlider->setValue(static_cast<int>(ceilingDb_ * 10.0));
    }

    {
        QSignalBlocker spinBlocker(ui->releaseSpin);
        QSignalBlocker sliderBlocker(ui->releaseSlider);

        ui->releaseSpin->setValue(releaseMs_);
        ui->releaseSlider->setValue(releaseMs_);
    }
}
