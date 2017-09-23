#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "can_app.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HeaterTabWrite(uint8_t item, const void *data)
{
    switch (item) {
    case 0x00:  // status
        ui->tableWidget_heater->setItem(0, 0, new QTableWidgetItem(QString::number(*(uint8_t *)data)));
        break;
    case 0x01: // temperatue
        ui->tableWidget_heater->setItem(1, 0, new QTableWidgetItem(QString::number(*(float *)data)));
        break;
    case 0x02: // pt100 adc code
        ui->tableWidget_heater->setItem(2, 0, new QTableWidgetItem(QString::number(*(int32_t *)data, 16)));
        break;
    case 0x03: // duty cycle
        ui->tableWidget_heater->setItem(3, 0, new QTableWidgetItem(QString::number(*(float *)data)));
        break;
    case 0x04: // heater on/off
        if (Heater.status == 0) {
            ui->radioButton_HeaterOff->click();
        }else {
            ui->radioButton_heaterOn->click();
        }
        break;
    case 0x05: // setpoint
        ui->lineEdit_heaterSP->setText(QString::number(int(*(int16_t *)data * 100)));
        break;
    case 0x06: // kp
        ui->lineEdit_heaterKp->setText(QString::number(int(*(int16_t *)data * 100)));
        break;
    case 0x07: // ki
        ui->lineEdit_heaterKi->setText(QString::number(int(*(int16_t *)data * 100)));
        break;
    case 0x08: // kd
        ui->lineEdit_heaterKd->setText(QString::number(int(*(int16_t *)data * 100)));
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
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(float *)data)));
        break;
    case 0x02:  // current
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(float *)data)));
        break;
    case 0x03:  // temperature
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(float *)data)));
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

void MainWindow::on_checkBox_bat1_charge_clicked()
{

}

void MainWindow::on_checkBox_bat1_enable_clicked()
{

}

void MainWindow::on_checkBox_bat2_charge_clicked()
{

}

void MainWindow::on_checkBox_bat2_enable_clicked()
{

}


void MainWindow::on_radioButton_heaterOn_clicked()
{

}

void MainWindow::on_radioButton_HeaterOff_clicked()
{

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
    SendCommand(CANID_CHARGE, CANID_MB, CMD_WR_ZONE, data, 4);
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
    SendCommand(CANID_CHARGE, CANID_MB, CMD_WR_ZONE, data, 8);
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
    SendCommand(CANID_CHARGE, CANID_MB, CMD_WR_ZONE, data, 8);
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
    SendCommand(CANID_CHARGE, CANID_MB, CMD_WR_ZONE, data, 8);
}

void MainWindow::on_pushButton_heaterUpLD_clicked()
{

}
