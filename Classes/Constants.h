
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "cocos2d.h"

USING_NS_CC;

#define  DEBUG_ALL_LEVEL
#define  DEBUG_BOX2D


class LHSprite;

extern const int STAGE_COUNT;
extern const int LEVEL_COUNT;
extern const int PER_LEVEL_COUNT;
extern const int CHARACTER_TYPE_COUNT;
extern const int INIT_PLAYER_LIFE;
extern const int COINPERLEVEL;
#define THROW_BOMB                  11
#define BREAK_BOMB                  12

#define UNDEFINED_VALUE 1000000000

#define UIUserInterfaceIdiomPad		0
#define UIUserInterfaceIdiomPad1	1

extern cocos2d::CCString g_resPath;
extern int      g_nCurLevel;
extern int      g_nCurMission;
extern int      g_nAvailableLevel[5];
extern int      g_nBestScore[200/*LEVEL_COUNT*/];

extern int    g_nBodyIndex[100];

extern bool     g_bGameSuccess;
extern bool     g_bGameOver;
extern bool     g_bGamePause;
extern int      g_nLife;
extern int      g_nHeroWeaponType;
extern bool     g_bSubLife;
extern cocos2d::CCArray  *g_arrBombEffect;
//extern int      g_nSwitch;
extern int      g_nBulletInfo[200/*LEVEL_COUNT*/][6];
extern int      g_arrBulletsCount;
//extern int      g_nGameScore;
extern int      g_nGameCoins;
extern int      g_nHeroSkinIndex;

extern bool     g_bTutorial;
extern bool		g_SaveTutorial;
extern bool		g_TestTutorial;

extern int     g_nArrTutorialCount[10/*LEVEL_COUNT*/];
extern float    g_fArrTutorialInfo[10/*LEVEL_COUNT*/][8][6];
extern bool		g_CanKillEnemy;		

extern bool      g_nHeroUnlocked[5];
extern bool        g_nMissionsUnlocked[5];

extern int      g_nSolutionsNumber;
extern int      g_nSkipsNumber;
extern int		SCREEN_WIDTH;
extern int		SCREEN_HEIGHT;
extern int VIRT_WIDTH;
extern int VIRT_HEIGHT;
extern int		 g_bStoreFromGame;

int UI_USER_INTERFACE_IDIOM();
CCArray* componentsSeparatedByString( CCString* srcStr,const char*sepStr );

void animateFocusNode(CCNode* menuItem,float m_orgScale);
void animateFocusLoseNode(CCNode* menuItem,float m_orgScale);

bool isBarrel(LHSprite* spr);
bool isHeroFriend(LHSprite* spr);
float watchAngle(CCPoint ptStart, CCPoint ptEnd);
void controlScale(CCNode *node);

float iDevPixelX(float x);
float iDevPixelY(float y);
float iDevSize(float s);
float iDevChildPixelX(float x);
float iDevChildPixelY(float y);
float iDevChildSize(float s);

float RandomFloat(float lo, float hi);
#endif