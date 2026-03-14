#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audiorecorder.h"
#include "audioplayer.h"
#include "widgets/frequencyplot.h"
#include "widgets/amplitudeplot.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartSlot();

    void onStopSlot();

    void onDeviceChangedSlot(const QString& device);

    void onDataSlot();

private:
    void init();

private:
    Ui::MainWindow *ui;

    AudioRecorder recorder_;
    Alsa::AudioPlayer player_;

    std::unique_ptr<FrequencyPlot> frequencyPlot_;
    std::unique_ptr<AmplitudePlot> amplitudePlot_;

    QTimer plotTimer_;

    bool isRunning_{false};
};

#endif // MAINWINDOW_H
