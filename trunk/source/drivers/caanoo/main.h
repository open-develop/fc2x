#ifndef __MAIN_H__
#define __MAIN_H__

#include "../../driver.h"
#include "dface.h"

extern int emuRunning;
extern int atMenu;

extern int NoWaiting;

extern int audio_fd;

static void DoFun(int frameskip);
static int isloaded = 0;

int LoadGame(const char *path);
int CloseGame(void);
unsigned int getTickCount();

extern unsigned int frame;

// Device management defaults
#define NUM_INPUT_DEVICES 3

// GamePad defaults
#define GAMEPAD_NUM_DEVICES 4
#define GAMEPAD_NUM_BUTTONS 10
extern const char *GamePadNames[GAMEPAD_NUM_BUTTONS];
extern const char *DefaultGamePadDevice[GAMEPAD_NUM_DEVICES];
extern const int DefaultGamePad[GAMEPAD_NUM_DEVICES][GAMEPAD_NUM_BUTTONS];

// PowerPad defaults
#define POWERPAD_NUM_DEVICES 2
#define POWERPAD_NUM_BUTTONS 12
extern const char *PowerPadNames[POWERPAD_NUM_BUTTONS];
extern const char *DefaultPowerPadDevice[POWERPAD_NUM_DEVICES];
extern const int DefaultPowerPad[POWERPAD_NUM_DEVICES][POWERPAD_NUM_BUTTONS];

// QuizKing defaults
#define QUIZKING_NUM_BUTTONS 6
extern const char *QuizKingNames[QUIZKING_NUM_BUTTONS];
extern const char *DefaultQuizKingDevice;
extern const int DefaultQuizKing[QUIZKING_NUM_BUTTONS];

// HyperShot defaults
#define HYPERSHOT_NUM_BUTTONS 4
extern const char *HyperShotNames[HYPERSHOT_NUM_BUTTONS];
extern const char *DefaultHyperShotDevice;
extern const int DefaultHyperShot[HYPERSHOT_NUM_BUTTONS];

// Mahjong defaults
#define MAHJONG_NUM_BUTTONS 21
extern const char *MahjongNames[MAHJONG_NUM_BUTTONS];
extern const char *DefaultMahjongDevice;
extern const int DefaultMahjong[MAHJONG_NUM_BUTTONS];

// TopRider defaults
#define TOPRIDER_NUM_BUTTONS 8
extern const char *TopRiderNames[TOPRIDER_NUM_BUTTONS];
extern const char *DefaultTopRiderDevice;
extern const int DefaultTopRider[TOPRIDER_NUM_BUTTONS];

// FTrainer defaults
#define FTRAINER_NUM_BUTTONS 12
extern const char *FTrainerNames[FTRAINER_NUM_BUTTONS];
extern const char *DefaultFTrainerDevice;
extern const int DefaultFTrainer[FTRAINER_NUM_BUTTONS];

// FamilyKeyBoard defaults
#define FAMILYKEYBOARD_NUM_BUTTONS 0x48
extern const char *FamilyKeyBoardNames[FAMILYKEYBOARD_NUM_BUTTONS];
extern const char *DefaultFamilyKeyBoardDevice;
extern const int DefaultFamilyKeyBoard[FAMILYKEYBOARD_NUM_BUTTONS];

#endif __MAIN_H__
