#ifndef _LED_KEY_H_
#define _LED_KEY_H_

#include "sg_lgpio.h"

#define PIN_D1    4 
#define PIN_D2    18 
#define PIN_K1    27
#define PIN_K2    22

#define D1_ON  Lgpio_write_pin(PIN_D1,LGPIO_LOW)
#define D1_OFF  Lgpio_write_pin(PIN_D1,LGPIO_HIGH)
#define D2_ON  Lgpio_write_pin(PIN_D2,LGPIO_LOW)
#define D2_OFF  Lgpio_write_pin(PIN_D2,LGPIO_HIGH)

void LED_KEY_Init(void);

void Led_key_task(void);

#endif
/***********************************************************
						end file
***********************************************************/


