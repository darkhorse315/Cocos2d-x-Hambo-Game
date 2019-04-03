#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/

using namespace CocosDenshion;

typedef enum _WND_ID
{
	WND_NONE = 0,
	WND_TITLE,
	WND_GAME,

	WND_COUNT
} WND_ID;

typedef enum _EFFECT_ID
{
	E_CLICK = 0,
	E_SHOOT_KNIFE,
	E_SHOOT_PISTOL,
	E_SHOOT_UZI,
	E_HAMBO_DIE,
	E_ENEMY_DIE,
	E_ICE_BREAK,
	E_REPLACE_WEAPON,
	E_BULLET_COLLISION,
	E_BOMB
} EFFECT_ID;

typedef enum _BGM_ID
{
	B_NONE = -1,
	B_TITLE = 0,
	B_GAME,
	B_GAME_SUCCESS,
	B_GAME_OVER,
} BGM_ID;

class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();


	void playEff(EFFECT_ID eID);
	void playBGM(BGM_ID bIDm, bool bLoop = true);
	void stopBGM();

	void loadSetting();
	void saveSetting();

	void changeWindow(WND_ID wndNew);
	void changeWindow(WND_ID wndNew,int param);

	void submitScore(int nScore);
	void showLeaderboard();

	bool getBGMEnable()		{	return m_bBGM;	};
	void setBGMEnable(bool bEnable);

	bool getEffEnable()		{	return m_bEffect;	};
	void setEffEnable(bool bEnable);

private:
	void initApp();
	void soundInit();

	bool m_bBGM;
	bool m_bEffect;

	SimpleAudioEngine *sae;

	BGM_ID	m_BGMid;
	WND_ID	m_nCurWnd;
};

#endif // _APP_DELEGATE_H_

