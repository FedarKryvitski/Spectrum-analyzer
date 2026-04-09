#include "pluginslistform.h"
#include "ui_pluginslistform.h"

#include <QAbstractItemView>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMessageBox>

PluginsListForm::PluginsListForm(QWidget *parent) : QWidget(parent), ui(new Ui::PluginsListForm)
{
    ui->setupUi(this);

    init();

    connect(ui->addItemButton, &QPushButton::clicked, this, &PluginsListForm::onAddItemSlot);
    connect(ui->removeItemButton, &QPushButton::clicked, this, &PluginsListForm::onRemoveItemSlot);
    connect(ui->backToAnalyzerButton, &QPushButton::clicked, this, &PluginsListForm::backRequested);
}

PluginsListForm::~PluginsListForm()
{
    delete ui;
}

void PluginsListForm::init()
{

}

void PluginsListForm::onAddItemSlot()
{

}

void PluginsListForm::onRemoveItemSlot()
{

}
