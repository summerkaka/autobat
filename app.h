#ifndef CAN_APP_H
#define CAN_APP_H

#include <QThread>

struct _StBattery{
    uint8_t         index;
    uint8_t         status;
    bool            mux_on;             // ideal-diode switch status
    bool            charge_en;          // soft switch to start charge
    float           voltage;
    float           max_volt;
    float           volt_lastmin;       // to monitor voltage drop
    float           remain_time;        // battery remain time to reach 10V alarm_line
    float           current;
    float           ng;                 // 0~1 , battery charge efficiency
    float           level;              // current capacity
    float           capacity;           // full capacity
    float           level_top;
    float           level_bottom;
    uint8_t	        scale_flag;	        // 0b00x1: level_bottom record, 0b001x: level_top record
    float           temperature;
    bool            is_aged;
    float           impedance;
    float           charge_current;     // dynamic setting for fast-charge
    uint32_t        pre_start_time;     // pre-charge start moment
    uint32_t        fast_start_time;    // fast-charge start moment
    uint32_t        fastcharge_timer;   // fast charge time out protection
    uint32_t        trickle_start_time; // trickle-charge start moment
    uint32_t        last_charge_time;	// last one cycle charge finsh moment (minutes)
    uint16_t        charge_times;       // total charge times of this battery
    uint16_t        err_code;
    float           ocv;                // for calculating impedance
};
typedef struct _StBattery stBattery;
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

typedef struct {
    uint8_t     status;
    float       voltage;
} stAdaptor;
extern stAdaptor Adaptor;

typedef struct {
    float   out_voltage;
    float   consumption;
    bool    is_covered;
    bool    is_switchoff;
} stFieldCase;
extern stFieldCase FieldCase;

class Thread_Poll : public QThread
{
    Q_OBJECT
public:
    Thread_Poll(QObject *parent = nullptr);
    ~Thread_Poll();

protected:
    void run();
};




extern int tt;
extern bool FanOn;
extern bool Valve_1_On;
extern bool Valve_2_On;
extern float gas1_pressure;
extern float gas2_pressure;

void InfoInit(void);

#endif // CAN_APP_H
