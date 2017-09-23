#ifndef DUALCURVE_H
#define DUALCURVE_H

#include <QWidget>
#include <QList>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>

#define SAMPLE_INTERVAL 5   // unit: s

using namespace QtCharts;

class DualCurve : public QWidget
{
    Q_OBJECT
public:
    explicit DualCurve(float &newdat1,
                           float &newdat2,
                           QWidget *parent = nullptr,
                           const QString &tittle = "实时动态曲线");
    ~DualCurve();
//    void Config(float &data1, float &data2, const QString &title = "实时动态曲线", int16_t sample_interval = 5000);

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    int timerId;
    float y_max = 0, y_min = 0;
    float &newdata1;
    float &newdata2;
    QString chart_tittle;
    QList<double> data1, data2; // 存储业务数据的 list
    QChart *chart;
    QChartView *chartView;
    QValueAxis *m_axis;
    QSplineSeries *curve1, *curve2;
    QScatterSeries *scatter1, *scatter2;
    void updateCurve(void);

signals:

public slots:

};



#endif // DUALCURVE_H
