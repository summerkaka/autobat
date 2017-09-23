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
    explicit SingleCurve(float &newdat1,
                         uint8_t sample_time = 2,
                         QWidget *parent = nullptr,
                         const QString &tittle = "实时动态曲线");
    ~SingleCurve();
//    void Config(float &data1, float &data2, const QString &title = "实时动态曲线", int16_t sample_interval = 5000);

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    int timerId;
    float y_max = 0, y_min = 0;
    float &newdata1;
    QString chart_tittle;
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
