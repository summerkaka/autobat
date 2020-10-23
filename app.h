#ifndef CAN_APP_H
#define CAN_APP_H

#include <QThread>

typedef struct {
    uint8_t     status;
    float       voltage;
    float       current;
    float       temperature;
    int16_t     level;
    int16_t     capacity;
    uint16_t    err_code;
    bool        mux_on;
} stBattery;
extern stBattery Battery_1;
extern stBattery Battery_2;

typedef struct {
    uint8_t     status;
    float       temperature;
    int32_t     pt100_adccode;
    float       setpoint;
    float       duty;
    int16_t     kp;
    int16_t     ki;
    int16_t     kd;
} stHeater;
extern stHeater Heater;

class Thread_Poll : public QThread
{
    Q_OBJECT
public:
    Thread_Poll(QObject *parent = nullptr);
    ~Thread_Poll();

protected:
    void run();
};

class Thread_Listen : public QThread
{
    Q_OBJECT
public:
    Thread_Listen(QObject *parent = nullptr);
    ~Thread_Listen();

protected:
    void run();
};

class Thread_Dummy : public QThread
{
    Q_OBJECT
public:
    Thread_Dummy(QObject *parent = nullptr);
    ~Thread_Dummy();

protected:
    void run();
};


#endif // CAN_APP_H
