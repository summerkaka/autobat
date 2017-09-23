#include "singlecurve.h"

#include <QDateTime>
#include <QHBoxLayout>

#define MAXSIZE     181    // 只存储最新的 51 个数据
#define MAXVALUE    100    // 数据的最大值为 100，因为我们生成的随机数为 [0, 100]
#define SAMPLE_INTERVAL 2  // unit: s

SingleCurve::SingleCurve(float &newd1, uint8_t sampel_time, QWidget *parent, const QString &tittle) :
    QWidget(parent),
    newdata1(newd1),
    chart_tittle(tittle)
{
    y_max = 0;
    y_min = 0;
//    newdata1 = data1;
//    newdata2 = data2;
    timerId = startTimer(sampel_time * 1000);
    qsrand(QDateTime::currentDateTime().toTime_t());
    curve1 = new QSplineSeries();
    scatter1 = new QScatterSeries();
    scatter1->setMarkerSize(8);
    // 预先分配坐标，这样在 dataReceived 中直接替换坐标了
    for (int i = 0; i < MAXSIZE; ++i) {
        curve1->append(i, -10);
        scatter1->append(i, -10);
    }
    chart = new QChart();
    chart->addSeries(curve1);
    chart->addSeries(scatter1);
    chart->legend()->hide();
    chart->setTitle(chart_tittle);
    chart->createDefaultAxes();
    chart->axisY()->setRange(0+25, MAXVALUE-25);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
}

SingleCurve::~SingleCurve()
{}

void SingleCurve::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        y_max = y_max > newdata1 ? y_max : newdata1;
        y_min = y_min < newdata1 ? y_min : newdata1;
        data1 << newdata1;
        // 数据个数超过了指定值，则删除最先接收到的数据，实现曲线向前移动
        while (data1.size() > MAXSIZE) {
            data1.removeFirst();
        }
        if (isVisible()) {
            // 界面被隐藏后就没有必要绘制数据的曲线了
            // 替换曲线中现有数据
            int delta1 = MAXSIZE - data1.size();
            chart->axisY()->setRange(y_min-5, y_max+5);
            for (int i = 0; i < data1.size(); ++i) {
                curve1->replace(delta1+i, curve1->at(delta1+i).x(), data1.at(i));
                scatter1->replace(delta1+i, scatter1->at(delta1+i).x(), data1.at(i));
            }
        }
    }
}
