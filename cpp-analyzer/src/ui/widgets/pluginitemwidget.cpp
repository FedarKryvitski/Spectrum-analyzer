#include "pluginitemwidget.h"
#include "ui_pluginitemwidget.h"

#include "base/plugin.h"

#include <QMouseEvent>

PluginItemWidget::PluginItemWidget(Plugins::IPlugin* plugin, QWidget *parent) :
      QWidget(parent),
      ui(new Ui::PluginItemWidget)
{
    ui->setupUi(this);
    ui->nameLabel->setText(QString::fromStdString(plugin->getName()));

    {
        QSignalBlocker blocker(ui->powerButton);

        const bool enabled = plugin->isEnabled();
        ui->powerButton->setChecked(enabled);
        setPluginEnabled(enabled);
    }

    connect(ui->powerButton, &QPushButton::toggled, this, [this](bool checked) {
        setPluginEnabled(checked);
        emit toggled(this, checked);
    });

    connect(ui->upButton, &QPushButton::clicked, this, [this]() {
        emit moveUpRequested(this);
    });

    connect(ui->downButton, &QPushButton::clicked, this, [this]() {
        emit moveDownRequested(this);
    });

    connect(ui->removeButton, &QPushButton::clicked, this, [this]() {
        emit removeRequested(this);
    });
}

PluginItemWidget::~PluginItemWidget()
{
    delete ui;
}

QString PluginItemWidget::getName() const
{
    return ui->nameLabel->text();
}

bool PluginItemWidget::isPluginEnabled() const
{
    return ui->powerButton->isChecked();
}

void PluginItemWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this);
    QWidget::mousePressEvent(event);
}

void PluginItemWidget::setPluginEnabled(const bool enabled)
{
    ui->nameLabel->setEnabled(enabled);
}
