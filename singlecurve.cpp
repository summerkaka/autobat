#include "singlecurve.h"

#include <QDateTime>
#include <QHBoxLayout>

#define MAXSIZE     181
#define MAXVALUE    100
#define SAMPLE_INTERVAL 2  // unit: s

SingleCurve::SingleCurve(QWidget *parent, const QString &tittle) :
    QWidget(parent)
{
    y_max = 0;
    y_min = 0;
    timerId = startTimer(SAMPLE_INTERVAL * 1000);
    qsrand(QDateTime::currentDateTime().toTime_t());
    curve1 = new QSplineSeries();
    scatter1 = new QScatterSeries();
    scatter1->setMarkerSize(5);
    for (int i = 0; i < 181; ++i) {
        curve1->append(i, -10);
        scatter1->append(i, -10);
    }
    chart = new QChart();
    chart->addSeries(curve1);
    chart->addSeries(scatter1);
    chart->legend()->hide();
    chart->setTitle(tittle);
    chart->createDefaultAxes();
    chart->axisY()->setRange(0 + 25, 100 - 25);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
}

SingleCurve::~SingleCurve()
{}

void SingleCurve::Config(float *data1, const QString &title, int16_t sample_interval)
{
    newdata1 = data1;
    chart->setTitle(title);
    killTimer(timerId);
    timerId = startTimer(sample_interval * 1000);
}

void SingleCurve::timerEvent(QTimerEvent *event)
{
    int delta1;
    if (event->timerId() == timerId) {
        if (newdata1 == nullptr)
            return;

        y_max = y_max > *newdata1 ? y_max : *newdata1;
        y_min = y_min < *newdata1 ? y_min : *newdata1;

        data1 << *newdata1;
        while (data1.size() > MAXSIZE) {
            data1.removeFirst();
        }
        if (isVisible()) {
            delta1 = MAXSIZE - data1.size();
            chart->axisY()->setRange(y_min-5, y_max+5);
            for (int i = 0; i < data1.size(); ++i) {
                curve1->replace(delta1+i, curve1->at(delta1+i).x(), data1.at(i));
                scatter1->replace(delta1+i, scatter1->at(delta1+i).x(), data1.at(i));
            }
        }
    }
}

