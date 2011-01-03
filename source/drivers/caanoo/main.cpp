/////////////////////////////////////////////////////////////////
//
// FC2X - FCEUX Caanoo Port
//   Author: Luke Arntson (Cthulhu32)
//   
// FCEUX is an open-source NES emulator by the wonderful
//   programmer ____. Other authors and various licensing
//   information can be found in the /documentation/ folder.
//
/////////////////////////////////////////////////////////////////


// System Libraries
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <math.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <linux/soundcard.h>

// Driver libraries
#include "main.h"
#include "config.h"

// GUI
#include "gui.h"
#include "throttle.h"
#include "joystick.h"

// FCEU libraries
#include "../../fceu.h"
//#include "../../video.cpp"
#include "../../version.h"

#define FB_DEV_PATH	    "/dev/fb0"
#define	MEM_NAME	    "/dev/mem"
#define SND_DEV_PATH	"/dev/dsp"		// audio

enum __LCD_SUB_COMMAND
{
    LCD_POWER_DOWN_ON_CMD,
    LCD_POWER_DOWN_OFF_CMD,
    LCD_GAMMA_SET_CMD,
    LCD_LIGHT_SET_CMD,
    LCD_PANEL_ID_CMD,
	LCD_DIRECTION_ON_CMD,		/* 320 x 240 */
	LCD_DIRECTION_OFF_CMD,		/* 240 x 320 */
};

#define FBIO_MAGIC	'D'
#define FBIO_LCD_CHANGE_CONTROL     _IOW(FBIO_MAGIC, 90, unsigned int[2])

#define SOUND_RATE 22050   /* the sampling rate */
#define SOUND_BITS 16      /* sample size: 8 or 16 bits */
#define SOUND_CHANNELS 2  /* 1 = mono 2 = stereo */
#define SYS_CLK_FREQ 27
#define CAANOO_CLK_DEFAULT 500 // Caanoo sits at 500 mhz

#define TICK_INTERVAL	16666.6667

extern bool oldInputDisplay;

int emuRunning = 1; // Are we still running?
int atMenu = 1;	// Are we currently in the menu?
static int			fb_fd;	// used for Caanoo
static int			fd_mem;

unsigned int frame = 0; // used for FPS counting

unsigned int *		memregl;
#define TIMER_BASE2 0x1900		// Use Timer2 (Timer3 causes a hardware failure..)
#define TIMER_REG(x) memregl[(TIMER_BASE2 + x) >> 2]

bool turbo = false;
int closeFinishedMovie = 0;

int gametype = 0;

int audio_fd;  // memory that points directly to our sound..

static int inited = 0;	

unsigned char scrBuffer[320*240*3]; // screen buffer

Config * g_config;

extern double g_fpsScale;

void ptimer_init(void)
{
        TIMER_REG(0x44) = 0x922;
        TIMER_REG(0x40) = 0x0c;
        TIMER_REG(0x08) = 0x6b;
}

unsigned int ptimer_get_ticks_micro(void)
{
        TIMER_REG(0x08) = 0x4b;  /* run timer, latch value */
        return TIMER_REG(0);
}

unsigned int getTickCount(void)
{
        TIMER_REG(0x08) = 0x4b;  /* run timer, latch value */
        return (unsigned int)(TIMER_REG(0)/1000.0);
}

void ptimer_cleanup(void)
{
        TIMER_REG(0x40) = 0x0c;
        TIMER_REG(0x08) = 0x23;
        TIMER_REG(0x00) = 0;
        TIMER_REG(0x40) = 0;
        TIMER_REG(0x44) = 0;
}

/**
 * Update the video, audio, and input subsystems with the provided
 * video (XBuf) and audio (Buffer) information.
 */
void
FCEUD_Update(uint8 *XBuf,
			 int32 *Buffer,
			 int Count,
			 unsigned char * screenBuffer)
{	
	int ocount = Count;
	// apply frame scaling to Count
	Count = (int)(Count / g_fpsScale);
	if(Count) {
		WriteSound(Buffer, Count);
		if(XBuf && (inited&4) && !(NoWaiting & 2))
			BlitScreen(XBuf, screenBuffer);
	
		/*
		int32 can=GetWriteSound();
		static int uflow=0;
		int32 tmpcan;

		// don't underflow when scaling fps
		if(can >= GetMaxSound() && g_fpsScale==1.0) uflow=1;	// Go into massive underflow mode.

		if(can > Count) can=Count;
		else uflow=0;

		WriteSound(Buffer,can);

		//if(uflow) puts("Underflow");
		tmpcan = GetWriteSound();
		// don't underflow when scaling fps
		if(g_fpsScale>1.0 || ((tmpcan < Count*0.90) && !uflow)) {
			if(XBuf && (inited&4) && !(NoWaiting & 2))
				BlitScreen(XBuf, screenBuffer);
			Buffer+=can;
			Count-=can;
			if(Count) {
				if(NoWaiting) {
					can=GetWriteSound();
					if(Count>can) Count=can;
					WriteSound(Buffer,Count);
				} else {
					while(Count>0) {
						WriteSound(Buffer,(Count<ocount) ? Count : ocount);
						Count -= ocount;
					}
				}
			}
		} //else puts("Skipped");
		*/
	} else {
		//if(!NoWaiting && (!(eoptions&EO_NOTHROTTLE) || FCEUI_EmulationPaused()))
		while (SpeedThrottle())
		{
			FCEUD_UpdateInput();
		}
		if(XBuf && (inited&4)) {
			BlitScreen(XBuf, screenBuffer);
		}
	}
	FCEUD_UpdateInput();
}

static void DoFun(int frameskip, unsigned char * screenBuffer)
{
	uint8 *gfx;
	int32 *sound;
	int32 ssize;
	static int fskipc = 0;
	static int opause = 0;

#ifdef FRAMESKIP
	fskipc = (fskipc + 1) % (frameskip + 1);
#endif

	if(NoWaiting) {
		gfx = 0;
	}
	FCEUI_Emulate(&gfx, &sound, &ssize, fskipc);
	FCEUD_Update(gfx, sound, ssize, screenBuffer);

	if(opause!=FCEUI_EmulationPaused()) {
		opause=FCEUI_EmulationPaused();
		SilenceSound(opause);
	}
}

/**
 * Initialize all of the subsystem drivers: video, audio, and joystick.
 */
static int
DriverInitialize(FCEUGI *gi)
{
	if(InitVideo(gi) < 0)
		return 0;
	
	inited|=4;

	if(InitSound())
		inited|=1;

	if(InitJoysticks())
		inited|=2;

	int fourscore=0;
//	g_config->getOption("SDL.FourScore", &fourscore);
//	eoptions &= ~EO_FOURSCORE;
//	if(fourscore)
//		eoptions |= EO_FOURSCORE;

	InitInputInterface();
	return 1;
}

/**
 * Loads a game, given a full path/filename.  The driver code must be
 * initialized after the game is loaded, because the emulator code
 * provides data necessary for the driver code(number of scanlines to
 * render, what virtual input devices to use, etc.).
 */
int LoadGame(const char *path)
{
	CloseGame();
	if(!FCEUI_LoadGame(path, 1)) {
		printf("Could not load %s\n", path);
		return 0;
	}
	
	printf("FCEU core loaded %s fine!!\n", path);
	
	ParseGIInput(GameInfo);
	RefreshThrottleFPS();

	if(!DriverInitialize(GameInfo)) {
		return(0);
	}
/*	
	// set pal/ntsc
	int id;
	g_config->getOption("SDL.PAL", &id);
	if(id)
		FCEUI_SetVidSystem(1);
	else
		FCEUI_SetVidSystem(0);
*/
	FCEUI_SetVidSystem(0);

	isloaded = 1;
	return 1;
}

int CloseGame()
{
	if(!isloaded) {
		return 0;
	}
	FCEUI_CloseGame();
	//DriverKill();
	isloaded = 0;
	GameInfo = 0;
	
	//InputUserActiveFix();
	return 1;
}

// Return to the Caanoo menu system..
static void returnToMenu()
{
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
}

//The timer
class Timer
{
    private:
    //The clock time when the timer started
    clock_t startTicks;

    //The ticks stored when the timer was paused
    clock_t pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = getTickCount();//SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = getTickCount() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = getTickCount() - pausedTicks;//SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return (getTickCount()-startTicks); //SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

static int loc_set_FCLK(unsigned int clock_in_mhz)
{
  unsigned int v, i;
  unsigned int mdiv, pdiv, sdiv = 0;
  pdiv = 9;
  mdiv = (clock_in_mhz * pdiv) / SYS_CLK_FREQ;
  if (mdiv & ~0x3ff)
    return -1;
  v = (pdiv<<18) | (mdiv<<8) | sdiv;

  memregl[0xf004>>2] = v;
  memregl[0xf07c>>2] |= 0x8000;

 for (i = 0; (memregl[0xf07c>>2] & 0x8000) && i < 0x100000; i++);
}

void cpu_deinit()
{
  loc_set_FCLK( CAANOO_CLK_DEFAULT );
  sleep(1);
}

static void exitCaanoo()
{
	close(audio_fd);
	close(fb_fd);

}

void cpu_init()
{
	if (( fd_mem = open( MEM_NAME, O_RDWR|O_SYNC))<0) {
		perror("open " MEM_NAME " failed");
		exit(-1);
	}
	memregl = (unsigned int*)mmap(0, 0x20000, PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, 0xc0000000);
	loc_set_FCLK( 500 );
}

static void initCaanoo()
{
	unsigned int send[2];
	int status, arg;
	unsigned char *mapped_mem = NULL;

	fb_fd = open(FB_DEV_PATH, O_RDWR);
	
	//  LcdOff...(1)
    send[0] = LCD_LIGHT_SET_CMD;
    send[1] = 0;
    ioctl(fb_fd, FBIO_LCD_CHANGE_CONTROL, send);
	
	// Clear Framebuffer
	mapped_mem = (unsigned char*)mmap(NULL, 320*240*3, PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, 0x5600000);
	memset(mapped_mem, 0x00, (320*240*3));
    munmap( mapped_mem, (320*240*3));
	
	//  LcdOn...
    send[0] = LCD_LIGHT_SET_CMD;
    send[1] = 75;
    ioctl(fb_fd, FBIO_LCD_CHANGE_CONTROL, send);
	
	// Open our audio digital processor
	audio_fd = open( SND_DEV_PATH, O_WRONLY );
	if ( audio_fd < 0 ){
		printf("error: failed to open " SND_DEV_PATH "\n");
		exit(0);
	}
	
	arg = SOUND_BITS;
	status = ioctl(audio_fd, SOUND_PCM_WRITE_BITS, &arg);
	
	arg = SOUND_CHANNELS;
	status = ioctl(audio_fd, SOUND_PCM_WRITE_CHANNELS, &arg);
	
	arg = SOUND_RATE;
	status = ioctl(audio_fd, SOUND_PCM_WRITE_RATE, &arg);
}

// Main function
int main(int argc, char * argv[])
{
	// Overclock is the very first thing we do..
	cpu_init();

	int error;
	CanGUI mGui;
	unsigned int next_time;
	unsigned int now;
	int frameskip = 0; // for now no frame skipping..
	
	unsigned char * mapped_mem = NULL;
	
	Timer fpscounter;
	
	initCaanoo(); // init our caanoo frame buffer
	
	FCEUD_Message("\nStarting "FCEU_NAME_AND_VERSION"...\n");

	g_config = InitConfig();
	
	if ( !g_config )
	{
		fprintf(stderr, "Could not init config.\n");
		goto EXIT;	// leave!
	}
	
	// initialize the infrastructure
	error = FCEUI_Initialize();
	if(error != 1) {
		fprintf(stderr, "Could not initialize FCEU.\n");
		goto EXIT;
	}
	
	printf("Initialized FCEU fine.\n");

	// update the input devices
	UpdateInput(g_config);
	
	mapped_mem = (unsigned char*)mmap(NULL, 320*240*3, PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, 0x5600000);
	
	printf("Initialized Caanoo memory + screen buffer.\n");
	fflush(stdout);
	
	// update the emu core
	UpdateEMUCore(g_config);
	
	ptimer_init(); // set some hardware flags..
	
	next_time = ptimer_get_ticks_micro() + TICK_INTERVAL;

	fpscounter.start();
	
	while(emuRunning)
	{
		if(GameInfo)
		{
			// Count up our FPS counter..
			/*
			frame++;
			if ( fpscounter.get_ticks() > 1000 ) // 1 second
			{
				double framesps = frame / ( fpscounter.get_ticks() / 1000.f );
				printf("FPS:%.2lf\n", framesps);
				fflush(stdout);
				frame = 0;
				fpscounter.start();
			}
			*/
			
			DoFun(frameskip, scrBuffer);
		
			// draw it
			memcpy(mapped_mem, scrBuffer, 320*240*3);
		}
		else
		{	
			// Do some gui crap..
			mGui.update();
			mGui.render(scrBuffer);
			
			// draw it
			memcpy(mapped_mem, scrBuffer, 320*240*3);
			
			now = ptimer_get_ticks_micro();
			if ( next_time > now)
			{
				usleep(next_time - now);
			}
			next_time += TICK_INTERVAL;
		}
	}
	
	cpu_deinit();
	ptimer_cleanup();
	
	munmap( mapped_mem, (320*240*3)); // unmap our frame buffer
	munmap( memregl, 0x20000);
	
EXIT:
	exitCaanoo();
	returnToMenu();
	return 0; // return back to menu..
}


/**
 * Opens a file to be read a byte at a time.
 */
EMUFILE_FILE* FCEUD_UTF8_fstream(const char *fn, const char *m)
{
	std::ios_base::openmode mode = std::ios_base::binary;
	if(!strcmp(m,"r") || !strcmp(m,"rb"))
		mode |= std::ios_base::in;
	else if(!strcmp(m,"w") || !strcmp(m,"wb"))
		mode |= std::ios_base::out | std::ios_base::trunc;
	else if(!strcmp(m,"a") || !strcmp(m,"ab"))
		mode |= std::ios_base::out | std::ios_base::app;
	else if(!strcmp(m,"r+") || !strcmp(m,"r+b"))
		mode |= std::ios_base::in | std::ios_base::out;
	else if(!strcmp(m,"w+") || !strcmp(m,"w+b"))
		mode |= std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
	else if(!strcmp(m,"a+") || !strcmp(m,"a+b"))
		mode |= std::ios_base::in | std::ios_base::out | std::ios_base::app;
    return new EMUFILE_FILE(fn, m);
	//return new std::fstream(fn,mode);
}

/**
 * Opens a file, C++ style, to be read a byte at a time.
 */
FILE *FCEUD_UTF8fopen(const char *fn, const char *mode)
{
	return(fopen(fn,mode));
}

/**
* Prints a textual message without adding a newline at the end.
*
* @param text The text of the message.
*
* TODO: This function should have a better name.
**/
void FCEUD_Message(const char *text)
{
	fputs(text, stdout);
}

/**
* Shows an error message in a message box.
* (For now: prints to stderr.)
*
* @param errormsg Text of the error message.
**/
void FCEUD_PrintError(const char *errormsg)
{
	fprintf(stderr, "%s\n", errormsg);
}

static char *s_linuxCompilerString = "g++ " __VERSION__;
/**
 * Returns the compiler string.
 */
const char *FCEUD_GetCompilerString() {
	return (const char *)s_linuxCompilerString;
}

void FCEUD_DebugBreakpoint() {
	return;
}

// dummy functions

#define DUMMY(__f) void __f(void) {printf("%s\n", #__f); FCEU_DispMessage("Not implemented.",0);}
DUMMY(FCEUD_HideMenuToggle)
DUMMY(FCEUD_MovieReplayFrom)
DUMMY(FCEUD_ToggleStatusIcon)
DUMMY(FCEUD_AviRecordTo)
DUMMY(FCEUD_AviStop)
void FCEUI_AviVideoUpdate(const unsigned char* buffer) { }
int FCEUD_ShowStatusIcon(void) {return 0;}
bool FCEUI_AviIsRecording(void) {return false;}
void FCEUI_UseInputPreset(int preset) { }
bool FCEUD_PauseAfterPlayback() { return false; }
// These are actually fine, but will be unused and overriden by the current UI code.
void FCEUD_TurboOn	(void) { }
void FCEUD_TurboOff   (void) {}
void FCEUD_TurboToggle(void) {}
FCEUFILE* FCEUD_OpenArchiveIndex(ArchiveScanRecord& asr, std::string &fname, int innerIndex) { return 0; }
FCEUFILE* FCEUD_OpenArchive(ArchiveScanRecord& asr, std::string& fname, std::string* innerFilename) { return 0; }
ArchiveScanRecord FCEUD_ScanArchive(std::string fname) { return ArchiveScanRecord(); }
