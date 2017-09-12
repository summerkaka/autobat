#ifndef REALTIMECURVE_H
#define REALTIMECURVE_H

#include <QWidget>
#include <QList>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QChartView>
using namespace QtCharts;

class RealTimeCurve : public QWidget
{
    Q_OBJECT
public:
    explicit RealTimeCurve(QWidget *parent = nullptr);
    ~RealTimeCurve();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void dataReceived(int value1, int value2);
    int timerId;
    int maxValue; // 业务数据的最大值
    float y_max = 0, y_min = 0;
    QList<double> data1, data2; // 存储业务数据的 list
    QChart *chart;
    QChartView *chartView;
    QSplineSeries *curve1, *curve2;
    QScatterSeries *scatterSeries1, *scatterSeries2;

signals:

public slots:

};

#endif // REALTIMECURVE_H
