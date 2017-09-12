#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_setcurrent_slider_1_sliderReleased();

    void on_setcurrent_slider_2_sliderReleased();

    void on_checkBox_bat1_charge_clicked();

    void on_checkBox_bat1_enable_clicked();

    void on_checkBox_bat2_charge_clicked();

    void on_checkBox_bat2_enable_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
