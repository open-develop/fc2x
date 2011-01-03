/* 
	(C) Copyright 2007,2008, Stephen M. Cameron.

	This file is part of wordwarvi.

	wordwarvi is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	wordwarvi is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with wordwarvi; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__
	
#include <stdio.h>
	
#define JOYSTICK_DEVNAME "/dev/input/event1"

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

// Raw input values..
#define CAANOO_BUTTON_A			0x120
#define CAANOO_BUTTON_X			0x121
#define CAANOO_BUTTON_B			0x122
#define CAANOO_BUTTON_Y			0x123
#define CAANOO_BUTTON_L			0x124
#define CAANOO_BUTTON_R			0x125
#define CAANOO_BUTTON_HOME		0x126
#define CAANOO_BUTTON_LOCK		0x127
#define CAANOO_BUTTON_I			0x128
#define CAANOO_BUTTON_II		0x129
#define CAANOO_BUTTON_THUMB		0x12A	// Press down on the thumb stick..

enum _buttons {
    CAN_BTN_A = 0,     //       A /             1
    CAN_BTN_X = 1,     //       X /             2
    CAN_BTN_B = 2,     //       B /             3
    CAN_BTN_Y = 3,     //       Y /             4
    CAN_BTN_L = 4,     //       L /         5, L1
    CAN_BTN_R = 5,     //       R /         6, L2
    CAN_BTN_HOME = 6,  //    Home /         7, R1
    CAN_BTN_HOLD = 7,  //    Hold /         8, R2
    CAN_BTN_HELP1 = 8, //  Help I /        Select
    CAN_BTN_HELP2 = 9, // Help II /         Start
    CAN_BTN_TACT = 10, //    Tact / L Thumb Stick
    CAN_BTN_UP = 11, 
    CAN_BTN_DOWN = 12,
    CAN_BTN_LEFT = 13,
    CAN_BTN_RIGHT = 14
};

struct js_event {
	unsigned int time;      /* event timestamp in milliseconds */
	short value;   /* value */
	unsigned char type;     /* event type */
	unsigned char number;   /* axis/button number */
};
/*
struct wwvi_js_event {
	int button[11];
	int stick_x;
	int stick_y;
};
*/

extern int buttonsPrev;	// What was pressed last time
extern int buttonsNow;	// What was just pressed
extern int buttonsHeld;	// What keys are currently being held

extern int open_joystick();
extern int read_joystick_event(struct js_event *jse);
extern void set_joystick_y_axis(int axis);
extern void set_joystick_x_axis(int axis);
extern void close_joystick();
extern int update_joystick(); // update all our buttons
//extern int get_joystick_status(int*);
	
extern int InitJoysticks();
	
#endif
