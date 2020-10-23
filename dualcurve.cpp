#include "dualcurve.h"
#include <QDateTime>
#include <QHBoxLayout>

#define MAXSIZE 181     // store the newest 181 data
#define MAXVALUE 100    // data max value is 100

DualCurve::DualCurve(QWidget *parent, const QString &tittlea) :
    QWidget(parent)
{
    y_max = 0;
    y_min = 0;
    timerId = startTimer(SAMPLE_INTERVAL * 1000);
    qsrand(QDateTime::currentDateTime().toTime_t());
    curve1 = new QSplineSeries();
    curve2 = new QSplineSeries();
    scatter1 = new QScatterSeries();
    scatter1->setMarkerSize(5);
    scatter2 = new QScatterSeries();
    scatter2->setMarkerSize(5);
    // pre assign axis, replace in data receive function
    for (int i = 0; i < MAXSIZE; ++i) {
        curve1->append(i, -10);
        scatter1->append(i, -10);
        curve2->append(i, -10);
        scatter2->append(i, -10);
    }
    chart = new QChart();

    chart->addSeries(curve1);
    chart->addSeries(curve2);
    chart->addSeries(scatter1);
    chart->addSeries(scatter2);
    chart->legend()->hide();
    chart->setTitle(tittlea);
    chart->createDefaultAxes();
    chart->axisY()->setRange(0+25, MAXVALUE-25);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
}

DualCurve::~DualCurve()
{}

void DualCurve::timerEvent(QTimerEvent *event)
{
    int delta1 = 0;
    int delta2 = 0;

    if (event->timerId() == timerId) {

        if (newdata1 == nullptr || newdata2 == nullptr)
            return;

        y_max = y_max > *newdata1 ? y_max : *newdata1;
        y_max = y_max > *newdata2 ? y_max : *newdata2;
        y_min = y_min < *newdata1 ? y_min : *newdata1;
        y_min = y_min < *newdata2 ? y_min : *newdata2;

        data1 << *newdata1;
        data2 << *newdata2;
        while (data1.size() > MAXSIZE) {
            data1.removeFirst();
        }
        while (data2.size() > MAXSIZE) {
            data2.removeFirst();
        }
        if (isVisible()) {
            delta1 = MAXSIZE - data1.size();
            delta2 = MAXSIZE - data2.size();
            chart->axisY()->setRange(y_min-0.2, y_max+0.2);
            for (int i = 0; i < data1.size(); ++i) {
                curve1->replace(delta1+i, curve1->at(delta1+i).x(), data1.at(i));
                scatter1->replace(delta1+i, scatter1->at(delta1+i).x(), data1.at(i));
            }
            for (int i = 0; i < data2.size(); ++i) {
                curve2->replace(delta2+i, curve2->at(delta2+i).x(), data2.at(i));
                scatter2->replace(delta2+i, scatter2->at(delta2+i).x(), data2.at(i));
            }
        }
    }
}

void DualCurve::Config(float *data1, float *data2, const QString &title, int16_t sample_interval)
{
    newdata1 = data1;
    newdata2 = data2;
    chart->setTitle(title);
    killTimer(timerId);
    timerId = startTimer(sample_interval * 1000);
}



