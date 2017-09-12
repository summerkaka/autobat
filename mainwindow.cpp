#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "app_include.h"

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

