#include "singlecurve.h"

#include <QDateTime>
#include <QHBoxLayout>

#define MAXSIZE     181
#define MAXVALUE    100
#define SAMPLE_INTERVAL 2  // unit: s

SingleCurve::SingleCurve(QWidget *parent, const QString &tittle) :
    QWidget(parent)
{
    curve1 = new QLineSeries();
    scatter1 = new QScatterSeries();
    scatter1->setMarkerSize(5);
    for (int i = 0; i < MAXSIZE; ++i) {
        curve1->append(i, -10);
        scatter1->append(i, -10);
    }
    chart = new QChart();
    chart->addSeries(curve1);
    chart->addSeries(scatter1);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, MAXSIZE);
    chart->axisY()->setRange(0, 1);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::HighQualityAntialiasing);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
}

SingleCurve::~SingleCurve()
{}

void SingleCurve::SetTitle(const QString &title)
{
    chart->setTitle(title);
}

void SingleCurve::Clear(void)
{
    float value = *data1.end();
    for (int i = 0; i < MAXSIZE; ++i) {
        data1 << value;
    }
}

void SingleCurve::operator << (float data)
{
    int delta1;
    float min, max;

    data1 << data;
    min = *std::min_element(data1.begin(), data1.end());
    max = *std::max_element(data1.begin(), data1.end());
    while (data1.size() > MAXSIZE) {
        data1.removeFirst();
    }
    if (isVisible()) {
        delta1 = MAXSIZE - data1.size();
        chart->axisY()->setRange(min - 0.01, max + 0.01);
        for (int i = 0; i < data1.size(); ++i) {
            curve1->replace(delta1+i, curve1->at(delta1+i).x(), data1.at(i));
            scatter1->replace(delta1+i, scatter1->at(delta1+i).x(), data1.at(i));
        }
    }
}
