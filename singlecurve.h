#ifndef SINGLECURVE_H
#define SINGLECURVE_H

#include <QWidget>
#include <QList>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>

using namespace QtCharts;

class SingleCurve : public QWidget
{
    Q_OBJECT
public:
    explicit SingleCurve(QWidget *parent = nullptr,
                         const QString &tittle = "nocurve");
    ~SingleCurve();
    void Config(float *data1, const QString &title = "realtime curve", int16_t sample_interval = 5);

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    int timerId;
    float y_max = 0, y_min = 0;
    float *newdata1 = nullptr;
    QList<double> data1; // 存储业务数据的 list
    QChart *chart;
    QChartView *chartView;
    QValueAxis *m_axis;
    QSplineSeries *curve1;
    QScatterSeries *scatter1;


signals:

public slots:

};

#endif // SINGLECURVE_H
