#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFontComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QProgressBar>
#include <QSpinBox>

#include "myopenglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // Field: Additional UI
    Ui::MainWindow *ui;
    QLabel *fLabCurFile;
    QProgressBar *progressBar1;

private:
    // Function: Additional UI initialize
    void iniUI();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSlider *slider_x;
    QSlider *slider_y;
    QSlider *slider_z;

    void ddebug();

private slots:
    // Slot: Button Interaction
    void on_actStart_triggered();
    void on_actStop_triggered();

    void on_actSlide_X(int x);
    void on_actSlide_Y(int y);
    void on_actSlide_Z(int z);
    void on_actEye_triggered();
};
#endif // MAINWINDOW_H
