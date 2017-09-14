#ifndef REALTIMECURVE_H
#define REALTIMECURVE_H

#include <QWidget>
#include <QList>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QChartView>
#include "realtimecurve.h"


using namespace QtCharts;

class RealTimeCurve : public QWidget
{
    Q_OBJECT
public:
    explicit RealTimeCurve(QWidget *parent = nullptr, const QString &tittle = "实时动态曲线");
    ~RealTimeCurve();
    void updateCurve(void);

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    int timerId;
    float y_max = 0, y_min = 0;
    QString chart_tittle;
    QList<double> data1, data2; // 存储业务数据的 list
    QChart *chart;
    QChartView *chartView;
    QSplineSeries *curve1, *curve2;
    QScatterSeries *scatter1, *scatter2;

signals:

public slots:

};



#endif // REALTIMECURVE_H
