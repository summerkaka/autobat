#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include "dualcurve.h"
#include "singlecurve.h"
#include "app.h"
#include "can_if.h"
#include "main.h"
#include "RT_CAN.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void HeaterTabWrite(uint8_t item, const void *data);
    void BatteryTabWrite(uint8_t bat_index, uint8_t item, const void *data);

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

    void on_spinBox_COMNum_valueChanged(int arg1);

    void on_pushButton_OpenCAN_clicked();

    void on_pushButton_CloseCAN_clicked();



protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;
    QMessageBox msg;
    bool com_isopen = false;
    int16_t com_num = 0;
    RT_CAN can_port;
    void PopConfirm(void);
    void Pop(const char *buf, int16_t timeout);
    uint8_t radio_heater, cfm = 0;
    int timerId;
    Thread_Listen *t_listen;
    Thread_Poll *t_poll;
    Thread_Dummy *t_dummy;
};

#endif // MAINWINDOW_H
