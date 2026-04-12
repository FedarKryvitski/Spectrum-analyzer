#pragma once

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class PluginSelector;
}

class PluginSelector : public QDialog
{
    Q_OBJECT

public:
    explicit PluginSelector(QWidget *parent = nullptr);
    ~PluginSelector();

    void setPlugins(const QMap<QString, QString> &plugins);

signals:
    void pluginSelected(const QString &pluginName);

private:
    void connectUi();

private:
    Ui::PluginSelector *ui;
};
