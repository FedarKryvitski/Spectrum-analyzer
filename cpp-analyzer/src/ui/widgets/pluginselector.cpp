#include "pluginselector.h"
#include "ui_pluginselector.h"

PluginSelector::PluginSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginSelector)
{
    ui->setupUi(this);

    ui->searchEdit->setFocus();
    ui->okButton->setEnabled(false);

    connectUi();
}

PluginSelector::~PluginSelector()
{
    delete ui;
}

void PluginSelector::connectUi()
{
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->okButton, &QPushButton::clicked, this, [this]() {
        if (auto *item = ui->pluginList->currentItem()) {
            emit pluginSelected(item->text());
            accept();
        }
    });

    connect(ui->pluginList, &QListWidget::itemSelectionChanged, this, [this]() {
        ui->okButton->setEnabled(!ui->pluginList->selectedItems().isEmpty());
    });

    connect(ui->pluginList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
        if (item) {
            emit pluginSelected(item->text());
            accept();
        }
    });

    connect(ui->searchEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        for (int i = 0; i < ui->pluginList->count(); ++i) {
            auto *item = ui->pluginList->item(i);
            bool matches = item->text().contains(text, Qt::CaseInsensitive);
            item->setHidden(!matches);

            if (!matches && item->isSelected()) {
                item->setSelected(false);
            }
        }
    });
}

void PluginSelector::setPlugins(const QMap<QString, QString> &plugins)
{
    ui->pluginList->clear();

    for (auto it = plugins.begin(); it != plugins.end(); ++it) {
        const QString &pluginName = it.key();
        const QString &iconPath = it.value();

        QIcon icon(iconPath);
        if (icon.isNull()) {
            icon = qApp->style()->standardIcon(QStyle::SP_FileIcon);
        }

        auto *item = new QListWidgetItem(icon, pluginName);
        item->setTextAlignment(Qt::AlignCenter);
        ui->pluginList->addItem(item);
    }

    ui->searchEdit->clear();
}
