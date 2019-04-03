//
//  PauseLayer.m
//  BalloonBonanza_iphone
//
//  Created by admin on 1/4/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "PauseLayer.h"
#include "AppDelegate.h"
#include "Constants.h"
#include "ModalAlert/ModalAlert.h"
#include "ccframework/GrowButton.h"
#include "ccframework/ResourceManager.h"
#include "LevelSelect/StageSelectLayer.h"
#include "gamelayer/GameLayer.h"
#include "StoreLayer/StoreLayer.h"
#include "LevelSelect/LevelSelectLayer.h"

USING_NS_CC_EXT;

bool PauseLayer::init()
{
	if( ScaleLayer::init() )
	{
        g_bGamePause = true;


		setPosition(0,SCREEN_HEIGHT);

		CCMoveTo *move1 = CCMoveTo::create(0.3f,ccp(0,0));
		CCActionInterval* actEase1 = CCEaseElasticOut::create(move1,0.4f);
		runAction( actEase1 );

		return true;
    }
	return false;
}

PauseLayer::~PauseLayer()
{
	removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void PauseLayer::setInfo(GameLayer* p,int nscore)
{
	p->enableButtons(false);
	nGameScore = nscore;
	createBackground();
	createInfo();
	createButtons();
}

void PauseLayer::createBackground()
{
	backLayer = CCLayerColor::create(ccc4(0,0,0,0),3000,30000);
// 	backLayer->setAnchorPoint(ccp(0,0));
// 	backLayer->setPosition(ccp(1500, 1500));
	addChild(backLayer);
	CCFadeTo *action = CCFadeTo::create(0.3f, 100);
	backLayer->runAction( action );

	CCSprite *sprDialog;
	sprDialog = m_resMgr->getSpriteWithName("game/gameresult/gamepause_bg");
	sprDialog->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ));
	addChild(sprDialog);
}

void PauseLayer::createInfo()
{
	CCLabelBMFont *levelLabel = CCLabelBMFont::create("Level:","font/PopupScreenTitleFont@2x.fnt");
	levelLabel->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(216)));    
	levelLabel->setColor(ccc3(247,166,0));
	levelLabel->setScale( iDevSize(1) );
	addChild(levelLabel);

	CCSprite* levelSp = m_resMgr->getSpriteWithName("game/gameresult/label");
	levelSp->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(264)));
	addChild(levelSp);

	levelCount = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	levelCount->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(264)));
	levelCount->setColor(ccc3(255,255,255));
	levelCount->setString(CCString::createWithFormat("%d-%d", g_nCurMission+1, g_nCurLevel%PER_LEVEL_COUNT+1)->getCString());
	levelCount->setScale( iDevSize(1) );
	addChild(levelCount);

	CCLabelBMFont *scoreLabel = CCLabelBMFont::create("Score:","font/PopupScreenTitleFont@2x.fnt");
	scoreLabel->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(216)));    
	scoreLabel->setColor(ccc3(247,166,0));
	scoreLabel->setScale( iDevSize(1) );
	addChild(scoreLabel);

	CCSprite* scoreSp = m_resMgr->getSpriteWithName("game/gameresult/label");
	scoreSp->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(264)));
	addChild(scoreSp);

	scoreCount = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	scoreCount->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(264)));
	scoreCount->setColor(ccc3(255,255,255));
	scoreCount->setString(CCString::createWithFormat("%d", nGameScore)->getCString());
	scoreCount->setScale( iDevSize(1) );
	addChild(scoreCount);

	CCLabelBMFont *coinLabel = CCLabelBMFont::create("Coins:","font/PopupScreenTitleFont@2x.fnt");
	coinLabel->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(321)));    
	coinLabel->setColor(ccc3(247,166,0));
	coinLabel->setScale( iDevSize(1) );
	addChild(coinLabel);

	CCSprite* coinSp = m_resMgr->getSpriteWithName("game/gameresult/label");
	coinSp->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(370)));
	addChild(coinSp);

	coinCount = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	coinCount->setPosition(ccp(iDevPixelX(358), SCREEN_HEIGHT - iDevPixelY(370)));
	coinCount->setColor(ccc3(255,255,255));
	coinCount->setString(CCString::createWithFormat("%d", g_nGameCoins)->getCString());
	coinCount->setScale( iDevSize(1) );
	addChild(coinCount);

	CCLabelBMFont *bestScoreLabel = CCLabelBMFont::create("Best Score:","font/PopupScreenTitleFont@2x.fnt");
	bestScoreLabel->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(321)));    
	bestScoreLabel->setColor(ccc3(247,166,0));
	bestScoreLabel->setScale( iDevSize(1) );
	addChild(bestScoreLabel);

	CCSprite* bestScoreSp = m_resMgr->getSpriteWithName("game/gameresult/label");
	bestScoreSp->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(370)));
	addChild(bestScoreSp);

	bestScoreCount = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	bestScoreCount->setPosition(ccp(iDevPixelX(683), SCREEN_HEIGHT - iDevPixelY(370)));
	bestScoreCount->setColor(ccc3(255,255,255));
	bestScoreCount->setString(CCString::createWithFormat("%d", g_nBestScore[g_nCurLevel])->getCString());
	bestScoreCount->setScale( iDevSize(1) );
	addChild(bestScoreCount);
}

void PauseLayer::createButtons()
{
	GrowButton *menuResume = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnResume1",
		"game/gameresult/btnResume2",
		this,
		cccontrol_selector(PauseLayer::onResume));
	menuResume->setPosition(ccp(iDevPixelX(527), SCREEN_HEIGHT - iDevPixelY(550)));    
	addChild(menuResume);

	GrowButton *menuMainmenu = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnMainmenu1",
		"game/gameresult/btnMainmenu2",
		this,
		cccontrol_selector(PauseLayer::onMainmenu));
	menuMainmenu->setPosition(ccp(iDevPixelX(527), SCREEN_HEIGHT - iDevPixelY(451)));    
	addChild(menuMainmenu);  

	GrowButton *menuShop = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnShop1",
		"game/gameresult/btnShop2",
		this,
		cccontrol_selector(PauseLayer::onShop));
	menuShop->setPosition(ccp(iDevPixelX(346), SCREEN_HEIGHT - iDevPixelY(451)));    
	addChild(menuShop);

	GrowButton *menuSkip = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnSkip1",
		"game/gameresult/btnSkip2",
		this,
		cccontrol_selector(PauseLayer::onSkip));
	menuSkip->setPosition(ccp(iDevPixelX(715), SCREEN_HEIGHT - iDevPixelY(550)));    
	addChild(menuSkip);

	if ( g_nSkipsNumber <= 0 )
	{
		menuSkip->setEnabled(false);
		menuSkip->setOpacity(200);
	}
	else
	{
		CCString *skipCount = CCString::createWithFormat("%d",g_nSkipsNumber);
		CCLabelBMFont *skipCountLabel = CCLabelBMFont::create(skipCount->getCString(),"font/MainMenuFont@2x.fnt");
		skipCountLabel->setPosition( ccp(menuSkip->boundingBox().size.width*0.9f,menuSkip->boundingBox().size.height*0.5f) );    
		skipCountLabel->setColor(ccc3(247,166,0));
		skipCountLabel->setScale( iDevSize(1) );
		menuSkip->addChild(skipCountLabel);
	}

	GrowButton *menuSolution = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnSolution1",
		"game/gameresult/btnSolution2",
		this,
		cccontrol_selector(PauseLayer::onTutorial));
	menuSolution->setPosition(ccp(iDevPixelX(715), SCREEN_HEIGHT - iDevPixelY(451)));    
	addChild(menuSolution);  
	
	GrowButton *menuRestart = GrowButton::buttonWithSpriteFileName(
		"game/gameresult/btnRestart1",
		"game/gameresult/btnRestart2",
		this,
		cccontrol_selector(PauseLayer::onRestart));
	menuRestart->setPosition(ccp(iDevPixelX(346), SCREEN_HEIGHT - iDevPixelY(550)));    
	addChild(menuRestart);
}

void PauseLayer::onMainmenu(CCObject *sender, CCControlEvent event) {
    m_app->playEff(E_CLICK);
    g_bGamePause = false;
    g_nLife = INIT_PLAYER_LIFE;
	CCScene *sce = StageSelectLayer::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f,sce));
}

void PauseLayer::onResume(CCObject *sender, CCControlEvent event) {
    m_app->playEff(E_CLICK);
    g_bGamePause = false;
	((GameLayer*)getParent())->enableButtons(true);

	CCMoveTo *move1 = CCMoveTo::create(0.3f,ccp(0,-SCREEN_HEIGHT ));
	CCActionInterval* actEase1 = CCEaseElasticIn::create(move1, 0.5f);
	runAction( actEase1 );

	CCFadeTo *action = CCFadeTo::create(0.3f, 0);
	backLayer->runAction( action );

	schedule(schedule_selector(PauseLayer::close), 0.6f);
}

void PauseLayer::close(float dt)
{
	static float fdt = 0;
	fdt += dt;
	if ( fdt > 0.4 )
	{
		fdt = 0;
		unscheduleAllSelectors();
		removeFromParentAndCleanup(true);
	}
}

void PauseLayer::onRestart(CCObject *sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

	g_bGamePause = false;
	g_bTutorial = false;
	m_app->changeWindow(WND_GAME);
}

void PauseLayer::onTutorial(CCObject *sender, CCControlEvent event)
{  
	m_app->playEff(E_CLICK);
    if(g_nSolutionsNumber == 0){
        onSelectLess();
        return;
    }
    g_nSolutionsNumber --;
    g_bGamePause = false;
    g_bTutorial = true;

	m_app->changeWindow(WND_GAME);
}

void PauseLayer::onShop(CCObject *sender, CCControlEvent event)
{  
	m_app->playEff(E_CLICK);

	g_bStoreFromGame = true;
	CCScene* sc = CCTransitionFade::create(0.7f,StoreLayer::scene());
	CCDirector::sharedDirector()->replaceScene(sc);
}

void PauseLayer::onSkip(CCObject *sender, CCControlEvent event)
{
    if(LEVEL_COUNT == g_nCurLevel + 1)
        return;

    if(g_nSkipsNumber == 0){
        onSelectLess();
        return;
    }

	m_app->playEff(E_CLICK);
	g_bTutorial = false;
    g_nSkipsNumber --;
    g_nCurLevel ++;    
	g_nAvailableLevel[g_nCurMission]++;
        
	if ( g_nCurLevel % PER_LEVEL_COUNT == 0 )
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

void PauseLayer::onSelectLess()
{
	ModalAlert::Tell("You do not have enough coins.",this,NULL);
}
