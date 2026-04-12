#include "pluginslistform.h"
#include "ui_pluginslistform.h"

#include "widgets/pluginitemwidget.h"

// TODO refactoring
#include "effects/gainer.h"
#include "widgets/gainerwidget.h"

PluginsListForm::PluginsListForm(QWidget *parent) : QWidget(parent), ui(new Ui::PluginsListForm)
{
    ui->setupUi(this);

    init();

    connect(ui->addItemButton, &QPushButton::clicked, this, &PluginsListForm::onAddItemSlot);
    connect(ui->backToAnalyzerButton, &QPushButton::clicked, this, &PluginsListForm::backRequested);
}

PluginsListForm::~PluginsListForm()
{
    delete ui;
}

void PluginsListForm::init()
{

}

void PluginsListForm::addNewPlugin(const QString &pluginName)
{
    auto *item = new PluginItemWidget(pluginName, this);

    connect(item, &PluginItemWidget::clicked, this,
        [this](PluginItemWidget* ptr) {

        qDebug() << "Open plugin UI for" << ptr->getName();
        auto* plugin = new Plugins::Gainer();
        auto* widget = new GainerWidget(plugin, ui->pagePluginView);

        showPluginWidget(widget);
    });

    connect(item, &PluginItemWidget::removeRequested, this, [this](PluginItemWidget* ptr){
        ui->pluginsContainer->removeWidget(ptr);
        ptr->deleteLater();
    });

    connect(item, &PluginItemWidget::moveUpRequested, this, [this](PluginItemWidget* ptr){
        int index = ui->pluginsContainer->indexOf(ptr);
        if (index > 0) {
            ui->pluginsContainer->removeWidget(ptr);
            ui->pluginsContainer->insertWidget(index - 1, ptr);
        }
    });

    connect(item, &PluginItemWidget::moveDownRequested, this, [this](PluginItemWidget* ptr){
        int index = ui->pluginsContainer->indexOf(ptr);
        if (index < ui->pluginsContainer->count() - 2) {
            ui->pluginsContainer->removeWidget(ptr);
            ui->pluginsContainer->insertWidget(index + 1, ptr);
        }
    });

    ui->pluginsContainer->insertWidget(ui->pluginsContainer->count() - 1, item);
}

void PluginsListForm::showPluginWidget(QWidget* widget)
{
    QLayoutItem* item;
    while ((item = ui->pluginViewLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    ui->pluginViewLayout->addWidget(widget);
    ui->stackedWidget->setCurrentWidget(ui->pagePluginView);
}

void PluginsListForm::onAddItemSlot()
{
    addNewPlugin("CCCC");
    addNewPlugin("BBBB");
    addNewPlugin("AAAAA");
}
