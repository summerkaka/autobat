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
    float           last_voltage;       // to monitor voltage drop
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
    uint8_t         aging_cnt[2];       // [0] for reach top, [1] for reach bottom
    float           charge_current;     // dynamic setting for fast-charge
    uint32_t        light_transition;   // light load begin time second
    uint32_t        heavy_transition;   // heavy load begin time second
    uint32_t        tick_sample;        // sample gauge voltage every 50ms
    uint32_t        tick_drop;          // timer to record battery vdrop, unit is second
    uint32_t        tick_2s;            // timer to check cell short
    uint32_t        pre_start_time;     // pre-charge start moment
    uint32_t        fast_start_time;    // fast-charge start moment
    uint32_t        fastcharge_timer;   // fast charge time out protection
    uint32_t        trickle_start_time; // trickle-charge start moment
    uint32_t        charge_finish_time;	// last one cycle charge finsh moment (minutes)
    uint16_t        charge_times;       // total charge times of this battery
    uint16_t        err_code;
    float           impedance;
    bool            ocv_flag;           // indicate bettery is oc
    float           i_heavy;            // last heavy load I record
    float           v_heavy;            // last heavy load V record
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
    stBattery   *bat_channel;    // (0)NULL, 1, 2, (3)BOTH
    float       consumption;
    float       v_syspwr;
    bool        is_covered;
    bool        is_switchon;
    uint32_t    cover_time;     // minutes
    uint32_t    switchoff_time; // minutes
    uint32_t    switchon_time;  // seconds, used to prevent spurious lo_pwr_trigger
    float       gas_1_pressure;
    float       gas_2_pressure;
} stSystem;
extern stSystem FieldCase;

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
extern uint32_t runtime_min;
extern uint32_t runtime_sec;
extern bool Gc_on;
void InfoInit(void);

#endif // CAN_APP_H
