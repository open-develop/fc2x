#include <stdio.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "joystick.h"

static int joystick_fd = -1;
static int s_jinited = 0;

int InitJoysticks()
{
	if ( s_jinited == 0)
	{
		open_joystick();
		if ( joystick_fd == -1 )
			return(0);
	}

    s_jinited = 1;
    return 1;
}

int open_joystick()
{
	joystick_fd = open(JOYSTICK_DEVNAME, O_RDONLY | O_NONBLOCK ); /* read write for force feedback? */
	if (joystick_fd < 0)
	{
		printf("Could not open joystick %s\n", JOYSTICK_DEVNAME);
	}
	
	/* maybe ioctls to interrogate features here? */
	return joystick_fd;
}
	
void close_joystick()
{
	close(joystick_fd);
}
	
int buttonsPrev = 0;
int buttonsNow;			// What keys are just now being pressed..
int buttonsHeld = 0;	// What keys are currently being held
	
int update_joystick()
{
	int i, rbytes;
	struct input_event event[16];
	
	buttonsNow = 0;
	
	if (joystick_fd < 0)
		return -1;
	
	rbytes = read(joystick_fd, &event, sizeof(event));
	if (rbytes <= 0)
	{
		return 0;
	}
	
	for (i=0; i<rbytes/sizeof(struct input_event); i++) 
	{
		switch (event[i].type) 
		{
			case EV_ABS: 
				switch (event[i].code) 
				{			
					case ABS_X:
						if ( event[i].value < 0x20 )
							buttonsNow |= (1<<CAN_BTN_LEFT);
						else if ( event[i].value > 0xDF )
							buttonsNow |= (1<<CAN_BTN_RIGHT);
						else
						{
							buttonsHeld &= ~(1<<CAN_BTN_LEFT);
							buttonsHeld &= ~(1<<CAN_BTN_RIGHT);
						}
					break;
					case ABS_Y:
						if ( event[i].value < 0x20 )
							buttonsNow |= (1<<CAN_BTN_UP);
						else if ( event[i].value > 0xDF )
							buttonsNow |= (1<<CAN_BTN_DOWN);
						else
						{
							buttonsHeld &= ~(1<<CAN_BTN_UP);
							buttonsHeld &= ~(1<<CAN_BTN_DOWN);
						}
					break;
					default   : 
						printf("type = 0x%x, code = 0x%x\n", event[i].type, event[i].code);
				}
				break;
			case EV_KEY:
				if (event[i].value)
				{
					switch(event[i].code)
					{
						case CAANOO_BUTTON_A:
							buttonsNow |= (1<<CAN_BTN_A);
						break;
						case CAANOO_BUTTON_X:
							buttonsNow |= (1<<CAN_BTN_X);
						break;
						case CAANOO_BUTTON_B:
							buttonsNow |= (1<<CAN_BTN_B);
						break;
						case CAANOO_BUTTON_Y:
							buttonsNow |= (1<<CAN_BTN_Y);
						break;
						case CAANOO_BUTTON_L:
							buttonsNow |= (1<<CAN_BTN_L);
						break;
						case CAANOO_BUTTON_R:
							buttonsNow |= (1<<CAN_BTN_R);
						break;
						case CAANOO_BUTTON_HOME:
							buttonsNow |= (1<<CAN_BTN_HOME);
						break;
						case CAANOO_BUTTON_LOCK:
							buttonsNow |= (1<<CAN_BTN_HOLD);
						break;
						case CAANOO_BUTTON_I:
							buttonsNow |= (1<<CAN_BTN_HELP1);
						break;
						case CAANOO_BUTTON_II:
							buttonsNow |= (1<<CAN_BTN_HELP2);
						break;
						case CAANOO_BUTTON_THUMB:
							buttonsNow |= (1<<CAN_BTN_TACT);
						break;
						default:
						break;
					};
				}
				else
				{
					switch(event[i].code)
					{
						case CAANOO_BUTTON_A:
							buttonsHeld &= ~(1<<CAN_BTN_A);
						break;
						case CAANOO_BUTTON_X:
							buttonsHeld &= ~(1<<CAN_BTN_X);
						break;
						case CAANOO_BUTTON_B:
							buttonsHeld &= ~(1<<CAN_BTN_B);
						break;
						case CAANOO_BUTTON_Y:
							buttonsHeld &= ~(1<<CAN_BTN_Y);
						break;
						case CAANOO_BUTTON_L:
							buttonsHeld &= ~(1<<CAN_BTN_L);
						break;
						case CAANOO_BUTTON_R:
							buttonsHeld &= ~(1<<CAN_BTN_R);
						break;
						case CAANOO_BUTTON_HOME:
							buttonsHeld &= ~(1<<CAN_BTN_HOME);
						break;
						case CAANOO_BUTTON_LOCK:
							buttonsHeld &= ~(1<<CAN_BTN_HOLD);
						break;
						case CAANOO_BUTTON_I:
							buttonsHeld &= ~(1<<CAN_BTN_HELP1);
						break;
						case CAANOO_BUTTON_II:
							buttonsHeld &= ~(1<<CAN_BTN_HELP2);
						break;
						case CAANOO_BUTTON_THUMB:
							buttonsHeld &= ~(1<<CAN_BTN_TACT);
						break;
						default:
						break;
					};
				}
				break;
			default:
				if (event[i].type)
					printf("type = 0x%x, code = 0x%x\n", event[i].type, event[i].code);
				break;
		}
	}
	
	buttonsHeld = buttonsHeld | buttonsNow;
	buttonsPrev = buttonsNow;
	
	return 0;
}
