//
//  GameSuccessLayer.m
//  hotair
//
//  Created by admin on 12/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "GameSuccessLayer.h"
#include "AppDelegate.h"
#include "Constants.h"
#include "ccframework/ResourceManager.h"
#include "gamelayer/GameLayer.h"
#include "StoreLayer/StoreLayer.h"
#include "MainMenu/TitleLayer.h"
#include "LevelSelect/LevelSelectLayer.h"
#include "LevelSelect/StageSelectLayer.h"

USING_NS_CC_EXT;

SHParticleFireWorks::SHParticleFireWorks()
{
	CCParticleSystemQuad::initWithTotalParticles(700);
	setAutoRemoveOnFinish(true);

	// duration
	setDuration(0.1f);

	setEmitterMode(kCCParticleModeGravity);

	// Gravity Mode: gravity
	setGravity(ccp(0,-100));

	// Gravity Mode: speed of particles
	setSpeed(70);
	setSpeedVar(40);

	// Gravity Mode: radial
	setRadialAccel(0);
	setRadialAccelVar(0);

	// Gravity Mode: tagential
	setTangentialAccel(0);
	setTangentialAccelVar(0);

	// angle
	setAngle(90);
	setAngleVar(360);

	// emitter position
	setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	setPosVar(CCPointZero);

	// life of particles
	setLife(5.0f);
	setLifeVar(2);

	// size, in pixels
	setStartSize(15.0f);
	setStartSizeVar(10.0f);
	setEndSize(kCCParticleStartSizeEqualToEndSize);

	// emits per second
	setEmissionRate(700/getDuration());

	// color of particles
	setStartColor(ccc4f(0.7f,0.1f,0.2f,1.0f));
	setStartColorVar(ccc4f(0.5f,0.5f,0.5f,0.0f));
	setEndColor(ccc4f(0.5f,0.5f,0.5f,0.0f));
	setEndColorVar(ccc4f(0.5f,0.5f,0.5f,0.0f));

	setTexture(CCTextureCache::sharedTextureCache()->addImage("stars2.png"));

	// additive
	setBlendAdditive(false);
}

bool GameSuccessLayer::init()
{
	if( ScaleLayer::init() )
	{
        m_app->playBGM(B_GAME_SUCCESS, false);

		setPosition( 0,SCREEN_HEIGHT );

		CCMoveTo *move1 = CCMoveTo::create(0.7f, ccp(0, 0/*- 120)*/));
		CCActionInterval* actEase1 = CCEaseElasticOut::create(move1,0.4f);
		runAction( actEase1 );

        //addBackEmitter();
		g_bTutorial = false;
		if ( g_TestTutorial )
		{
			g_bTutorial = true;
			actionGameSuccess();
		}
		return true;
    }

	return false;
}

GameSuccessLayer::~GameSuccessLayer()
{
	g_bGameSuccess = false;
	unscheduleAllSelectors();
	stopAllActions();
	removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void GameSuccessLayer::setInfo(GameLayer* parent,int score)
{
	parent->enableButtons(false);
	nGameScore = score;
	if ( nGameScore > g_nBestScore[g_nCurLevel] )
		g_nBestScore[g_nCurLevel] = nGameScore;

	createBackground();
	createInfo();
	createButtons();
}

void GameSuccessLayer::createBackground()
{
	CCLayerColor* backLayer = CCLayerColor::create(ccc4(0,0,0,0), 3000, 30000);
// 	backLayer->setAnchorPoint(ccp(0,0));
// 	backLayer->setPosition(ccp(1500, 1500));
	addChild(backLayer);
	CCFadeTo *action = CCFadeTo::create(0.3f, 100);
	backLayer->runAction( action );

	CCSprite *sprDialog;
	sprDialog = m_resMgr->getSpriteWithName("game/gameresult/levelcomplete_bg");
	sprDialog->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	addChild(sprDialog);
}

void GameSuccessLayer::createInfo()
{
	CCLabelBMFont *levelLabel = CCLabelBMFont::create("Level:","font/PopupScreenTitleFont@2x.fnt");
	levelLabel->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(240)));    
	levelLabel->setColor(ccc3(247,166,0));
	levelLabel->setScale( iDevSize(1) );
	addChild(levelLabel);

	CCSprite* levelSp = m_resMgr->getSpriteWithName("game/gameresult/label");
	levelSp->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(288)));
	addChild(levelSp);

	levelCount = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	levelCount->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(288)));
	levelCount->setColor(ccc3(255,255,255));
	levelCount->setString(CCString::createWithFormat("%d-%d", g_nCurMission+1,g_nCurLevel%PER_LEVEL_COUNT+1)->getCString());
	levelCount->setScale( iDevSize(1) );
	addChild(levelCount);

	CCLabelBMFont *scoreLabel = CCLabelBMFont::create("Score:","font/PopupScreenTitleFont@2x.fnt");
	scoreLabel->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(240)));    
	scoreLabel->setColor(ccc3(247,166,0));
	scoreLabel->setScale( iDevSize(1) );
	addChild(scoreLabel);

	CCSprite* scoreSp = m_resMgr->getSpriteWithName("game/gameresult/label");
	scoreSp->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(288)));
	addChild(scoreSp);

	scoreCount = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	scoreCount->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(288)));
	scoreCount->setColor(ccc3(255,255,255));
	scoreCount->setString(CCString::createWithFormat("%d", nGameScore)->getCString());
	scoreCount->setScale( iDevSize(1) );
	addChild(scoreCount);

	CCLabelBMFont *coinLabel = CCLabelBMFont::create("Coins:","font/PopupScreenTitleFont@2x.fnt");
	coinLabel->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(346)));    
	coinLabel->setColor(ccc3(247,166,0));
	coinLabel->setScale( iDevSize(1) );
	addChild(coinLabel);

	CCSprite* coinSp = m_resMgr->getSpriteWithName("game/gameresult/label");
	coinSp->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(394)));
	addChild(coinSp);

	coinCount = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	coinCount->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(394)));
	coinCount->setColor(ccc3(255,255,255));
	coinCount->setString(CCString::createWithFormat("%d", g_nGameCoins)->getCString());
	coinCount->setScale( iDevSize(1) );
	addChild(coinCount);

	CCLabelBMFont *bestScoreLabel = CCLabelBMFont::create("Best Score:","font/PopupScreenTitleFont@2x.fnt");
	bestScoreLabel->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(346)));    
	bestScoreLabel->setColor(ccc3(247,166,0));
	bestScoreLabel->setScale( iDevSize(1) );
	addChild(bestScoreLabel);

	CCSprite* bestScoreSp = m_resMgr->getSpriteWithName("game/gameresult/label");
	bestScoreSp->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(394)));
	addChild(bestScoreSp);

	bestScoreCount = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	bestScoreCount->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(394)));
	bestScoreCount->setColor(ccc3(255,255,255));
	bestScoreCount->setString(CCString::createWithFormat("%d", g_nBestScore[g_nCurLevel])->getCString());
	bestScoreCount->setScale( iDevSize(1) );
	addChild(bestScoreCount);
}

void GameSuccessLayer::createButtons()
{
	GrowButton *menuMainmenu = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnMainmenu1",
		"game/gameresult/btnMainmenu2",
		this,
		cccontrol_selector(GameSuccessLayer::onMainmenu));
	menuMainmenu->setPosition(ccp(iDevPixelX(666), SCREEN_HEIGHT - iDevPixelY(487)));    
	addChild(menuMainmenu);  

	GrowButton *menuShop = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnShop1",
		"game/gameresult/btnShop2",
		this,
		cccontrol_selector(GameSuccessLayer::onShop));
	menuShop->setPosition(ccp(iDevPixelX(390), SCREEN_HEIGHT - iDevPixelY(487)));    
	addChild(menuShop);

	GrowButton *menuRestart = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnRestart1",
		"game/gameresult/btnRestart2",
		this,
		cccontrol_selector(GameSuccessLayer::onRestart));
	menuRestart->setPosition(ccp(iDevPixelX(390), SCREEN_HEIGHT - iDevPixelY(576)));    
	addChild(menuRestart);

	GrowButton *menuNext = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnNext1",
		"game/gameresult/btnNext2",
		this,
		cccontrol_selector(GameSuccessLayer::onNext));
	menuNext->setPosition(ccp(iDevPixelX(666), SCREEN_HEIGHT - iDevPixelY(576)));    
	addChild(menuNext);
	setTouchEnabled(true);
}

void GameSuccessLayer::addBackEmitter()
{
	SHParticleFireWorks *m_pEmitter = new SHParticleFireWorks();
	m_pEmitter->retain();
	addChild(m_pEmitter, 10);
	m_pEmitter->setAutoRemoveOnFinish(true);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pEmitter->setPosition(ccp(rand()%(SCREEN_WIDTH/2), rand()%(SCREEN_HEIGHT/2)+SCREEN_HEIGHT/2));
}

void GameSuccessLayer::onMainmenu(CCObject *sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

	g_nLife = INIT_PLAYER_LIFE;
	CCScene *sce = StageSelectLayer::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f, sce));
}

void GameSuccessLayer::onShop(CCObject *sender, CCControlEvent event)
{  
	m_app->playEff(E_CLICK);
	g_bStoreFromGame = true;

	CCScene* sc = CCTransitionFade::create(0.7f,StoreLayer::scene());
	CCDirector::sharedDirector()->replaceScene(sc);
}

void GameSuccessLayer::onRestart(CCObject *sender, CCControlEvent event)
{
    m_app->playEff(E_CLICK);

    g_bGameSuccess = false;
	removeFromParentAndCleanup(true);
    m_app->changeWindow(WND_GAME);
}

void GameSuccessLayer::onNext(CCObject *sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

    actionGameSuccess();
}

void GameSuccessLayer::actionGameSuccess()
{
    g_nCurLevel ++;
    if(g_nAvailableLevel[g_nCurMission] < g_nCurLevel % PER_LEVEL_COUNT)
        g_nAvailableLevel[g_nCurMission] = g_nCurLevel % PER_LEVEL_COUNT;
    if(g_nAvailableLevel[g_nCurMission] > (LEVEL_COUNT - 1) % PER_LEVEL_COUNT)
        g_nAvailableLevel[g_nCurMission] = (LEVEL_COUNT - 1) % PER_LEVEL_COUNT;
    
    if(g_nCurLevel == (int)LEVEL_COUNT){        
		CCScene *scene = TitleLayer::scene();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f,scene));
        g_nLife = INIT_PLAYER_LIFE;   
        g_nCurLevel = 0;
		m_app->saveSetting();
    }else if ( g_nCurLevel % PER_LEVEL_COUNT == 0 )
    {
		g_nMissionsUnlocked[g_nCurMission+1] = true;
		if ( g_nAvailableLevel[g_nCurMission+1] < 0 )
			g_nAvailableLevel[g_nCurMission+1] = 0;
		
		CCScene *scene = LevelSelectLayer::scene();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f,scene));
    }
	else
    {
        m_app->saveSetting();
        m_app->changeWindow(WND_GAME);
    }    
}
