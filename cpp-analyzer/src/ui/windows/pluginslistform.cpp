#include "pluginslistform.h"
#include "ui_pluginslistform.h"

#include <QAbstractItemView>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMessageBox>

PluginsListForm::PluginsListForm(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::PluginsListForm)
{
    ui->setupUi(this);

    init();

    connect(ui->addItemButton, &QPushButton::clicked,
            this, &PluginsListForm::onAddItemSlot);

    connect(ui->removeItemButton, &QPushButton::clicked,
            this, &PluginsListForm::onRemoveItemSlot);

    connect(ui->backToAnalyzerButton, &QPushButton::clicked,
            this, &PluginsListForm::backRequested);
}

PluginsListForm::~PluginsListForm()
{
    delete ui;
}

void PluginsListForm::init()
{
    ui->editableListWidget->setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::EditKeyPressed
        );
}

void PluginsListForm::onAddItemSlot()
{
    bool ok = false;
    const QString text = QInputDialog::getText(
        this,
        "Add item",
        "Item name:",
        QLineEdit::Normal,
        "",
        &ok);

    if (ok && !text.trimmed().isEmpty())
    {
        auto *item = new QListWidgetItem(text.trimmed());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->editableListWidget->addItem(item);
    }
}

void PluginsListForm::onRemoveItemSlot()
{
    auto *item = ui->editableListWidget->currentItem();

    if (!item)
    {
        QMessageBox::information(this, "Info", "Select an item first.");
        return;
    }

    delete item;
}
