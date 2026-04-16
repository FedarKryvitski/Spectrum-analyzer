#include "pluginslistform.h"
#include "ui_pluginslistform.h"

#include "controllers/plugincontroller.h"
#include "widgets/pluginitemwidget.h"
#include "widgets/pluginselector.h"
#include "widgets/pluginwidgetfactory.h"

PluginsListForm::PluginsListForm(QWidget *parent) : QWidget(parent), ui(new Ui::PluginsListForm)
{
    ui->setupUi(this);

    connect(ui->addItemButton, &QPushButton::clicked, this, &PluginsListForm::onAddItemSlot);
    connect(ui->backToAnalyzerButton, &QPushButton::clicked, this, &PluginsListForm::backRequested);
}

PluginsListForm::~PluginsListForm()
{
    delete ui;
}

void PluginsListForm::setController(PluginController *controller)
{
    controller_ = controller;
    connect(controller_, &PluginController::pipelineChanged, this, &PluginsListForm::updateListView);
    updateListView();
}

void PluginsListForm::clearLayout()
{
    QLayoutItem *child;
    while ((child = ui->pluginsContainer->takeAt(0)) != nullptr)
    {
        if (child->widget())
        {
            child->widget()->deleteLater();
        }
        delete child;
    }
}

void PluginsListForm::updateListView()
{
    if (!controller_)
        return;

    clearLayout();

    auto plugins = controller_->getPipeline()->getPlugins();

    for (int index = 0; index < plugins.size(); ++index)
    {
        auto plugin = plugins[index];
        auto item = createItem(plugin.get(), index);

        ui->pluginsContainer->addWidget(item);
    }

    ui->pluginsContainer->addStretch();
}

void PluginsListForm::onAddItemSlot()
{
    if (!controller_)
        return;

    PluginSelector selector(this);

    QMap<QString, QString> available;
    for (auto &name : controller_->getAvailablePlugins())
    {
        available.insert(name, ":/icons/default_fx.png");
    }
    selector.setPlugins(available);

    connect(&selector, &PluginSelector::pluginSelected, this,
            [this](const QString &name) { controller_->addPlugin(name); });

    selector.exec();
}

PluginItemWidget *PluginsListForm::createItem(Plugins::IPlugin *plugin, const int index)
{
    auto *item = new PluginItemWidget(plugin, this);

    connect(item, &PluginItemWidget::pluginClicked, this, [this, plugin]() { openPluginDialog(plugin); });

    connect(item, &PluginItemWidget::enabledToggled, this,
            [this, index](PluginItemWidget *item, bool enabled) { controller_->togglePlugin(index, enabled); });

    connect(item, &PluginItemWidget::removeRequested, this,
            [this, index](PluginItemWidget *item) { controller_->removePlugin(index); });

    connect(item, &PluginItemWidget::moveUpRequested, this,
            [this, index](PluginItemWidget *item) { controller_->movePlugin(index, index - 1); });

    connect(item, &PluginItemWidget::moveDownRequested, this,
            [this, index](PluginItemWidget *item) { controller_->movePlugin(index, index + 1); });

    return item;
}

void PluginsListForm::openPluginDialog(Plugins::IPlugin *plugin)
{
    auto *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(QString::fromStdString(plugin->getName()));

    auto *layout = new QVBoxLayout();
    dialog->setLayout(layout);

    auto *widget = Plugins::createPluginWidget(plugin, dialog);
    if (!widget)
    {
        dialog->deleteLater();
        return;
    }

    layout->addWidget(widget);

    dialog->resize(300, 250);
    dialog->show();
}
