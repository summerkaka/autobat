#include "realtimecurve.h"
#include <QDateTime>
#include <QHBoxLayout>

#define MAXSIZE 51      // 只存储最新的 51 个数据
#define MAXVALUE 100    // 数据的最大值为 100，因为我们生成的随机数为 [0, 100]


RealTimeCurve::RealTimeCurve(QWidget *parent, const QString &tittle) :
    QWidget(parent),
    chart_tittle(tittle)
{
    y_max = 0;
    y_min = 0;
    timerId = startTimer(200);
    qsrand(QDateTime::currentDateTime().toTime_t());
    curve1 = new QSplineSeries();
    curve2 = new QSplineSeries();
    scatter1 = new QScatterSeries();
    scatter1->setMarkerSize(8);
    scatter2= new QScatterSeries();
    scatter2->setMarkerSize(8);
    // 预先分配坐标，这样在 dataReceived 中直接替换坐标了
    for (int i = 0; i < MAXSIZE; ++i) {
        curve1->append(i * 10, -10);
        scatter1->append(i * 10, -10);
        curve2->append(i * 10, -10);
        scatter2->append(i * 10, -10);
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

RealTimeCurve::~RealTimeCurve()
{}

void RealTimeCurve::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        // 模拟不停的接收到新数据
        int newData = qrand() % (MAXVALUE + 1);
        int newData2 = qrand() % (MAXVALUE + 1);
        y_max = y_max > newData ? y_max : newData;
        y_max = y_max > newData2 ? y_max : newData2;
        y_min = y_min < newData ? y_min : newData;
        y_min = y_min < newData2 ? y_min : newData2;
//        updateCurve(newData, newData2);
    }
}

void RealTimeCurve::updateCurve(void)
{
    // 模拟不停的接收到新数据
    int newData = qrand() % (MAXVALUE + 1);
    int newData2 = qrand() % (MAXVALUE + 1);
    y_max = y_max > newData ? y_max : newData;
    y_max = y_max > newData2 ? y_max : newData2;
    y_min = y_min < newData ? y_min : newData;
    y_min = y_min < newData2 ? y_min : newData2;

//    data1 << value[1];
//    data2 << value[2];
    data1 << newData;
    data2 << newData2;
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
        chart->axisY()->setRange(y_min, y_max);
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


