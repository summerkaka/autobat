#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include "dualcurve.h"
#include "singlecurve.h"
#include "app_include.h"

namespace Ui {
class MainWindow;
}

typedef enum {
    kBool = 1,
    kUint8 = 2,
    kUint16 = 3,
    kInt16 = 4,
    kUint32 = 5,
    kFloat = 6
} eType;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_setcurrent_slider_1_sliderReleased();

    void on_setcurrent_slider_2_sliderReleased();

    void on_checkBox_bat1_charge_clicked(bool checked);

    void on_checkBox_bat1_enable_clicked(bool checked);

    void on_checkBox_bat2_charge_clicked(bool checked);

    void on_checkBox_bat2_enable_clicked(bool checked);

    void on_lineEdit_heaterSP_returnPressed();

    void on_lineEdit_heaterKp_returnPressed();

    void on_lineEdit_heaterKi_returnPressed();

    void on_lineEdit_heaterKd_returnPressed();

    void on_pushButton_OpenCAN_clicked();

    void on_pushButton_CloseCAN_clicked();

    void on_checkBox_fan_clicked(bool checked);

    void on_checkBox_valve1_clicked(bool checked);

    void on_checkBox_valve2_clicked(bool checked);

    void on_comboBox_heater_activated(const QString &arg1);

    void on_spinBox_COMNum_editingFinished();

    void on_pushButton_reset1_clicked();

    void on_pushButton_reset2_clicked();

    void on_pushButton_reset3_clicked();

    void on_pushButton_reset4_clicked();

    void on_lineEdit_fwPath_editingFinished();

    void on_pushButton_fwPath_clicked();

    void on_pushButton_fwUpdate_clicked();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;
    QMessageBox msg;
    Thread_Poll *t_poll;

    int timerId;
    int timerId_curveSample;

    QString fwPath;

    void Pop(const char *buf, int16_t timeout);
    void BatteryTabWrite(uint8_t bat_index, uint8_t item, const void *data, eType format);
    void batUpdate(stBattery *bat);
    void heaterUpdate(void);
};

#endif // MAINWINDOW_H
