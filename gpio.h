#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

#define GPIO_PIN_SET    1
#define GPIO_PIN_RESET  0

typedef struct {
    int gpio0 :1;
    int gpio1 :1;
    int gpio2 :1;
    int gpio3 :1;
    int gpio4 :1;
    int gpio5 :1;
    int gpio6 :1;
    int gpio7 :1;
    int gpio8 :1;
    int gpio9 :1;
    int gpio10 :1;
    int gpio11 :1;
    int gpio12 :1;
    int gpio13 :1;
    int gpio14 :1;
    int gpio15 :1;
} GpioBit;

typedef union {
    long All;
    GpioBit Bit;
} stGpio;

extern stGpio gpio0_16;

void Gpio_Init(void);
int Gpio_Write(int, bool);
int Gpio_Pin_Config(int num, int base, char *direction, bool init_value);
int Gpio_Pin_DeConfig(int num, int base);

#endif // GPIO_H
