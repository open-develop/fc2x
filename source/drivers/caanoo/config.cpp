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

#include "main.h"
//#include "throttle.h"
#include "config.h"

#include "../common/cheat.h"
#include "../../types.h"

#include "input.h"
#include "dface.h"
/*
#include "sdl.h"
#include "sdl-video.h"
#include "unix-netplay.h"
*/

/**
 * Read a custom pallete from a file and load it into the core.
 */
int
LoadCPalette(const std::string &file)
{
    uint8 tmpp[192];
    FILE *fp;

    if(!(fp = FCEUD_UTF8fopen(file.c_str(), "rb"))) {
        printf(" Error loading custom palette from file: %s\n", file.c_str());
        return 0;
    }
    size_t result = fread(tmpp, 1, 192, fp);
    if(result != 192) {
		printf(" Error reading custom palette from file: %s\n", file.c_str());
		return 0;
	}
    FCEUI_SetPaletteArray(tmpp);
    fclose(fp);
    return 1;
}

/**
 * Creates the subdirectories used for saving snapshots, movies, game
 * saves, etc.  Hopefully obsolete with new configuration system.
 */
static void
CreateDirs(const std::string &dir)
{
    char *subs[7]={"fcs","snaps","gameinfo","sav","cheats","movies"};
    std::string subdir;
    int x;

    mkdir(dir.c_str(), S_IRWXU);
    for(x = 0; x < 6; x++) {
        subdir = dir + PSS + subs[x];
        mkdir(subdir.c_str(), S_IRWXU);
    }
}

/**
 * Attempts to locate FCEU's application directory.  This will
 * hopefully become obsolete once the new configuration system is in
 * place.
 */
static void
GetBaseDirectory(std::string &dir)
{
    char *home = getenv("HOME");
    if(home) {
        dir = std::string(home) + "/.fceux";
    } else {

        dir = "";
    }
}


Config * InitConfig()
{
    std::string dir, prefix;
    Config *config;

    GetBaseDirectory(dir);

    FCEUI_SetBaseDirectory(dir.c_str());
    CreateDirs(dir);

    config = new Config(dir);

    // sound options
    config->addOption('s', "sound", "Caanoo.Sound", 1);
    config->addOption("volume", "Caanoo.Sound.Volume", 150);
    config->addOption("trianglevol", "Caanoo.Sound.TriangleVolume", 256);
    config->addOption("square1vol", "Caanoo.Sound.Square1Volume", 256);
    config->addOption("square2vol", "Caanoo.Sound.Square2Volume", 256);
    config->addOption("noisevol", "Caanoo.Sound.NoiseVolume", 256);
    config->addOption("pcmvol", "Caanoo.Sound.PCMVolume", 256);
    config->addOption("soundrate", "Caanoo.Sound.Rate", 44100);
    config->addOption("soundq", "Caanoo.Sound.Quality", 1);
    config->addOption("soundrecord", "Caanoo.Sound.RecordFile", "");
    config->addOption("soundbufsize", "Caanoo.Sound.BufSize", 128);
	config->addOption("lowpass", "Caanoo.Sound.LowPass", 0);
    
    config->addOption('g', "gamegenie", "Caanoo.GameGenie", 0);
    config->addOption("pal", "Caanoo.PAL", 0);
    config->addOption("frameskip", "Caanoo.Frameskip", 0);
    config->addOption("clipsides", "Caanoo.ClipSides", 0);
    config->addOption("nospritelim", "Caanoo.DisableSpriteLimit", 1);

    // color control
    config->addOption('p', "palette", "Caanoo.Palette", "");
    config->addOption("tint", "Caanoo.Tint", 56);
    config->addOption("hue", "Caanoo.Hue", 72);
    config->addOption("ntsccolor", "Caanoo.NTSCpalette", 0);

    // scanline settings
    config->addOption("slstart", "Caanoo.ScanLineStart", 0);
    config->addOption("slend", "Caanoo.ScanLineEnd", 239);

    // video controls
    config->addOption('x', "xres", "Caanoo.XResolution", 512);
    config->addOption('y', "yres", "Caanoo.YResolution", 448);
    config->addOption('f', "fullscreen", "Caanoo.Fullscreen", 0);
    config->addOption('b', "bpp", "Caanoo.BitsPerPixel", 32);
    config->addOption("doublebuf", "Caanoo.DoubleBuffering", 0);
    config->addOption("autoscale", "Caanoo.AutoScale", 1);
    config->addOption("keepratio", "Caanoo.KeepRatio", 1);
    config->addOption("xscale", "Caanoo.XScale", 1.0);
    config->addOption("yscale", "Caanoo.YScale", 1.0);
    config->addOption("xstretch", "Caanoo.XStretch", 0);
    config->addOption("ystretch", "Caanoo.YStretch", 0);
    config->addOption("noframe", "Caanoo.NoFrame", 0);
    config->addOption("special", "Caanoo.SpecialFilter", 0);

	/*
    // network play options - netplay is broken
    config->addOption("server", "SDL.NetworkIsServer", 0);
    config->addOption('n', "net", "SDL.NetworkIP", "");
    config->addOption('u', "user", "SDL.NetworkUsername", "");
    config->addOption('w', "pass", "SDL.NetworkPassword", "");
    config->addOption('k', "netkey", "SDL.NetworkGameKey", "");
    config->addOption("port", "SDL.NetworkPort", 4046);
    config->addOption("players", "SDL.NetworkPlayers", 1);
    */
	
    // input configuration options
    config->addOption("input1", "Caanoo.Input.0", "GamePad.0");
    config->addOption("input2", "Caanoo.Input.1", "GamePad.1");
    config->addOption("input3", "Caanoo.Input.2", "Gamepad.2");
    config->addOption("input4", "Caanoo.Input.3", "Gamepad.3");

    // allow for input configuration
    config->addOption('i', "inputcfg", "Caanoo.InputCfg", InputCfg);
    
    // display input
    config->addOption("inputdisplay", "Caanoo.InputDisplay", 0);
    
    // pause movie playback at frame x
    config->addOption("pauseframe", "Caanoo.PauseFrame", 0);
    config->addOption("moviemsg", "Caanoo.MovieMsg", 1);
    
    // overwrite the config file?
    config->addOption("no-config", "Caanoo.NoConfig", 0);
    
    // video playback
    config->addOption("playmov", "Caanoo.Movie", "");
    config->addOption("subtitles", "Caanoo.SubtitleDisplay", 1);
	
	config->addOption("fourscore", "Caanoo.FourScore", 0);

    // fcm -> fm2 conversion
    config->addOption("fcmconvert", "Caanoo.FCMConvert", "");
    
    // fm2 -> srt conversion
    config->addOption("ripsubs", "Caanoo.RipSubs", "");
	
	// enable new PPU core
	config->addOption("newppu", "Caanoo.NewPPU", 0);

    // GamePad 0 - 3
    for(unsigned int i = 0; i < GAMEPAD_NUM_DEVICES; i++) {
        char buf[64];
        snprintf(buf, 20, "Caanoo.Input.GamePad.%d.", i);
        prefix = buf;

        config->addOption(prefix + "DeviceType", DefaultGamePadDevice[i]);
        config->addOption(prefix + "DeviceNum",  0);
        for(unsigned int j = 0; j < GAMEPAD_NUM_BUTTONS; j++) {
            config->addOption(prefix + GamePadNames[j], DefaultGamePad[i][j]);
        }
    }
    
    // PowerPad 0 - 1
    for(unsigned int i = 0; i < POWERPAD_NUM_DEVICES; i++) {
        char buf[64];
        snprintf(buf, 20, "Caanoo.Input.PowerPad.%d.", i);
        prefix = buf;

        config->addOption(prefix + "DeviceType", DefaultPowerPadDevice[i]);
        config->addOption(prefix + "DeviceNum",  0);
        for(unsigned int j = 0; j < POWERPAD_NUM_BUTTONS; j++) {
            config->addOption(prefix +PowerPadNames[j], DefaultPowerPad[i][j]);
        }
    }

    // QuizKing
    prefix = "Caanoo.Input.QuizKing.";
    config->addOption(prefix + "DeviceType", DefaultQuizKingDevice);
    config->addOption(prefix + "DeviceNum", 0);
    for(unsigned int j = 0; j < QUIZKING_NUM_BUTTONS; j++) {
        config->addOption(prefix + QuizKingNames[j], DefaultQuizKing[j]);
    }

    // HyperShot
    prefix = "Caanoo.Input.HyperShot.";
    config->addOption(prefix + "DeviceType", DefaultHyperShotDevice);
    config->addOption(prefix + "DeviceNum", 0);
    for(unsigned int j = 0; j < HYPERSHOT_NUM_BUTTONS; j++) {
        config->addOption(prefix + HyperShotNames[j], DefaultHyperShot[j]);
    }

    // Mahjong
    prefix = "Caanoo.Input.Mahjong.";
    config->addOption(prefix + "DeviceType", DefaultMahjongDevice);
    config->addOption(prefix + "DeviceNum", 0);
    for(unsigned int j = 0; j < MAHJONG_NUM_BUTTONS; j++) {
        config->addOption(prefix + MahjongNames[j], DefaultMahjong[j]);
    }

    // TopRider
    prefix = "Caanoo.Input.TopRider.";
    config->addOption(prefix + "DeviceType", DefaultTopRiderDevice);
    config->addOption(prefix + "DeviceNum", 0);
    for(unsigned int j = 0; j < TOPRIDER_NUM_BUTTONS; j++) {
        config->addOption(prefix + TopRiderNames[j], DefaultTopRider[j]);
    }

    // FTrainer
    prefix = "Caanoo.Input.FTrainer.";
    config->addOption(prefix + "DeviceType", DefaultFTrainerDevice);
    config->addOption(prefix + "DeviceNum", 0);
    for(unsigned int j = 0; j < FTRAINER_NUM_BUTTONS; j++) {
        config->addOption(prefix + FTrainerNames[j], DefaultFTrainer[j]);
    }

    // FamilyKeyBoard
    prefix = "Caanoo.Input.FamilyKeyBoard.";
    config->addOption(prefix + "DeviceType", DefaultFamilyKeyBoardDevice);
    config->addOption(prefix + "DeviceNum", 0);
    for(unsigned int j = 0; j < FAMILYKEYBOARD_NUM_BUTTONS; j++) {
        config->addOption(prefix + FamilyKeyBoardNames[j],
                          DefaultFamilyKeyBoard[j]);
    }

    // for FAMICOM microphone in pad 2 pad 1 didn't have it
    // Takeshi no Chousenjou uses it for example.
    prefix = "Caanoo.Input.FamicomPad2.";
    config->addOption("rp2mic", prefix + "EnableMic", 0);
    
/*
    const int Hotkeys[HK_MAX] = {
		SDLK_F1, // cheat menu
		SDLK_F2, // bind state
		SDLK_F3, // load lua
		SDLK_F4, // toggleBG
		SDLK_F5, // save state
		SDLK_F6, // fds select
		SDLK_F7, // load state
		SDLK_F8, // fds eject
		SDLK_F6, // VS insert coin
		SDLK_F8, // VS toggle dipswitch
		SDLK_PERIOD, // toggle frame display
		SDLK_F10, // toggle subtitle
		SDLK_F11, // reset
		SDLK_F12, // screenshot
		SDLK_PAUSE, // pause
		SDLK_MINUS, // speed++
		SDLK_EQUALS, // speed--
		SDLK_BACKSLASH, //frame advnace
		SDLK_TAB, // turbo
		SDLK_COMMA, // toggle input display
		SDLK_q, // toggle movie RW
		SDLK_QUOTE, // toggle mute capture
		0, // quit
		SDLK_DELETE, // frame advance lag skip
		SDLK_SLASH, // lag counter display
		SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
		SDLK_6, SDLK_7, SDLK_8, SDLK_9}; 
	
	prefix = "SDL.Hotkeys.";	
    for(int i=0; i < HK_MAX; i++)
		config->addOption(prefix + HotkeyStrings[i], Hotkeys[i]);
*/
		
    // All mouse devices
    config->addOption("Caanoo.OekaKids.0.DeviceType", "Mouse");
    config->addOption("Caanoo.OekaKids.0.DeviceNum", 0);

    config->addOption("Caanoo.Arkanoid.0.DeviceType", "Mouse");
    config->addOption("Caanoo.Arkanoid.0.DeviceNum", 0);

    config->addOption("Caanoo.Shadow.0.DeviceType", "Mouse");
    config->addOption("Caanoo.Shadow.0.DeviceNum", 0);

    config->addOption("Caanoo.Zapper.0.DeviceType", "Mouse");
    config->addOption("Caanoo.Zapper.0.DeviceNum", 0);

    return config;
}

void UpdateEMUCore(Config *config)
{
	FCEUI_SetNTSCTH(0, 56, 72);
	FCEUI_SetVidSystem(0); // NTSC = 0, PAL = 1
	FCEUI_SetGameGenie(0);
	FCEUI_SetLowPass(0);
	FCEUI_DisableSpriteLimitation(1);
    FCEUI_SetRenderedLines(8, 231, 0, 239);
}
