#include "gui.h"
#include "main.h" // For load game?
#include "vincent.h"

#include <dirent.h>
#include <math.h>
#include <algorithm>

#define CURSOR_ASCII ">"

enum
{
	STATE_GETCWD = 0,	// Get the current working directory
	STATE_GETDIR,		// Listing the current directory (needed?)
	STATE_MOVING,		// Moving to the [next] directory
	STATE_LOAD,			// Load the file listed
	STATE_IDLE			// Don't do anything		
};

CanGUI::CanGUI()
{
	// Create gui
	state = STATE_GETCWD; // Get the current working directory
	updateRender = true; // we want to update what the user sees
	
	open_joystick();

	northPressed = southPressed = bPressed = false;
	cursorID = 0; // where to show the >
	cursorOffset = 0; // offset (is our ID >= 27)
	
	chdir("/mnt/sd/game/fc2x/");		// Go to this path for now, should come from an INI
}

CanGUI::~CanGUI()
{
	printf("Closing joystick?\n");
	close_joystick();
	clearFiles();
}

void CanGUI::clearFiles()
{
	// Destroy gui components
	for(int i = 0; i < files.size(); i++)
	{
		free(files[i]); // clear the memory
	}
	files.clear(); // clean the vector
}

static bool stringCompare( const char * left, const char * right ){
	for(int i =0; i < strlen(left) && i < strlen(right); ++i)
	{
		if ( tolower(left[i]) < tolower(right[i]))
			return true;
		else if ( tolower(left[i]) > tolower(right[i]))
			return false;
	}
	if ( strlen(left) < strlen(right))
		return true;
	return false;
}

int CanGUI::update()
{
	switch(state)
	{
		case STATE_GETCWD:
			getcwd(curPath, 1024);	// get the current working directory
			state = STATE_GETDIR;
		break;
		case STATE_GETDIR:
			DIR * dp;
			DIR * tempdp;
			struct dirent *dirp;
			if((dp = opendir(curPath)) == NULL){
				fprintf(stderr, "Cannot open %s\n", curPath);
				state = STATE_IDLE;
				return -1;
			}
			clearFiles(); // remove the previous entries
			while (( dirp = readdir(dp)) != NULL) {
				char * newString;
				tempdp = opendir(dirp->d_name);
				if ( tempdp == NULL)
				{
					newString = (char*)malloc(dirp->d_reclen+1); // Leave one extra for our > cursor
					strncpy(newString, dirp->d_name, dirp->d_reclen);
				}
				else
				{
					newString = (char*)malloc(dirp->d_reclen+2); // Leave one extra for our > cursor
					strcpy(newString, "/");
					strncat(newString, dirp->d_name, dirp->d_reclen);
					closedir(tempdp);
				}
				
				files.push_back(newString);
			}
			closedir(dp);
			std::sort( files.begin(), files.end(), stringCompare );
			
			// Add the initial cursor
			if ( files.size() > 0 )
			{
				char temp[1024];
				strcpy(temp, files[0]);
				strcpy(files[0], CURSOR_ASCII);
				strcat(files[0], temp);
				cursorID = 0; // set to file 0
				cursorOffset = 0; // set the offset to 0
			}
			updateRender = true; // draw again
			state = STATE_IDLE;
		break;
		case STATE_LOAD:
			LoadGame(nextPath);
			frame = 0;				// reset the FPS counter frame..
			state = STATE_GETCWD; // wait.. i guess.. should be a separate state here..
		break;
		case STATE_IDLE:
			update_joystick();
			if ( buttonsNow & (1<<CAN_BTN_HOME) )
			{
				emuRunning = 0; // stop the emulator..
				return 0;
			}
			
			if ( northPressed == false )
			{
				if ( buttonsNow & (1<<CAN_BTN_UP) )
				{
					moveUp();
					northPressed = true;
				}
			}
			else
			{
				if ( (buttonsHeld & (1<<CAN_BTN_UP)) == 0)
					northPressed = false;
			}
			if ( southPressed == false )
			{
				if ( buttonsNow & (1<<CAN_BTN_DOWN) )
				{
					moveDown();
					southPressed = true;
				}
			}
			else
			{
				if ( (buttonsHeld & (1<<CAN_BTN_DOWN)) == 0)
					southPressed = false;
			}
			if ( bPressed == false )
			{
				if ( buttonsNow & (1<<CAN_BTN_B) )
				{
					bPressed = true;
					// Change directory!
					if ( files[cursorID][1] == '/')
					{
						if ( strcmp(&(files[cursorID][1]), "/..")==0 && strcmp(curPath, "/mnt/sd") == 0 )
						{
							printf("Not going to move back beyond the sd card.\n");
							break;
						}
						strncpy(nextPath, &(files[cursorID][2]), 1024);
						chdir(nextPath); // go to this path
						state = STATE_GETCWD;
					}
					else
					{
						strncpy(nextPath, &(files[cursorID][1]), 1024);
						printf("Load file: %s\n", nextPath);
						state = STATE_LOAD;
					}
				}
			}
			else
			{
				if ( (buttonsHeld & (1<<CAN_BTN_B)) == 0)
					bPressed = false;
			}
		break;
		default:
		break;
	};
	
	return 0;
}

void CanGUI::moveUp()
{
	if ( cursorID > 0 )	// Do not move past the top
	{
		// Remove cursor
		char temp[1024];
		strcpy(temp, &(files[cursorID][1]));
		strcpy(files[cursorID], temp);
		
		cursorID--; // move up one
		if ( (cursorID - cursorOffset) < 0)
			cursorOffset--; // move our offset up one
		
		// Render cursor
		strcpy(temp, files[cursorID]);
		strcpy(files[cursorID], CURSOR_ASCII);
		strcat(files[cursorID], temp);
		
		updateRender = true;
	}
}

void CanGUI::moveDown()
{
	if ( cursorID < (files.size()-1) )
	{
		// Remove cursor
		char temp[1024];
		strcpy(temp, &(files[cursorID][1]));
		strcpy(files[cursorID], temp);
		
		cursorID++; // move up one
		if ( (cursorID - cursorOffset) > 26)
			cursorOffset++; // move our offset
		
		// Render cursor
		strcpy(temp, files[cursorID]);
		strcpy(files[cursorID], CURSOR_ASCII);
		strcat(files[cursorID], temp);

		updateRender = true;
	}
}

// X,Y signify the start of the letter
static void RenderLetter(char letter, int sx, int sy, unsigned char * mapped_mem)
{
	int x,xx,y,yy;

	for(y=sy, yy=0; y<sy+8; ++y, yy=y*960)
	{ 
		for(x=sx, xx = x*3; x<sx+8; x++, xx = x*3)
		{
			// Pixels are stored in left to right high-low order. so 0x3F = 0011 1111.. 8 pixels wide
			if ( vincent_data[letter][(y-sy)] & (1<<(7-(x-sx))) )
			{
				mapped_mem[yy + xx] = 0xff; // R
				mapped_mem[yy + xx + 1] = 0xff; // G
				mapped_mem[yy + xx + 2] = 0xff; // B
				//((unsigned int*)screen->pixels)[y*(screen->pitch/4)+x] = 0xFFFFFFFF; // white!
			}
		}
	}
}

static void GuiPrint(const char * string, int x, int y, unsigned char * mapped_mem)
{
	int i=0;
	int j=strlen(string);
	while(i<j)
	{
		RenderLetter(string[i], x+i*8, y, mapped_mem);
		i++;
	}
}

void CanGUI::render(unsigned char *mapped_mem)
{
	if ( updateRender )
	{
		int i; // should start at some offset..
		
		// Draw our screen black
		memset(mapped_mem, 0x0, 0x38400);
		
		// Draw the title
		GuiPrint("=== FC2X Test Gui (Super Alpha) ===", 20, 6, mapped_mem);
		
		// Draw the list of files
		GuiPrint("*Current:", 8, 14, mapped_mem);
		GuiPrint(curPath, 80, 14, mapped_mem);
		
		// List everything in our file list..
		for(i = cursorOffset; i < files.size() && i < cursorOffset+27; i++)
		{
			GuiPrint(files[i], 3, 22+((i-cursorOffset)*8), mapped_mem);
		}
		
		updateRender = false; // don't need to render again
	}
	else if ( state == STATE_LOAD )
	{
		// Black it all out..
		memset(mapped_mem, 0x0, 0x38400);
	}
}
