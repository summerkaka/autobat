#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dualcurve.h"
#include "singlecurve.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void HeaterSendNewSetting(uint8_t item, const void *data);
    void HeaterTabWrite(uint8_t item, const void *data);
    void BatterySendNewSetting(uint8_t item, const void *data);
    void BatteryTabWrite(uint8_t bat_index, uint8_t item, const void *data);


private slots:

    void on_setcurrent_slider_1_sliderReleased();

    void on_setcurrent_slider_2_sliderReleased();

    void on_checkBox_bat1_charge_clicked();

    void on_checkBox_bat1_enable_clicked();

    void on_checkBox_bat2_charge_clicked();

    void on_checkBox_bat2_enable_clicked();

    void on_radioButton_heaterOn_clicked();

    void on_radioButton_HeaterOff_clicked();

    void on_lineEdit_heaterSP_returnPressed();

    void on_lineEdit_heaterKp_returnPressed();

    void on_lineEdit_heaterKi_returnPressed();

    void on_lineEdit_heaterKd_returnPressed();

    void on_pushButton_heaterUpLD_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
