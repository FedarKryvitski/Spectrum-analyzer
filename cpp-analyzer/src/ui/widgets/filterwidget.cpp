#include "filterwidget.h"

#include "base/plugin.h"

#include <QSignalBlocker>
#include <QTimer>

FilterWidget::FilterWidget(Plugins::IPlugin *plugin, QWidget *parent)
    : IPluginWidget(plugin, parent), ui(new Ui::FilterWidget)
{
    ui->setupUi(this);

    connect(ui->lowCutoffSlider, &QSlider::valueChanged, this, [this](const int value) {
        lowCutoffFrequency_ = 20.0 * pow(1000.0, value / 1000.0);
        applyToPlugin();
        update();
    });

    connect(ui->lowCutoffSpin, &QDoubleSpinBox::valueChanged, this, [this](const double value) {
        lowCutoffFrequency_ = value;
        applyToPlugin();
        update();
    });

    connect(ui->highCutoffSlider, &QSlider::valueChanged, this, [this](const int value) {
        highCutoffFrequency_ = 20.0 * pow(1000.0, value / 1000.0);
        applyToPlugin();
        update();
    });

    connect(ui->highCutoffSpin, &QDoubleSpinBox::valueChanged, this, [this](const double value) {
        highCutoffFrequency_ = value;
        applyToPlugin();
        update();
    });

    QTimer::singleShot(0, this, [this]() {
        updateFromPlugin();
        update();
    });
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::updateFromPlugin()
{
    const auto settings = plugin_->getSettings();

    lowCutoffFrequency_ = settings["lowCutoffFrequency"];
    highCutoffFrequency_ = settings["highCutoffFrequency"];
}

void FilterWidget::applyToPlugin()
{
    const auto settings = nlohmann::json{
        {"lowCutoffFrequency", lowCutoffFrequency_},
        {"highCutoffFrequency", highCutoffFrequency_},
    };

    plugin_->setSettings(settings);
}

void FilterWidget::update()
{
    const auto calculateSliderPosition = [](const double hz) -> int {
        int sliderPosition = static_cast<int>(log10(hz / 20.0) / log10(1000.0) * 1000.0);
        sliderPosition = std::clamp(sliderPosition, 0, 1000);

        return sliderPosition;
    };

    {
        QSignalBlocker spinBlocker(ui->highCutoffSpin);
        QSignalBlocker sliderBlocker(ui->highCutoffSlider);

        ui->highCutoffSpin->setValue(highCutoffFrequency_);
        ui->highCutoffSlider->setValue(calculateSliderPosition(highCutoffFrequency_));
    }

    {
        QSignalBlocker spinBlocker(ui->lowCutoffSpin);
        QSignalBlocker sliderBlocker(ui->lowCutoffSlider);

        ui->lowCutoffSpin->setValue(lowCutoffFrequency_);
        ui->lowCutoffSlider->setValue(calculateSliderPosition(lowCutoffFrequency_));
    }
}
