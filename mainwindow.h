#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "dualcurve.h"
#include "singlecurve.h"

typedef enum {
    kHeaterNoClick = 0,
    kHeaterOff = 1,
    kHeaterOn = 2,
} radioButtonHeaterStatus;

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

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private slots:

    void on_setcurrent_slider_1_sliderReleased();

    void on_setcurrent_slider_2_sliderReleased();

    void on_checkBox_bat1_charge_clicked(bool checked);

    void on_checkBox_bat1_enable_clicked(bool checked);

    void on_checkBox_bat2_charge_clicked(bool checked);

    void on_checkBox_bat2_enable_clicked(bool checked);

    void on_radioButton_heaterOn_clicked(bool checked);

    void on_radioButton_heaterOff_clicked(bool checked);

    void on_lineEdit_heaterSP_returnPressed();

    void on_lineEdit_heaterKp_returnPressed();

    void on_lineEdit_heaterKi_returnPressed();

    void on_lineEdit_heaterKd_returnPressed();

    void on_radioButton_heaterOn_pressed();

    void on_radioButton_heaterOff_pressed();

private:
    Ui::MainWindow *ui;
    QMessageBox msg;
    void PopConfirm(void);
    void Pop(int value);
    radioButtonHeaterStatus radio_heater;
    uint8_t cfm = 0;
    int timerId_upadteUi = 0;
};

#endif // MAINWINDOW_H
