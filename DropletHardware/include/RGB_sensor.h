#include <avr/io.h>

#ifndef RGB_sensor_h
#define RGB_sensor_h

#define RGB_SENSOR_PORT PORTA
#define RGB_SENSOR_R_PIN_bm PIN5_bm
#define RGB_SENSOR_G_PIN_bm PIN6_bm
#define RGB_SENSOR_B_PIN_bm PIN7_bm

uint8_t rsenbase;
uint8_t gsenbase;
uint8_t bsenbase;

void rgb_sensor_init();

void rgb_sensor_enable();
void rgb_sensor_disable();

uint8_t get_red_sensor();
uint8_t get_green_sensor();
uint8_t get_blue_sensor();



#endif
