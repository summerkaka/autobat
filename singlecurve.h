#ifndef SINGLECURVE_H
#define SINGLECURVE_H

#include <QWidget>
#include <QList>
#include <QPushButton>
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
    void SetTitle(const QString &title = "realtime curve");
    void Clear(void);
    void operator << (float data);

protected:
    QList<float> data1; // 存储业务数据的 list

private:
    QChart *chart;
    QChartView *chartView;
    QValueAxis *m_axis;
    QLineSeries *curve1;
    QScatterSeries *scatter1;

signals:

public slots:

};

#endif // SINGLECURVE_H
