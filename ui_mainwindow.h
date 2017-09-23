/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <singlecurve.h>
#include "dualcurve.h"
#include "can_app.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget_bat;
    QWidget *tab;
    QTableWidget *tableWidget_bat;
    QGroupBox *groupBox;
    QLabel *label_4;
    QSlider *setcurrent_slider_2;
    QCheckBox *checkBox_bat2_enable;
    QCheckBox *checkBox_bat2_charge;
    QGroupBox *groupBox_2;
    QLabel *label_5;
    QSlider *setcurrent_slider_1;
    QCheckBox *checkBox_bat1_enable;
    QCheckBox *checkBox_bat1_charge;
    DualCurve *rtcurve_bat_v;
    DualCurve *rtcurve_bat_t;
    QWidget *tab_3;
    QTableWidget *tableWidget_heater;
    SingleCurve *rtcurve_heater_t;
    QGroupBox *groupBox_3;
    QLineEdit *lineEdit_heaterKp;
    QLineEdit *lineEdit_heaterKi;
    QLineEdit *lineEdit_heaterKd;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit_heaterSP;
    QLabel *label_6;
    QRadioButton *radioButton_heaterOn;
    QRadioButton *radioButton_HeaterOff;
    QPushButton *pushButton_heaterUpLD;
    QWidget *tab_2;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QProgressBar *progressBar;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 800);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget_bat = new QTabWidget(centralWidget);
        tabWidget_bat->setObjectName(QStringLiteral("tabWidget_bat"));
        tabWidget_bat->setGeometry(QRect(5, 5, 1271, 696));
        tabWidget_bat->setMaximumSize(QSize(16777215, 16777215));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tableWidget_bat = new QTableWidget(tab);
        if (tableWidget_bat->columnCount() < 2)
            tableWidget_bat->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_bat->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_bat->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableWidget_bat->rowCount() < 7)
            tableWidget_bat->setRowCount(7);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_bat->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_bat->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_bat->setVerticalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_bat->setVerticalHeaderItem(3, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_bat->setVerticalHeaderItem(4, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_bat->setVerticalHeaderItem(5, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_bat->setVerticalHeaderItem(6, __qtablewidgetitem8);
        tableWidget_bat->setObjectName(QStringLiteral("tableWidget_bat"));
        tableWidget_bat->setGeometry(QRect(60, 45, 301, 236));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(80, 455, 201, 81));
        groupBox->setStyleSheet(QLatin1String("QGroupBox{\n"
"border-width:2px;\n"
"border-style:solid;\n"
"border-radius: 10px;\n"
"border-color:gray;\n"
"margin-top:1ex;\n"
"}\n"
"QGroupBox::title{\n"
"subcontrol-origin:margin;\n"
"subcontrol-position:top left;\n"
"left:10px;\n"
"margin-left:0px;\n"
"padding:0 1px;\n"
"}"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(175, 60, 21, 17));
        setcurrent_slider_2 = new QSlider(groupBox);
        setcurrent_slider_2->setObjectName(QStringLiteral("setcurrent_slider_2"));
        setcurrent_slider_2->setGeometry(QRect(10, 60, 160, 16));
        setcurrent_slider_2->setOrientation(Qt::Horizontal);
        checkBox_bat2_enable = new QCheckBox(groupBox);
        checkBox_bat2_enable->setObjectName(QStringLiteral("checkBox_bat2_enable"));
        checkBox_bat2_enable->setGeometry(QRect(90, 30, 71, 23));
        checkBox_bat2_charge = new QCheckBox(groupBox);
        checkBox_bat2_charge->setObjectName(QStringLiteral("checkBox_bat2_charge"));
        checkBox_bat2_charge->setGeometry(QRect(10, 30, 71, 23));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(80, 345, 201, 81));
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox{\n"
"border-width:2px;\n"
"border-style:solid;\n"
"border-radius: 10px;\n"
"border-color:gray;\n"
"margin-top:1ex;\n"
"}\n"
"QGroupBox::title{\n"
"subcontrol-origin:margin;\n"
"subcontrol-position:top left;\n"
"left:10px;\n"
"margin-left:0px;\n"
"padding:0 1px;\n"
"}"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(175, 60, 21, 17));
        setcurrent_slider_1 = new QSlider(groupBox_2);
        setcurrent_slider_1->setObjectName(QStringLiteral("setcurrent_slider_1"));
        setcurrent_slider_1->setGeometry(QRect(10, 60, 160, 16));
        setcurrent_slider_1->setOrientation(Qt::Horizontal);
        checkBox_bat1_enable = new QCheckBox(groupBox_2);
        checkBox_bat1_enable->setObjectName(QStringLiteral("checkBox_bat1_enable"));
        checkBox_bat1_enable->setGeometry(QRect(90, 30, 71, 23));
        checkBox_bat1_charge = new QCheckBox(groupBox_2);
        checkBox_bat1_charge->setObjectName(QStringLiteral("checkBox_bat1_charge"));
        checkBox_bat1_charge->setGeometry(QRect(10, 30, 71, 23));
        rtcurve_bat_v = new DualCurve(Battery_1.voltage, Battery_2.voltage, tab);
        rtcurve_bat_v->setObjectName(QStringLiteral("rtcurve_bat_v"));
        rtcurve_bat_v->setGeometry(QRect(400, 40, 856, 311));
        rtcurve_bat_t = new DualCurve(Battery_1.temperature, Battery_2.temperature, tab);
        rtcurve_bat_t->setObjectName(QStringLiteral("rtcurve_bat_t"));
        rtcurve_bat_t->setGeometry(QRect(400, 360, 856, 286));
        tabWidget_bat->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        tableWidget_heater = new QTableWidget(tab_3);
        if (tableWidget_heater->columnCount() < 1)
            tableWidget_heater->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_heater->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        if (tableWidget_heater->rowCount() < 4)
            tableWidget_heater->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_heater->setVerticalHeaderItem(0, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_heater->setVerticalHeaderItem(1, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_heater->setVerticalHeaderItem(2, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_heater->setVerticalHeaderItem(3, __qtablewidgetitem13);
        tableWidget_heater->setObjectName(QStringLiteral("tableWidget_heater"));
        tableWidget_heater->setGeometry(QRect(30, 35, 216, 146));
        rtcurve_heater_t = new SingleCurve(Heater.temperature, 2, tab_3);
        rtcurve_heater_t->setObjectName(QStringLiteral("rtcurve_heater_t"));
        rtcurve_heater_t->setGeometry(QRect(265, 15, 996, 631));
        groupBox_3 = new QGroupBox(tab_3);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(35, 195, 191, 201));
        groupBox_3->setStyleSheet(QLatin1String("QGroupBox{\n"
"border-width:2px;\n"
"border-style:solid;\n"
"border-radius: 10px;\n"
"border-color:gray;\n"
"margin-top:1ex;\n"
"}\n"
"QGroupBox::title{\n"
"subcontrol-origin:margin;\n"
"subcontrol-position:top left;\n"
"left:10px;\n"
"margin-left:0px;\n"
"padding:0 1px;\n"
"}"));
        lineEdit_heaterKp = new QLineEdit(groupBox_3);
        lineEdit_heaterKp->setObjectName(QStringLiteral("lineEdit_heaterKp"));
        lineEdit_heaterKp->setGeometry(QRect(90, 90, 71, 25));
        lineEdit_heaterKi = new QLineEdit(groupBox_3);
        lineEdit_heaterKi->setObjectName(QStringLiteral("lineEdit_heaterKi"));
        lineEdit_heaterKi->setGeometry(QRect(90, 125, 71, 25));
        lineEdit_heaterKd = new QLineEdit(groupBox_3);
        lineEdit_heaterKd->setObjectName(QStringLiteral("lineEdit_heaterKd"));
        lineEdit_heaterKd->setGeometry(QRect(90, 160, 71, 25));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 95, 26, 16));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 130, 26, 17));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(60, 165, 26, 17));
        lineEdit_heaterSP = new QLineEdit(groupBox_3);
        lineEdit_heaterSP->setObjectName(QStringLiteral("lineEdit_heaterSP"));
        lineEdit_heaterSP->setGeometry(QRect(90, 55, 71, 25));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 60, 66, 16));
        radioButton_heaterOn = new QRadioButton(groupBox_3);
        radioButton_heaterOn->setObjectName(QStringLiteral("radioButton_heaterOn"));
        radioButton_heaterOn->setGeometry(QRect(15, 25, 41, 23));
        radioButton_HeaterOff = new QRadioButton(groupBox_3);
        radioButton_HeaterOff->setObjectName(QStringLiteral("radioButton_HeaterOff"));
        radioButton_HeaterOff->setGeometry(QRect(60, 25, 46, 23));
        pushButton_heaterUpLD = new QPushButton(groupBox_3);
        pushButton_heaterUpLD->setObjectName(QStringLiteral("pushButton_heaterUpLD"));
        pushButton_heaterUpLD->setGeometry(QRect(125, 25, 56, 21));
        tabWidget_bat->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        pushButton = new QPushButton(tab_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(240, 70, 96, 25));
        lineEdit = new QLineEdit(tab_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(35, 70, 191, 25));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(240, 110, 96, 25));
        progressBar = new QProgressBar(tab_2);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(35, 160, 291, 23));
        progressBar->setValue(24);
        tabWidget_bat->addTab(tab_2, QString());
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(1200, 705, 61, 25));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(1120, 705, 61, 25));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget_bat->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_bat->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "battery-1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_bat->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "battery-2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_bat->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "status", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_bat->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "voltage", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_bat->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "current", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_bat->verticalHeaderItem(3);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "temperature", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_bat->verticalHeaderItem(4);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "level", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_bat->verticalHeaderItem(5);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "capacity", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_bat->verticalHeaderItem(6);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "err code", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "battery-2", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "0A", Q_NULLPTR));
        checkBox_bat2_enable->setText(QApplication::translate("MainWindow", "enable", Q_NULLPTR));
        checkBox_bat2_charge->setText(QApplication::translate("MainWindow", "charge", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "battery-1", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "0A", Q_NULLPTR));
        checkBox_bat1_enable->setText(QApplication::translate("MainWindow", "enable", Q_NULLPTR));
        checkBox_bat1_charge->setText(QApplication::translate("MainWindow", "charge", Q_NULLPTR));
        tabWidget_bat->setTabText(tabWidget_bat->indexOf(tab), QApplication::translate("MainWindow", "battery", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_heater->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "heater1&2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_heater->verticalHeaderItem(0);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "status", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_heater->verticalHeaderItem(1);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "temperature", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_heater->verticalHeaderItem(2);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "pt100 adc_code", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_heater->verticalHeaderItem(3);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "duty cycle", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "settings", Q_NULLPTR));
        lineEdit_heaterKp->setText(QApplication::translate("MainWindow", "Kp", Q_NULLPTR));
        lineEdit_heaterKi->setText(QApplication::translate("MainWindow", "Ki", Q_NULLPTR));
        lineEdit_heaterKd->setText(QApplication::translate("MainWindow", "Kd", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Kp", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Ki", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Kd", Q_NULLPTR));
        lineEdit_heaterSP->setText(QApplication::translate("MainWindow", "set point", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "set point", Q_NULLPTR));
        radioButton_heaterOn->setText(QApplication::translate("MainWindow", "on", Q_NULLPTR));
        radioButton_HeaterOff->setText(QApplication::translate("MainWindow", "off", Q_NULLPTR));
        pushButton_heaterUpLD->setText(QApplication::translate("MainWindow", "upload", Q_NULLPTR));
        tabWidget_bat->setTabText(tabWidget_bat->indexOf(tab_3), QApplication::translate("MainWindow", "heater", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "browse file", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "update start", Q_NULLPTR));
        tabWidget_bat->setTabText(tabWidget_bat->indexOf(tab_2), QApplication::translate("MainWindow", "fw update", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MainWindow", "exit", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainWindow", "reset", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
