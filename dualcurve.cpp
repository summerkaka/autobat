#include "dualcurve.h"
#include <QDateTime>
#include <QHBoxLayout>

#define MAXSIZE 181      // 只存储最新的 51 个数据
#define MAXVALUE 100    // 数据的最大值为 100，因为我们生成的随机数为 [0, 100]

DualCurve::DualCurve(float &newd1, float &newd2, QWidget *parent, const QString &tittle) :
    QWidget(parent),
    newdata1(newd1),
    newdata2(newd2),
    chart_tittle(tittle)
{
    y_max = 0;
    y_min = 0;
//    newdata1 = data1;
//    newdata2 = data2;
    timerId = startTimer(SAMPLE_INTERVAL * 1000);
    qsrand(QDateTime::currentDateTime().toTime_t());
    curve1 = new QSplineSeries();
    curve2 = new QSplineSeries();
    scatter1 = new QScatterSeries();
    scatter1->setMarkerSize(8);
    scatter2= new QScatterSeries();
    scatter2->setMarkerSize(8);
    // 预先分配坐标，这样在 dataReceived 中直接替换坐标了
    for (int i = 0; i < MAXSIZE; ++i) {
        curve1->append(i, -10);
        scatter1->append(i, -10);
        curve2->append(i, -10);
        scatter2->append(i, -10);
//        scatterSeries->append(i * 10, -10);
    }
    chart = new QChart();

    chart->addSeries(curve1);
    chart->addSeries(curve2);
    chart->addSeries(scatter1);
    chart->addSeries(scatter2);
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

DualCurve::~DualCurve()
{}

void DualCurve::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        // 模拟不停的接收到新数据
//        int newData = qrand() % (MAXVALUE + 1);
//        int newData2 = qrand() % (MAXVALUE + 1);
        y_max = y_max > newdata1 ? y_max : newdata1;
        y_max = y_max > newdata2 ? y_max : newdata2;
        y_min = y_min < newdata1 ? y_min : newdata1;
        y_min = y_min < newdata2 ? y_min : newdata2;
        updateCurve();
    }
}

//void DualCurve::Config(float &data1, float &data2, const QString &title, int16_t sample_interval)
//{
//    newdata1 = data1;
//    newdata2 = data2;
//    chart->setTitle(title);
//    timerId = startTimer(sample_interval);
//}

void DualCurve::updateCurve(void)
{
    data1 << newdata1;
    data2 << newdata2;
    // 数据个数超过了指定值，则删除最先接收到的数据，实现曲线向前移动
    while (data1.size() > MAXSIZE) {
        data1.removeFirst();
    }
    while (data2.size() > MAXSIZE) {
        data2.removeFirst();
    }
    if (isVisible()) {
        // 界面被隐藏后就没有必要绘制数据的曲线了
        // 替换曲线中现有数据
        int delta1 = MAXSIZE - data1.size();
        int delta2 = MAXSIZE - data2.size();
        chart->axisY()->setRange(y_min-5, y_max+5);
        for (int i = 0; i < data1.size(); ++i) {
            curve1->replace(delta1+i, curve1->at(delta1+i).x(), data1.at(i));
            scatter1->replace(delta1+i, scatter1->at(delta1+i).x(), data1.at(i));
        }
        for (int i = 0; i < data2.size(); ++i) {
            curve2->replace(delta2+i, curve2->at(delta2+i).x(), data2.at(i));
            scatter2->replace(delta2+i, scatter2->at(delta2+i).x(), data2.at(i));
        }
    }
//    return (void *)0;
}


