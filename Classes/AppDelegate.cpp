
#include "AppDelegate.h"
#include "Constants.h"
#include "logo/logo.h"
#include "GameLayer/LoadScene.h"
#include "Options/OptionLayer.h"
#include "GameLayer/GameLayer.h"
#include "MainMenu/TitleLayer.h"
#include "StoreLayer/StoreLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
	
}

AppDelegate::~AppDelegate() 
{
	saveSetting();
	sae->stopBackgroundMusic();
	sae->stopAllEffects();
	//SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();

    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	initApp();

// 	// create a scene. it's an autorelease object
// 	//CCScene *pScene = logo::scene();
// 	CCScene *pScene = StoreLayer::scene();
// 	pDirector->runWithScene(pScene);

	changeWindow(WND_TITLE);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AppDelegate::initApp()
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Process screen resolution
	//
	//////////////////////////////////////////////////////////////////////////
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize designSize = CCSizeMake(VIRT_WIDTH, VIRT_HEIGHT);
// 	if (screenSize.height > 320)
// 	{
// 		CCSize resourceSize = CCSizeMake(960, 640);
// 		CCFileUtils::sharedFileUtils()->setResourceDirectory("hd");
// 		pDirector->setContentScaleFactor(resourceSize.height/designSize.height);
// 	}
//	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionNoBorder);
//	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionExactFit); 

	SCREEN_WIDTH = CCDirector::sharedDirector()->getWinSize().width;
	SCREEN_HEIGHT = CCDirector::sharedDirector()->getWinSize().height;

	//////////////////////////////////////////////////////////////////////////
	//
	// Set resource directory
	//
	//////////////////////////////////////////////////////////////////////////
	TargetPlatform target = getTargetPlatform(); 
    if (target == kTargetIpad)
    {
        // ipad
		g_resPath = CCString("ipad_res/");
        CCFileUtils::sharedFileUtils()->setResourceDirectory("");
        
        // don't enable retina because we don't have ipad hd resource
        //CCEGLView::sharedOpenGLView()->setDesignResolutionSize(960, 640, kResolutionNoBorder);
    }
    else if (target == kTargetIphone)
    {
        // iphone
        g_resPath = CCString("iphone_res");
        // try to enable retina on device
//		if (true == CCDirector::sharedDirector()->enableRetinaDisplay(true))
//		{
//			// iphone hd
//			CCFileUtils::sharedFileUtils()->setResourceDirectory("");
//		}
//		else 
        {
            CCFileUtils::sharedFileUtils()->setResourceDirectory("");
        }
    }
	else if (target == kTargetAndroid)
	{
		// android
		g_resPath = CCString("ipad_res/");
		CCFileUtils::sharedFileUtils()->setResourceDirectory("");
	}
    else 
    {
        // windows, blackberry, linux or mac
        // use 960*640 resources as design resolution size
		g_resPath = CCString("ipad_res/");
        CCFileUtils::sharedFileUtils()->setResourceDirectory("Resources");
        //CCEGLView::sharedOpenGLView()->setDesignResolutionSize(960, 640, kResolutionNoBorder);
    }

	//////////////////////////////////////////////////////////////////////////
	//
	// Initialize application
	//
	//////////////////////////////////////////////////////////////////////////
	soundInit();

	for ( int i = 0; i < 100; i++ )
		g_nBodyIndex[i] = i;
	
	loadSetting();

	m_nCurWnd = WND_NONE;
	srand( ( unsigned )time( NULL ) );
	g_arrBombEffect = new CCArray();
}

void AppDelegate::soundInit()
{
	sae = SimpleAudioEngine::sharedEngine();

	CCFileUtils*	fileUtils = CCFileUtils::sharedFileUtils();

	sae->preloadBackgroundMusic(fileUtils->fullPathFromRelativePath("sounds/MainMenuMusic.mp3"));
	sae->preloadBackgroundMusic(fileUtils->fullPathFromRelativePath("sounds/game_music.mp3"));
	sae->preloadBackgroundMusic(fileUtils->fullPathFromRelativePath("sounds/Hambo_Win.mp3"));
	sae->preloadBackgroundMusic(fileUtils->fullPathFromRelativePath("sounds/Hambo_Loose.mp3"));

	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/btn_click.mp3"));
	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/Snd_BulletHighCtc.mp3"));
	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/Snd_Explosion_3.mp3"));
	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/Snd_KnifeRotate.mp3"));
	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/Snd_Pistol.mp3"));
	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/Snd_Uzi.mp3"));
	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/Hambo_damaged.mp3"));
	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/enemy_die.mp3"));
	sae->preloadEffect(fileUtils->fullPathFromRelativePath("sounds/GlassShatter.mp3"));

	sae->stopBackgroundMusic();

	sae->setBackgroundMusicVolume(0.5f);
	sae->setEffectsVolume(1.0f);

	m_BGMid = B_NONE;
}

void AppDelegate::playBGM(BGM_ID bID, bool bLoop)
{
	if (!m_bBGM)
		return;

	if ( m_BGMid == bID )
		return;

	stopBGM();

	CCString*	szPath;
	switch (bID) {
		case B_TITLE:			szPath = CCString::create("sounds/MainMenuMusic.mp3");	break;
		case B_GAME:			szPath = CCString::create("sounds/game_music.mp3");		break;
		case B_GAME_SUCCESS:	szPath = CCString::create("sounds/Hambo_Win.mp3");		break;
		case B_GAME_OVER:		szPath = CCString::create("sounds/Hambo_Loose.mp3");	break;
	}

	m_BGMid = bID;

	CCFileUtils*	fileUtils = CCFileUtils::sharedFileUtils();
	CCString*		strNewBGM = CCString::create( fileUtils->fullPathFromRelativePath(szPath->getCString()) );
	sae->playBackgroundMusic(strNewBGM->getCString(), bLoop);
}

void AppDelegate::stopBGM()
{
	if (sae->isBackgroundMusicPlaying())
	{
		sae->stopBackgroundMusic();
		//sae->unloadEffect(m_strPlayingBGM->getCString());
	}
	m_BGMid = B_NONE;
}

void AppDelegate::playEff(EFFECT_ID eID)
{
	if(!m_bEffect)
		return;

	CCString*	szPath;
	//sae->unloadEffect(m_strPlayingEffect->getCString());
	switch (eID) {
		case E_CLICK:				szPath = CCString::create("sounds/btn_click.mp3");			break;
		case E_BULLET_COLLISION:	szPath = CCString::create("sounds/Snd_BulletHighCtc.mp3");	break;
		case E_BOMB:				szPath = CCString::create("sounds/Snd_Explosion_3.mp3");	break;
		case E_REPLACE_WEAPON:		szPath = CCString::create("sounds/Snd_Explosion_3.mp3");	break;
		case E_SHOOT_KNIFE:			szPath = CCString::create("sounds/Snd_KnifeRotate.mp3");	break;
		case E_SHOOT_PISTOL:		szPath = CCString::create("sounds/Snd_Pistol.mp3");			break;
		case E_SHOOT_UZI:			szPath = CCString::create("sounds/Snd_Uzi.mp3");			break;
		case E_HAMBO_DIE:			szPath = CCString::create("sounds/Hambo_damaged.mp3");		break;
		case E_ENEMY_DIE:			szPath = CCString::create("sounds/enemy_die.mp3");			break;
		case E_ICE_BREAK:			szPath = CCString::create("sounds/GlassShatter.mp3");		break;
	}
	CCFileUtils*	fileUtils = CCFileUtils::sharedFileUtils();
	CCString*		strNewEff = CCString::create( fileUtils->fullPathFromRelativePath(szPath->getCString()) );
	sae->playEffect(strNewEff->getCString());
}

void AppDelegate::loadSetting()
{
	CCUserDefault*	setting = CCUserDefault::sharedUserDefault();
	m_bBGM					= setting->getBoolForKey("BGM",						true);
	m_bEffect				= setting->getBoolForKey("Effect",					true);
	g_nCurLevel				= setting->getIntegerForKey("Success_Level",		0);
	for ( int i = 0; i < LEVEL_COUNT; i++ )
	{
		CCString *str = CCString::createWithFormat("BestScoreOf%d",i);
		g_nBestScore[i]		= setting->getIntegerForKey(str->getCString(),		0);
	}
	//g_nGameScore			= setting->getIntegerForKey("Game_Score",			0);
	g_nGameCoins			= setting->getIntegerForKey("Game_Coins",			0);

	g_nAvailableLevel[0]		= setting->getIntegerForKey("Available_Level0",		0);
	for ( int i = 1; i < STAGE_COUNT; i++ )
	{
		CCString *str = CCString::createWithFormat("Available_Level%d",i);
		g_nAvailableLevel[i]		= setting->getIntegerForKey(str->getCString(),		-1);
	}
	
	g_nSkipsNumber			= setting->getIntegerForKey("Skip_Count",			3);
	g_nSolutionsNumber		= setting->getIntegerForKey("Solutions_Count",		3);
	g_nHeroSkinIndex		= setting->getIntegerForKey("Hero_Skin_Index",		0);
	g_nHeroUnlocked[0]		= setting->getBoolForKey("Hero_Unlocked_Index_0",	true);
	g_nHeroUnlocked[1]		= setting->getBoolForKey("Hero_Unlocked_Index_1",	false);
	g_nHeroUnlocked[2]		= setting->getBoolForKey("Hero_Unlocked_Index_2",	false);
	g_nHeroUnlocked[3]		= setting->getBoolForKey("Hero_Unlocked_Index_3",	false);
	g_nHeroUnlocked[4]		= setting->getBoolForKey("Hero_Unlocked_Index_4",	false);
	g_nMissionsUnlocked[0]	= setting->getBoolForKey("Mission_Unlocked_Index_0",true);
	g_nMissionsUnlocked[1]	= setting->getBoolForKey("Mission_Unlocked_Index_1",false);
	g_nMissionsUnlocked[2]	= setting->getBoolForKey("Mission_Unlocked_Index_2",false);
	g_nMissionsUnlocked[3]	= setting->getBoolForKey("Mission_Unlocked_Index_3",false);
	g_nMissionsUnlocked[4]	= setting->getBoolForKey("Mission_Unlocked_Index_4",false);

#ifdef DEBUG_ALL_LEVEL

	g_nGameCoins = 90000;
	g_nAvailableLevel[0] = PER_LEVEL_COUNT - 1;
	g_nAvailableLevel[1] = PER_LEVEL_COUNT - 1;
	g_nAvailableLevel[2] = PER_LEVEL_COUNT - 1;
	g_nAvailableLevel[3] = PER_LEVEL_COUNT - 1;
	g_nAvailableLevel[4] = PER_LEVEL_COUNT - 1;
	g_nMissionsUnlocked[0] = true;
	g_nMissionsUnlocked[1] = true;
	g_nMissionsUnlocked[2] = true;
	g_nMissionsUnlocked[3] = true;
	g_nMissionsUnlocked[4] = true;
#endif
}

void AppDelegate::saveSetting()
{
	CCUserDefault*	setting = CCUserDefault::sharedUserDefault();
	setting->setBoolForKey("BGM",						m_bBGM);
	setting->setBoolForKey("Effect",					m_bEffect);
	setting->setIntegerForKey("Success_Level",			g_nCurLevel);
	for ( int i = 0; i < LEVEL_COUNT; i++ )
	{
		CCString *str = CCString::createWithFormat("BestScoreOf%d",i);
		setting->setIntegerForKey(str->getCString(),g_nBestScore[i]);
	}
	//setting->setIntegerForKey("Game_Score",				g_nGameScore);
	setting->setIntegerForKey("Game_Coins",				g_nGameCoins);
	for ( int i = 0; i < STAGE_COUNT; i++ )
	{
		CCString *str = CCString::createWithFormat("Available_Level%d",i);
		setting->setIntegerForKey(str->getCString(),		g_nAvailableLevel[i]);
	}
	setting->setIntegerForKey("Skip_Count",				g_nSkipsNumber);
	setting->setIntegerForKey("Solutions_Count",		g_nSolutionsNumber);
	setting->setIntegerForKey("Hero_Skin_Index",		g_nHeroSkinIndex);
	setting->setBoolForKey("Hero_Unlocked_Index_0",		g_nHeroUnlocked[0]);
	setting->setBoolForKey("Hero_Unlocked_Index_1",		g_nHeroUnlocked[1]);
	setting->setBoolForKey("Hero_Unlocked_Index_2",		g_nHeroUnlocked[2]);
	setting->setBoolForKey("Hero_Unlocked_Index_3",		g_nHeroUnlocked[3]);
	setting->setBoolForKey("Hero_Unlocked_Index_4",		g_nHeroUnlocked[4]);
	setting->setBoolForKey("Mission_Unlocked_Index_0",	g_nMissionsUnlocked[0]);
	setting->setBoolForKey("Mission_Unlocked_Index_1",	g_nMissionsUnlocked[1]);
	setting->setBoolForKey("Mission_Unlocked_Index_2",	g_nMissionsUnlocked[2]);
	setting->setBoolForKey("Mission_Unlocked_Index_3",	g_nMissionsUnlocked[3]);
	setting->setBoolForKey("Mission_Unlocked_Index_4",	g_nMissionsUnlocked[4]);
	setting->flush();
}

void AppDelegate::setBGMEnable(bool bEnable)
{
	if (m_bBGM == bEnable)
		return;

	m_bBGM = bEnable;

	if (bEnable)
		playBGM(B_TITLE);
	else
		stopBGM();
}

void AppDelegate::setEffEnable(bool bEnable)
{
	m_bEffect = bEnable;
}

void AppDelegate::submitScore(int nScore)
{

}

void AppDelegate::showLeaderboard()
{
// 	GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
// 	if (leaderboardController != NULL) {
// 		leaderboardController.category = self.currentLeaderBoard;
// 		leaderboardController.timeScope = GKLeaderboardTimeScopeAllTime;
// 		leaderboardController.leaderboardDelegate = self; 
// 		[viewController presentModalViewController: leaderboardController animated: YES];
// 		[leaderboardController release];
// 	}
}

void AppDelegate::changeWindow(WND_ID wndNew)
{
	changeWindow(wndNew, -1);
}

void AppDelegate::changeWindow(WND_ID wndNew, int param)
{
	CCDirector*	director = CCDirector::sharedDirector();
	CCScene*	scene = NULL;

	scene = LoadScene::scene(m_nCurWnd, wndNew, param);
	if( scene ) {
		if (m_nCurWnd == WND_NONE) {
			director->runWithScene(scene);
		}
		else {
			stopBGM();
			director->replaceScene(CCTransitionFade::create(0.5f, scene));
		}
		m_nCurWnd = wndNew;
	}
}