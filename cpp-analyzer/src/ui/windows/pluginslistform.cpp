#include "pluginslistform.h"
#include "ui_pluginslistform.h"

#include "widgets/pluginitemwidget.h"
#include "controllers/plugincontroller.h"

PluginsListForm::PluginsListForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PluginsListForm)
{
    ui->setupUi(this);

    connect(ui->addItemButton, &QPushButton::clicked, this, &PluginsListForm::onAddItemSlot);
    connect(ui->backToAnalyzerButton, &QPushButton::clicked, this, &PluginsListForm::backRequested);
}

PluginsListForm::~PluginsListForm()
{
    delete ui;
}

void PluginsListForm::setController(PluginController* controller)
{
    controller_ = controller;
    connect(controller_, &PluginController::pipelineChanged, this, &PluginsListForm::updateListView);
    updateListView();
}

void PluginsListForm::updateListView()
{
    if (!controller_) return;

    QLayoutItem* child;
    while ((child = ui->pluginsContainer->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    auto plugins = controller_->getPipeline()->getPlugins();

    for (int i = 0; i < plugins.size(); ++i) {
        auto plugin = plugins[i];
        auto *item = new PluginItemWidget(QString::fromStdString(plugin->getName()), this);

        connect(item, &PluginItemWidget::clicked, this, [this, plugin]() {

        });

        connect(item, &PluginItemWidget::removeRequested, this, [this, i]() {
            controller_->removePlugin(i);
        });

        connect(item, &PluginItemWidget::moveUpRequested, this, [this, i]() {
            controller_->movePlugin(i, i - 1);
        });

        connect(item, &PluginItemWidget::moveDownRequested, this, [this, i]() {
            controller_->movePlugin(i, i + 1);
        });

        ui->pluginsContainer->addWidget(item);
    }

    ui->pluginsContainer->addStretch();
}

void PluginsListForm::showPluginWidget(QWidget* widget)
{
    if (!widget)
        return;

    QLayoutItem* item;
    while ((item = ui->pluginViewLayout->takeAt(0)) != nullptr) {
        if (item->widget())
            item->widget()->deleteLater();

        delete item;
    }

    ui->pluginViewLayout->addWidget(widget);
    ui->stackedWidget->setCurrentWidget(ui->pagePluginView);
}

void PluginsListForm::onAddItemSlot()
{
    if (!controller_)
        return;

    controller_->addPlugin("Gain");
}
