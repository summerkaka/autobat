#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>

#define UI_REFRESH_MS   100
#define CURVE_SAMPLE_MS 2000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int8_t i = 0;
    ui->setupUi(this);
    for (i=0; i<ui->tableWidget_bat->rowCount(); i++)
        ui->tableWidget_bat->resizeRowToContents(i);
    timerId = startTimer(UI_REFRESH_MS);
    timerId_curveSample = startTimer(CURVE_SAMPLE_MS);
    ui->rtcurve_bat1_v->SetTitle("bat 1 v");
    ui->rtcurve_bat1_t->SetTitle("bat 1 t");
    ui->rtcurve_bat2_v->SetTitle("bat 2 v");
    ui->rtcurve_bat2_t->SetTitle("bat 2 t");
    ui->rtcurve_heater_t->SetTitle("heater t");
    ui->checkBox_adaptor->setDisabled(true);
    ui->checkBox_powerButton->setDisabled(true);
    ui->checkBox_lid->setDisabled(true);
    t_poll = new Thread_Poll;
}

MainWindow::~MainWindow()
{
//    t_dummy.quit();   no need
//    t_listen.quit();
//    t_poll.quit();
    delete ui;
}

void MainWindow::Pop(const char *buf, int16_t timeout)
{
    QString string(buf);
    msg.setText(string);
    QTimer::singleShot(timeout, &msg, &QMessageBox::close);
    msg.exec();
}

void MainWindow::BatteryTabWrite(uint8_t bat_index, uint8_t item, const void *data, eType format)
{
    if (bat_index != 0x00 && bat_index != 0x01)
        return;

    switch(format) {
    case kUint8:
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(uint8_t *)data)));
        break;
    case kUint16:
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(uint16_t *)data)));
        break;
    case kInt16:
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(int16_t *)data)));
        break;
    case kUint32:
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(uint32_t *)data)));
        break;
    case kFloat:
        ui->tableWidget_bat->setItem(item, bat_index, new QTableWidgetItem(QString::number(*(float *)data, 'f', 4)));
        break;
    default: break;
    }

    if (item == 0x06) {
        if (bat_index == 0x00)
            ui->checkBox_bat1_enable->setChecked(*(bool *)data);
        else if (bat_index == 0x01)
            ui->checkBox_bat2_enable->setChecked(*(bool *)data);
    }
    if (item == 0x13) {
        if (bat_index == 0x00)
            ui->checkBox_bat1_charge->setChecked(*(bool *)data);
        else if (bat_index == 0x01)
            ui->checkBox_bat2_charge->setChecked(*(bool *)data);
    }
}

void MainWindow::batUpdate(stBattery *bat)
{
    BatteryTabWrite(bat->index, 0, &bat->status, kUint8);
    BatteryTabWrite(bat->index, 1, &bat->voltage, kFloat);
    BatteryTabWrite(bat->index, 2, &bat->current, kFloat);
    BatteryTabWrite(bat->index, 3, &bat->temperature, kFloat);
    BatteryTabWrite(bat->index, 4, &bat->level, kFloat);
    BatteryTabWrite(bat->index, 5, &bat->capacity, kFloat);
    BatteryTabWrite(bat->index, 6, &bat->mux_on, kUint8);
    BatteryTabWrite(bat->index, 7, &bat->remain_time, kFloat);
    BatteryTabWrite(bat->index, 8, &bat->charge_current, kFloat);
    BatteryTabWrite(bat->index, 9, &bat->fastcharge_timer, kUint32);
    BatteryTabWrite(bat->index, 10, &bat->pre_start_time, kUint32);
    BatteryTabWrite(bat->index, 11, &bat->fast_start_time, kUint32);
    BatteryTabWrite(bat->index, 12, &bat->trickle_start_time, kUint32);
    BatteryTabWrite(bat->index, 13, &bat->last_charge_time, kUint32);
    BatteryTabWrite(bat->index, 14, &bat->charge_times, kUint16);
    BatteryTabWrite(bat->index, 15, &bat->scale_flag, kUint8);
    BatteryTabWrite(bat->index, 16, &bat->is_aged, kUint8);
    BatteryTabWrite(bat->index, 17, &bat->ocv, kFloat);
    BatteryTabWrite(bat->index, 18, &bat->impedance, kFloat);
    BatteryTabWrite(bat->index, 19, &bat->charge_en, kUint8);
    BatteryTabWrite(bat->index, ui->tableWidget_bat->rowCount()-1, &bat->err_code, kUint8);
}

void MainWindow::heaterUpdate(void)
{
    // status
    ui->tableWidget_heater->setItem(0, 0, new QTableWidgetItem(QString::number((uint8_t)Heater.status)));
    // temperature
    ui->tableWidget_heater->setItem(1, 0, new QTableWidgetItem(QString::number((float)Heater.temperature, 'f', 4)));
    // pt100 adc code
    ui->tableWidget_heater->setItem(2, 0, new QTableWidgetItem(QString::number((int32_t)Heater.pt100_adccode, 16)));
    // duty cycle
    ui->tableWidget_heater->setItem(3, 0, new QTableWidgetItem(QString::number((float)Heater.duty, 'f', 4)));
    // setpoint
    ui->tableWidget_heater->setItem(4, 0, new QTableWidgetItem(QString::number((float)Heater.setpoint, 'f', 4)));
    // kp
    ui->tableWidget_heater->setItem(5, 0, new QTableWidgetItem(QString::number((int16_t)Heater.kp, 10)));
    // ki
    ui->tableWidget_heater->setItem(6, 0, new QTableWidgetItem(QString::number((int16_t)Heater.ki, 10)));
    // kd
    ui->tableWidget_heater->setItem(7, 0, new QTableWidgetItem(QString::number((int16_t)Heater.kd, 10)));
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId)
    {
        // battery 1
        batUpdate(&Battery_1);

        // battery 2
        batUpdate(&Battery_2);

        // heater
        heaterUpdate();

        // others
        ui->checkBox_adaptor->setChecked((const bool)Adaptor.status);
        ui->checkBox_fan->setChecked((const bool)FanOn);
        ui->checkBox_valve1->setChecked((const bool)Valve_1_On);
        ui->checkBox_valve2->setChecked((const bool)Valve_2_On);
        ui->lineEdit_gas1->setText(QString::number(gas1_pressure));
        ui->lineEdit_gas2->setText(QString::number(gas2_pressure));

        ui->label_test->setText(QString::number(tt));

        printf("updateui\n");
    }
    else if (event->timerId() == timerId_curveSample)
    {
        *(ui->rtcurve_bat1_v) << Battery_1.voltage;
        *(ui->rtcurve_bat1_t) << Battery_1.temperature;
        /*ui->rtcurve_bat2_v << Battery_2.voltage;
        ui->rtcurve_bat2_t << Battery_2.temperature*/;
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
    uint8_t data[8];
    uint16_t value = 0;
    uint16_t mask = DIO_BAT1_CHARGE;
    data[0] = BLOCK_A;
    value = checked ? DIO_BAT1_CHARGE : 0;
    WriteWordL(&data[1], mask);
    WriteWordL(&data[3], value);
    if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_DIO, data, 5, 10) == 0) {
        Pop("succeed", 500);
        Battery_1.charge_en = checked;
    }else {
        Pop("fail", 500);
    }
}

void MainWindow::on_checkBox_bat1_enable_clicked(bool checked)
{
    uint8_t data[8];
    uint16_t value = 0;
    uint16_t mask = DIO_BAT1_SUPPLY;
    data[0] = BLOCK_A;
    value = checked ? DIO_BAT1_SUPPLY : 0;
    WriteWordL(&data[1], mask);
    WriteWordL(&data[3], value);
    if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_DIO, data, 5, 10) == 0) {
        Pop("succeed", 1000);
        Battery_1.mux_on = checked;
    }else {
        Pop("fail", 1000);
    }
}

void MainWindow::on_checkBox_bat2_charge_clicked(bool checked)
{
    uint8_t data[8];
    uint16_t value = 0;
    uint16_t mask = DIO_BAT2_CHARGE;
    data[0] = BLOCK_A;
    value = checked ? DIO_BAT2_CHARGE : 0;
    WriteWordL(&data[1], mask);
    WriteWordL(&data[3], value);
    if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_DIO, data, 5, 10) == 0) {
        Pop("succeed", 1000);
        Battery_2.charge_en = checked;
    }else {
        Pop("fail", 1000);
    }
}

void MainWindow::on_checkBox_bat2_enable_clicked(bool checked)
{
    uint8_t data[8];
    uint16_t value = 0;
    uint16_t mask = DIO_BAT2_SUPPLY;
    data[0] = BLOCK_A;
    value = checked ? DIO_BAT2_SUPPLY : 0;
    WriteWordL(&data[1], mask);
    WriteWordL(&data[3], value);
    if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_DIO, data, 5, 5) == 0) {
        Pop("succeed", 500);
        Battery_2.mux_on = checked;
    }else {
        Pop("fail", 500);
    }  
}

//void MainWindow::on_radioButton_heaterOn_pressed()
//{
//    if (ui->radioButton_heaterOn->isChecked())
//        radio_heater = 2;
//    else if (ui->radioButton_heaterOff->isChecked())
//        radio_heater = 1;
//    else if (ui->radioButton_heaterOff->isChecked() && ui->radioButton_heaterOn->isChecked())
//        radio_heater = 0;
//}

//void MainWindow::on_radioButton_heaterOff_pressed()
//{
//    if (ui->radioButton_heaterOn->isChecked())
//        radio_heater = 2;
//    else if (ui->radioButton_heaterOff->isChecked())
//        radio_heater = 1;
//    else if (ui->radioButton_heaterOff->isChecked() && ui->radioButton_heaterOn->isChecked())
//        radio_heater = 0;
//}

//void MainWindow::on_radioButton_heaterOn_clicked(bool checked)
//{
//    if (radio_heater == 2)
//        return;

//    uint8_t data[8];
//    uint16_t value = 0;
//    uint16_t mask = DIO_HEATER;
//    data[0] = BLOCK_A;
//    value = checked ? DIO_HEATER : 0;
//    WriteWordL(&data[1], mask);
//    WriteWordL(&data[3], value);

//    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WRITE_DIO, data, 5, 10) == 0) {
//        Pop("succeed", 1000);
//        radio_heater = 2;
//    }else {
//        Pop("fail", 1000);
//        if (radio_heater == 1)
//            ui->radioButton_heaterOff->setChecked(true);
//    }
//}

//void MainWindow::on_radioButton_heaterOff_clicked(bool checked)
//{
//    if (radio_heater == 1)
//        return;

//    uint8_t data[8];
//    uint16_t value = 0;
//    uint16_t mask = DIO_HEATER;
//    data[0] = BLOCK_A;
//    value = 0;
//    WriteWordL(&data[1], mask);
//    WriteWordL(&data[3], value);

//    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_WRITE_DIO, data, 5, 10) == 0) {
//        Pop("succeed", 1000);
//        radio_heater = 1;
//    }else {
//        Pop("fail", 1000);
//        if (radio_heater == 2)
//            ui->radioButton_heaterOn->setChecked(true);
//    }
//}

void MainWindow::on_lineEdit_heaterSP_returnPressed()
{
    bool ok = false;
    uint8_t data[8] = {0};
    tuType32Bit tu_value;
    Heater.setpoint = ui->lineEdit_heaterSP->text().toFloat(&ok);
    tu_value.tFloat = Heater.setpoint;
    data[0] = 0x03;
    data[1] = 0x02;
    WriteLongL(&data[2], tu_value.tInt32);
    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_DEBUG_WR, data, 6, 5) == 0)
        Pop("succeed", 500);
    else
        Pop("fail", 500);
}

void MainWindow::on_lineEdit_heaterKp_returnPressed()
{
    bool ok = false;
    uint8_t data[8] = {0};
    Heater.kp = ui->lineEdit_heaterKp->text().toInt(&ok, 10);
    data[0] = 0x03;
    data[1] = 0x04;
    WriteLongL(&data[2], Heater.kp);

    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_DEBUG_WR, data, 4, 10) == 0)
        Pop("succeed", 500);
    else
        Pop("fail", 500);
}

void MainWindow::on_lineEdit_heaterKi_returnPressed()
{
    bool ok = false;
    uint8_t data[8] = {0};
    Heater.ki = ui->lineEdit_heaterKi->text().toInt(&ok, 10);
    data[0] = 0x03;
    data[1] = 0x05;
    WriteLongL(&data[2], Heater.ki);

    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_DEBUG_WR, data, 4, 10) == 0)
        Pop("succeed", 500);
    else
        Pop("fail", 500);
}

void MainWindow::on_lineEdit_heaterKd_returnPressed()
{
    bool ok = false;
    uint8_t data[8] = {0};
    Heater.kd = ui->lineEdit_heaterKd->text().toInt(&ok, 10);
    data[0] = 0x03;
    data[1] = 0x06;
    WriteLongL(&data[2], Heater.kd);

    if (SendCommand(CANID_CHARGE, CANID_MB, CMD_DEBUG_WR, data, 4, 10) == 0)
        Pop("succeed", 500);
    else
        Pop("fail", 500);
}

void MainWindow::on_pushButton_OpenCAN_clicked()
{
    char buf[64];
    int8_t ret = 0;

    if (com_isopen == true)
        return;

    if (can_port.Open(com_num, 115200)) {
        sprintf_s(buf, "COM port %d open succeed", com_num);
        Pop(buf, 500);
        com_isopen = true;
        if ((ret = CAN_Init(1000000)) == 0) {
            sprintf_s(buf, "COM port %d config succeed, baud rate 1M", com_num);
            Pop(buf, 500);
            ui->label_comstatus->setText("is opened");
            t_poll->start();
        }else {
            sprintf_s(buf, "COM port %d config fail", com_num);
            Pop(buf, 500);
            can_port.Close();
            sprintf_s(buf, "COM port %d is closed", com_num);
            Pop(buf, 500);
            ui->label_comstatus->setText("is closed");
            com_isopen = false;
        }
    }else {
        sprintf_s(buf, "COM port %d open fail", com_num);
        Pop(buf, 500);
    }
}

void MainWindow::on_pushButton_CloseCAN_clicked()
{
    char buf[64];
    can_port.Close();
    com_isopen = false;
    ui->label_comstatus->setText("is closed");
    sprintf_s(buf, "COM port %d is closed\n", com_num);
    Pop(buf, 500);
    t_poll->quit();
}


void MainWindow::on_checkBox_fan_clicked(bool checked)
{
    uint8_t data[8];
    uint16_t value = 0;
    uint16_t mask = DIO_FAN;
    data[0] = BLOCK_A;
    value = checked ? DIO_FAN : 0;
    WriteWordL(&data[1], mask);
    WriteWordL(&data[3], value);
    if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_DIO, data, 5, 10) == 0) {
        Pop("succeed", 500);
        FanOn = checked;
    }else {
        Pop("fail", 500);
    }
}

void MainWindow::on_checkBox_valve1_clicked(bool checked)
{
    uint8_t data[8];
    uint16_t value = 0;
    uint16_t mask = DIO_VALVE1_SW;
    data[0] = BLOCK_A;
    value = checked ? DIO_VALVE1_SW : 0;
    WriteWordL(&data[1], mask);
    WriteWordL(&data[3], value);
    if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_DIO, data, 5, 10) == 0) {
        Pop("succeed", 500);
        Valve_1_On = checked;
    }else {
        Pop("fail", 500);
    }
}

void MainWindow::on_checkBox_valve2_clicked(bool checked)
{
    uint8_t data[8];
    uint16_t value = 0;
    uint16_t mask = DIO_VALVE2_SW;
    data[0] = BLOCK_A;
    value = checked ? DIO_VALVE2_SW : 0;
    WriteWordL(&data[1], mask);
    WriteWordL(&data[3], value);
    if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_DIO, data, 5, 10) == 0) {
        Pop("succeed", 500);
        Valve_2_On = checked;
    }else {
        Pop("fail", 500);
    }
}

void MainWindow::on_comboBox_heater_activated(const QString &arg1)
{
    uint8_t data[8];
    int32_t value;
    if (QString::compare(arg1, "OFF") == 0) {
        data[0] = 0x03;
        data[1] = 0x00;
        if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_SP, data, 2, 5) == 0)
            Pop("succeed", 500);
        else
            Pop("fail", 500);
    }else if (QString::compare(arg1, "PID ON") == 0) {
        value = ui->lineEdit_heaterSP->text().toFloat() * 100;
        data[0] = 0x03;
        data[1] = 0x01;
        WriteLongL(&data[2], value);
        if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_SP, data, 5, 5) == 0)
            Pop("succeed", 500);
        else
            Pop("fail", 500);
    }else if (QString::compare(arg1, "FIXPWM ON") == 0) {
        value = ui->lineEdit_heaterDuty->text().toInt();
        data[0] = 0x03;
        data[1] = 0x02;
        WriteWordL(&data[2], value);
        if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_SP, data, 4, 5) == 0)
            Pop("succeed", 500);
        else
            Pop("fail", 500);
    }else if(QString::compare(arg1, "RESET") == 0) {
        value = Heater.duty * 10000;
        data[0] = 0x03;
        data[1] = 0x04;
        WriteLongL(&data[2], value);
        if (SendCommand(CANID_CHARGE, CANID_PC, CMD_WRITE_SP, data, 2, 5) == 0)
            Pop("succeed", 500);
        else
            Pop("fail", 500);
    }
}

void MainWindow::on_spinBox_COMNum_editingFinished()
{
    com_num = ui->spinBox_COMNum->text().toInt();
    on_pushButton_OpenCAN_clicked();
}

void MainWindow::on_pushButton_reset1_clicked()
{
    ui->rtcurve_bat1_v->Clear();
}

void MainWindow::on_pushButton_reset2_clicked()
{
    ui->rtcurve_bat1_t->Clear();
}

void MainWindow::on_pushButton_reset3_clicked()
{
    ui->rtcurve_bat2_v->Clear();
}

void MainWindow::on_pushButton_reset4_clicked()
{
    ui->rtcurve_bat2_t->Clear();
}
