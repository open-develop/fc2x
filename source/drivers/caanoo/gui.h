#ifndef __GUI_H__
#define __GUI_H__

/////////////////////////////////////////////////
// GUI for Gp2X
//  Author: Luke Arntson
//  Created: 12/5/2010 by Luke Arntson
//  Modified: 12/5/2010 by Luke Arntson
//////////////////////////////////////////////////

#include <vector>
#include <linux/input.h>
#include "joystick.h"

class CanGUI
{
public:
	CanGUI();
	~CanGUI();
	int update();
	void render(unsigned char *screen);
private:
	void clearFiles();  // clear our directory list..
	char curPath[1024]; // Current working directory..
	char nextPath[1024]; // Next directory to move to..
	char fileName[1024]; // Full file name to load
	int state; 	// current GUI state
	std::vector<char *> files;
	bool updateRender;
	struct input_event gJoystick[16]; // up to 16 events..
	void moveUp();
	void moveDown();
	bool northPressed; // don't double press..
	bool southPressed;
	bool bPressed;
	int cursorID;
	int cursorOffset; // For multiple panes..
};

#endif // __GUI_H__
