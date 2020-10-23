#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include "can_if.h"

const QString answer1 = "yes, my master.";
const QString answer2 = "job's done.";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timerId = startTimer(1000);
    ui->rtcurve_bat_v->Config(&Battery_1.voltage, &Battery_2.voltage, "voltage", 2);
    ui->rtcurve_bat_t->Config(&Battery_1.temperature, &Battery_2.temperature, "temperature", 2);
    ui->rtcurve_heater_t->Config(&Heater.temperature, "heater temperature", 2);
    t_listen = new Thread_Listen;
    t_poll = new Thread_Poll;
    t_dummy = new Thread_Dummy;
    t_dummy->start();
}

MainWindow::~MainWindow()
{
//    t_dummy.quit();
//    t_listen.quit();
//    t_poll.quit();
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        uint8_t value = 0;

        // battery 1
        BatteryTabWrite(0, 0, &Battery_1.status);
        BatteryTabWrite(0, 1, &Battery_1.voltage);
        BatteryTabWrite(0, 2, &Battery_1.current);
        BatteryTabWrite(0, 3, &Battery_1.temperature);
        BatteryTabWrite(0, 4, &Battery_1.level);
        BatteryTabWrite(0, 5, &Battery_1.capacity);
        BatteryTabWrite(0, 6, &Battery_1.err_code);
        if (Battery_1.status >= 0x03 && Battery_1.status <= 0x05)
            value = 1;
        else
            value = 0;
        BatteryTabWrite(0, 7, &value);
        BatteryTabWrite(0, 8, &Battery_1.mux_on);

        // battery 2
        BatteryTabWrite(1, 0, &Battery_2.status);
        BatteryTabWrite(1, 1, &Battery_2.voltage);
        BatteryTabWrite(1, 2, &Battery_2.current);
        BatteryTabWrite(1, 3, &Battery_2.temperature);
        BatteryTabWrite(1, 4, &Battery_2.level);
        BatteryTabWrite(1, 5, &Battery_2.capacity);
        BatteryTabWrite(1, 6, &Battery_2.err_code);
        if (Battery_2.status >= 0x03 && Battery_2.status <= 0x05)
            value = 1;
        else
            value = 0;
        BatteryTabWrite(1, 7, &value);
        BatteryTabWrite(1, 8, &Battery_2.mux_on);

        // heater
        HeaterTabWrite(0, &Heater.status);
        HeaterTabWrite(1, &Heater.temperature);
        HeaterTabWrite(2, &Heater.pt100_adccode);
        HeaterTabWrite(3, &Heater.duty);
//        HeaterTabWrite(4, &Heater.status);
        HeaterTabWrite(5, &Heater.setpoint);
        HeaterTabWrite(6, &Heater.kp);
        HeaterTabWrite(7, &Heater.ki);
        HeaterTabWrite(8, &Heater.kd);

        printf("updateui\n");
    }
}

void MainWindow::PopConfirm(void)
{
    if (cfm == 0) {
        cfm = 1;
        msg.setText(answer1);
        msg.exec();
    }else {
        cfm = 0;
        msg.setText(answer2);
        msg.exec();
    }
}

void MainWindow::Pop(const char *buf, int16_t timeout)
{
    msg.setText(buf);
    QTimer::singleShot(timeout, &msg, &QMessageBox::close);
    msg.exec();
}

void MainWindow::HeaterTabWrite(uint8_t item, const void *data)
{
    switch (item) {
    case 0x00: // status
        ui->tableWidget_heater->setItem(0, 0, new QTableWidgetItem(QString::number(*(uint8_t *)data)));
        break;
    case 0x01: // temperatue
        ui->tableWidget_heater->setItem(1, 0, new QTableWidgetItem(QString::number(*(float *)data, 'f', 4)));
        break;
    case 0x02: // pt100 adc code
        ui->tableWidget_heater->setItem(2, 0, new QTableWidgetItem(QString::number(*(int32_t *)data, 16)));
        break;
    case 0x03: // duty cycle
        ui->tableWidget_heater->setItem(3, 0, new QTableWidgetItem(QString::number(*(float *)data, 'f', 4)));
        break;
    case 0x04: // heater on/off
        if (*(uint8_t *)data == 0) {
            ui->radioButton_heaterOff->click();
        }else {
            ui->radioButton_heaterOn->click();
        }
        break;
    case 0x05: // setpoint
        ui->lineEdit_heaterSP->setText(QString::number(*(float *)data, 'f', 4));
        break;
    case 0x06: // kp
        ui->lineEdit_heaterKp->setText(QString::number(*(int16_t *)data));
        break;
    case 0x07: // ki
        ui->lineEdit_heaterKi->setText(QString::number(*(int16_t *)data));
        break;
    case 0x08: // kd
        ui->lineEdit_heaterKd->setText(QString::number(*(int16_t *)data));
        break;
    default : break;
    }
}

void MainWindow::BatteryTabWrite(uint8_t bat_index, uint8_t item, const void *data)
{
    if (bat_index != 0x00 && bat_index != 0x01)
        return;

    switch (item) {
    case 0x00:  // status
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(uint8_t *)data)));
        break;
    case 0x01:  // voltage
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(float *)data, 'f', 4)));
        break;
    case 0x02:  // current
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(float *)data, 'f', 4)));
        break;
    case 0x03:  // temperature
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(float *)data, 'f', 4)));
        break;
    case 0x04:  // level
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(int16_t *)data)));
        break;
    case 0x05:  // capacity
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(int16_t *)data)));
        break;
    case 0x06:  // error code
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(uint8_t *)data)));
        break;
    case 0x07:  // charge switch
        if (bat_index == 0x00)
            ui->checkBox_bat1_charge->setChecked(*(bool *)data);
        else if (bat_index == 0x01)
            ui->checkBox_bat2_charge->setChecked(*(bool *)data);
        break;
    case 0x08:  // enable switch
        if (bat_index == 0x00)
            ui->checkBox_bat1_enable->setChecked(*(bool *)data);
        else if (bat_index == 0x01)
            ui->checkBox_bat2_enable->setChecked(*(bool *)data);
        break;
    default : break;
    }
}

void MainWindow::on_setcurrent_slider_1_sliderReleased()
{

}

void MainWindow::on_setcurrent_slider_2_sliderReleased()
{

}

void MainWindow::on_checkBox_bat1_charge_clicked(bool checked)
{
    uint8_t value = ui->checkBox_bat1_charge->isChecked() ? DIO_BAT1_CHARGE : 0x00;
    uint8_t data[8] = {BLOCK_A, 0x00, DIO_BAT1_CHARGE, 0x00, value};
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WRITE_DIO, data, 5) == 0)
        Pop("succeed", 1000);
    else {
        ui->checkBox_bat1_charge->toggle();
    }
}

void MainWindow::on_checkBox_bat1_enable_clicked(bool checked)
{
    uint8_t value = ui->checkBox_bat1_enable->isChecked() ? DIO_BAT1_SUPPLY : 0x00;
    uint8_t data[8] = {BLOCK_A, 0x00, DIO_BAT1_SUPPLY, 0x00, value};
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WRITE_DIO, data, 5) == 0)
        Pop("succeed", 1000);
    else {
        ui->checkBox_bat1_enable->toggle();
    }
}

void MainWindow::on_checkBox_bat2_charge_clicked(bool checked)
{
    uint8_t value = ui->checkBox_bat2_charge->isChecked() ? DIO_BAT2_CHARGE : 0x00;
    uint8_t data[8] = {BLOCK_A, 0x00, DIO_BAT2_CHARGE, 0x00, value};
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WRITE_DIO, data, 5) == 0)
        Pop("succeed", 1000);
    else {
        ui->checkBox_bat2_charge->toggle();
    }
}

void MainWindow::on_checkBox_bat2_enable_clicked(bool checked)
{
    uint8_t value = ui->checkBox_bat2_enable->isChecked() ? DIO_BAT2_SUPPLY : 0x00;
    uint8_t data[8] = {BLOCK_A, 0x00, DIO_BAT2_SUPPLY, 0x00, value};
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WRITE_DIO, data, 5) == 0)
        Pop("succeed", 1000);
    else {
        ui->checkBox_bat2_enable->toggle();
    }
}

void MainWindow::on_radioButton_heaterOn_pressed()
{
    if (ui->radioButton_heaterOn->isChecked())
        radio_heater = 2;
    else if (ui->radioButton_heaterOff->isChecked())
        radio_heater = 1;
    else if (ui->radioButton_heaterOff->isChecked() && ui->radioButton_heaterOn->isChecked())
        radio_heater = 0;
}

void MainWindow::on_radioButton_heaterOff_pressed()
{
    if (ui->radioButton_heaterOn->isChecked())
        radio_heater = 2;
    else if (ui->radioButton_heaterOff->isChecked())
        radio_heater = 1;
    else if (ui->radioButton_heaterOff->isChecked() && ui->radioButton_heaterOn->isChecked())
        radio_heater = 0;
}

void MainWindow::on_radioButton_heaterOn_clicked(bool checked)
{
    uint8_t data[8] = {BLOCK_A, 0x00, DIO_HEATER, 0x00, DIO_HEATER};
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WRITE_DIO, data, 5) == 0) {
        Pop("succeed", 1000);
        radio_heater = 2;
    }else {
        if (radio_heater == 1)
            ui->radioButton_heaterOff->setChecked(true);
    }
}

void MainWindow::on_radioButton_heaterOff_clicked(bool checked)
{
    uint8_t data[8] = {BLOCK_A, 0x00, DIO_HEATER, 0x00, 0x00};
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WRITE_DIO, data, 5) == 0) {
        Pop("succeed", 1000);
        radio_heater = 1;
    }else {
        if (radio_heater == 2)
            ui->radioButton_heaterOn->setChecked(true);
    }
}

void MainWindow::on_lineEdit_heaterSP_returnPressed()
{
    bool ok = false;
    uint8_t data[8] = {0};
    Heater.setpoint = ui->lineEdit_heaterSP->text().toFloat(&ok);
    int16_t value = Heater.setpoint * 100;
    data[0] = 0x03;
    data[1] = 0x03;
    WriteWordL(&data[2], value);
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WR_ZONE, data, 4) == 0)
        Pop("succeed", 1000);
}

void MainWindow::on_lineEdit_heaterKp_returnPressed()
{
    bool ok = false;
    uint8_t data[8] = {0};
    Heater.kp = ui->lineEdit_heaterKp->text().toInt(&ok, 10);
    data[0] = 0x03;
    data[1] = 0x05;
    WriteWordL(&data[2], Heater.kp);
    WriteWordL(&data[4], Heater.ki);
    WriteWordL(&data[6], Heater.kd);
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WR_ZONE, data, 8) == 0)
        Pop("succeed", 1000);
}

void MainWindow::on_lineEdit_heaterKi_returnPressed()
{
    bool ok = false;
    uint8_t data[8] = {0};
    Heater.ki = ui->lineEdit_heaterKi->text().toInt(&ok, 10);
    data[0] = 0x03;
    data[1] = 0x05;
    WriteWordL(&data[2], Heater.kp);
    WriteWordL(&data[4], Heater.ki);
    WriteWordL(&data[6], Heater.kd);
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WR_ZONE, data, 8) == 0)
        Pop("succeed", 1000);
}

void MainWindow::on_lineEdit_heaterKd_returnPressed()
{
    bool ok = false;
    uint8_t data[8] = {0};
    Heater.kd = ui->lineEdit_heaterKd->text().toInt(&ok, 10);
    data[0] = 0x03;
    data[1] = 0x05;
    WriteWordL(&data[2], Heater.kp);
    WriteWordL(&data[4], Heater.ki);
    WriteWordL(&data[6], Heater.kd);
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WR_ZONE, data, 8) == 0)
        Pop("succeed", 1000);
}

void MainWindow::on_spinBox_COMNum_valueChanged(int arg1)
{
    com_num = arg1;
}

void MainWindow::on_pushButton_OpenCAN_clicked()
{
    char buf[64];
    if (can_port.Open(com_num, 115200)) {
        sprintf_s(buf, "COM port %d open succeed\n", com_num);
        msg.setText(buf);
        QTimer::singleShot(1500, &msg, &QMessageBox::close);
        msg.exec();
    }else {
        com_isopen = true;
        sprintf_s(buf, "COM port %d open fail\n", com_num);
        msg.setText(buf);
        QTimer::singleShot(1500, &msg, &QMessageBox::close);
        msg.exec();
        t_listen->start();
        t_poll->start();
    }
}

void MainWindow::on_pushButton_CloseCAN_clicked()
{
    char buf[64];
    can_port.Close();
    com_isopen = false;
    sprintf_s(buf, "COM port %d is closed\n", com_num);
    msg.setText(buf);
    QTimer::singleShot(1500, &msg, &QMessageBox::close);
    msg.exec();
    t_poll->quit();
    t_listen->quit();
}

