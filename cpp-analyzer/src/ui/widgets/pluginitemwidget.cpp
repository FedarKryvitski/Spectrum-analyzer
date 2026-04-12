#include "pluginitemwidget.h"
#include "ui_pluginitemwidget.h"
#include <QMouseEvent>
#include <QDebug>

PluginItemWidget::PluginItemWidget(const QString &name, QWidget *parent) :
      QWidget(parent),
      ui(new Ui::PluginItemWidget)
{
    ui->setupUi(this);
    ui->nameLabel->setText(name);

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

void PluginItemWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this);
    QWidget::mousePressEvent(event);
}
