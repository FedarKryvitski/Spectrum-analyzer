#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audiorecorder.h"
#include "audioplayer.h"
#include "frequencyplot.h"
#include "amplitudeplot.h"

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
    void on_buttonStart_clicked();
    void on_buttonStop_clicked();
    void on_comboBox_currentIndexChanged(int index);
    
    void onReadyRead();

private:
    void init();

    void startRecording();
    void stopRecording();

private:
    Ui::MainWindow *ui;
    AudioRecorder recorder_;
    AudioPlayer player_;
    FrequencyPlot frequencyPlot_;
    AmplitudePlot amplitudePlot_;

    QTimer plotTimer_;
    bool isRunning_{ false };
};

#endif // MAINWINDOW_H
