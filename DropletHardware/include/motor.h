#include <avr/io.h>

#ifndef motor_h
#define motor_h

#define MOTOR_STATUS_DIRECTION		0x07
#define MOTOR_STATUS_ON				0x80

#define NORTH				0
#define NORTHEAST			1
#define SOUTHEAST			2
#define SOUTH				3 
#define SOUTHWEST			4
#define NORTHWEST			5

#define CLOCKWISE			6
#define COUNTERCLOCKWISE	7

// Timing for taking a step:
#define MOTOR_ON_TIME			30
#define MOTOR_OFF_TIME			30

#include "droplet_init.h"
#include "scheduler.h"

volatile uint8_t motor_status; // [ on, cancel, 0, 0, 0, direction(2-0) ] 
volatile Task_t* current_motor_task;

int16_t motor_on_time;
int16_t motor_off_time;

int16_t motor_adjusts[8][3];
//int8_t motor_signs[8][3];

uint16_t mm_per_kilostep[8]; //For the spin directions, this is degrees per kilostep.

// Sets up the timers for the motors PWM, pins to motor controller, and 
// reads the motor settings from non-volatile memory (user signature row)
void	motor_init();

// Walk in specified direction for specified number of steps
// direction (0-7, see #defines above for which direction maps to what number)
uint8_t	move_steps(uint8_t direction, uint16_t num_steps);

void walk(uint8_t direction, uint16_t mm);

// Stops all motors
void stop();

uint8_t is_moving(void); // returns 0 if droplet is not moving, otherwise returns the direction of motion (1-6)

uint16_t	get_mm_per_kilostep(uint8_t direction);
void		set_mm_per_kilostep(uint8_t direction, uint16_t dist);
void		read_motor_settings();
void		write_motor_settings();
void		print_motor_values();
void		broadcast_motor_adjusts();
void		print_dist_per_step();
void		broadcast_dist_per_step();
uint16_t	get_mm_per_kilostep(uint8_t direction);
void		set_mm_per_kilostep(uint8_t direction, uint16_t dist);

static inline void motor_forward(uint8_t num)
{
	switch(num)
	{
		case 0: TCC0.CTRLB = TC_WGMODE_SS_gc | TC0_CCBEN_bm; PORTC.PIN1CTRL = PORT_INVEN_bm; PORTC.OUTSET |= PIN0_bm; break;
		case 1: TCC1.CTRLB = TC_WGMODE_SS_gc | TC1_CCBEN_bm; PORTC.PIN5CTRL = PORT_INVEN_bm; PORTC.OUTSET |= PIN4_bm; break;
		case 2: TCE0.CTRLB = TC_WGMODE_SS_gc | TC0_CCBEN_bm;PORTE.PIN1CTRL = PORT_INVEN_bm; PORTE.OUTSET |= PIN0_bm; break;
	}
}

static inline void motor_backward(uint8_t num)
{
	switch(num)
	{
		case 0: TCC0.CTRLB = TC_WGMODE_SS_gc | TC0_CCAEN_bm; PORTC.PIN0CTRL = PORT_INVEN_bm; PORTC.OUTSET |= PIN1_bm; break;
		case 1: TCC1.CTRLB = TC_WGMODE_SS_gc | TC1_CCAEN_bm; PORTC.PIN4CTRL = PORT_INVEN_bm; PORTC.OUTSET |= PIN5_bm; break;
		case 2: TCE0.CTRLB = TC_WGMODE_SS_gc | TC0_CCAEN_bm; PORTE.PIN0CTRL = PORT_INVEN_bm; PORTE.OUTSET |= PIN1_bm; break;
	}
}

#endif
