//
//  TitleLayer.m
//  hotair
//
//  Created by admin on 12/6/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "TitleLayer.h"
#include "AppDelegate.h"
#include "Constants.h"
#include "Options/OptionLayer.h"
#include "ccframework/ResourceManager.h"
#include "LevelSelect/LevelSelectLayer.h"
#include "StoreLayer/StoreLayer.h"
#include "ModalAlert/ModalAlert.h"

USING_NS_CC_EXT;

CCScene* TitleLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	TitleLayer *layer = TitleLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool TitleLayer::init()
{
	if (ScaleLayer::init())
	{
        m_app->playBGM(B_TITLE);
        createBackground();
        createButtons();
    }
    return true;
}

TitleLayer::~TitleLayer()
{
	stopAllActions();
	unscheduleAllSelectors();
	removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures(); 
}

void TitleLayer::createBackground()
{
	CCSprite *sprite = m_resMgr->getSpriteWithName("common_bg1");
    sprite->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    addChild(sprite, -100);

	sprite = m_resMgr->getSpriteWithName("mainmenu/flag_bar");
	sprite->setPosition(ccp(iDevPixelX(149), iDevPixelY(384)));
	addChild(sprite, -100);

	sprite = m_resMgr->getSpriteWithName("mainmenu/title_hero");
	sprite->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	addChild(sprite, -100);

	sprite = m_resMgr->getSpriteWithName("common_sun");
	sprite->setPosition( ccp(iDevPixelX(747), iDevPixelY(700)) );
	addChild(sprite, -100);
	CCRepeatForever *repAct = CCRepeatForever::create( CCRotateBy::create(0.1f, 2.0f) );
	sprite->runAction(repAct);

	sprite = m_resMgr->getSpriteWithName("common_bg2");
	sprite->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	addChild(sprite, -100);

	sprite = m_resMgr->getSpriteWithName("common_bg3");
	sprite->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	addChild(sprite, -100);
}

void TitleLayer::createButtons()
{
	m_menuItem[0] = GrowButton::buttonWithSpriteFileName("mainmenu/btnPlay1","mainmenu/btnPlay2",this,cccontrol_selector(TitleLayer::onPlay));
    addChild(m_menuItem[0]);
	m_menuItem[0]->setPosition(iDevPixelX(213), SCREEN_HEIGHT-iDevPixelY(163));
    
	m_menuItem[1] = GrowButton::buttonWithSpriteFileName("mainmenu/btnOptions1","mainmenu/btnOptions2",this,cccontrol_selector(TitleLayer::onOptions));
    addChild(m_menuItem[1]);
	m_menuItem[1]->setPosition(iDevPixelX(143), SCREEN_HEIGHT-iDevPixelY(259));
    
	m_menuItem[2] = GrowButton::buttonWithSpriteFileName("mainmenu/btnAbout1","mainmenu/btnAbout2",this,cccontrol_selector(TitleLayer::onHighscore));
    addChild(m_menuItem[2]);
	m_menuItem[2]->setPosition(iDevPixelX(175), SCREEN_HEIGHT-iDevPixelY(355));
    
	m_menuItem[3] = GrowButton::buttonWithSpriteFileName("mainmenu/btnShop1","mainmenu/btnShop2",this,cccontrol_selector(TitleLayer::onShop));
    addChild(m_menuItem[3]);
	m_menuItem[3]->setPosition(iDevPixelX(171), SCREEN_HEIGHT-iDevPixelY(456));

	m_menuItem[4] = GrowButton::buttonWithSpriteFileName("mainmenu/btnMore1","mainmenu/btnMore2",this,cccontrol_selector(TitleLayer::onMoreGame));
	m_menuItem[4]->setPosition( ccp(iDevPixelX(183), SCREEN_HEIGHT-iDevPixelY(571)) );
	addChild(m_menuItem[4]);

	for ( int i = 0; i < 5;i++ )
		m_menuItem[i]->setScale(0);
		
	m_menuItem[5] = GrowButton::buttonWithSpriteFileName("mainmenu/btnClose1","mainmenu/btnClose2",this,cccontrol_selector(TitleLayer::onClose));
	m_menuItem[5]->setPosition( ccp( iDevPixelX(896), SCREEN_HEIGHT-iDevPixelY(672)) );
	addChild(m_menuItem[5]);

	titleSp = m_resMgr->getSpriteWithName("mainmenu/title");
	titleSp->setPosition( ccp(SCREEN_WIDTH * 0.65f,SCREEN_HEIGHT * 1.3f ));
	addChild(titleSp);

	schedule( schedule_selector(TitleLayer::onInitAction),0.6f );

}

void TitleLayer::onInitAction(float dt)
{
	unschedule( schedule_selector(TitleLayer::onInitAction) );
	for ( int i = 0; i < 5; i++ )
	{
		m_menuItem[i]->runAction(CCSequence::create( CCDelayTime::create(i*0.1f), CCEaseBackOut::create(CCScaleTo::create(0.3f, 1.0f, 1.0f)),NULL));
	}

	CCMoveTo *move1 = CCMoveTo::create(0.6f,ccp(SCREEN_WIDTH * 0.65,SCREEN_HEIGHT * 0.75 ));
	CCActionInterval* actEase1 = CCEaseBounceOut::create(move1/*,0.4f*/);
	titleSp->runAction( CCSequence::create( CCDelayTime::create(0.5), actEase1,NULL ));
}

void TitleLayer::onClose( CCObject* sender, CCControlEvent event )
{
	m_app->playEff(E_CLICK);

	CCDirector::sharedDirector()->end();
	//ModalAlert::Confirm("Do you want to quit?", this, menu_selector(TitleLayer::onQuitOK),NULL);
}

void TitleLayer::onQuitOK( CCObject* sender )
{
	CCDirector::sharedDirector()->end();
}

void TitleLayer::onMoreGame( CCObject* sender, CCControlEvent event )
{
	m_app->playEff(E_CLICK);
}

void TitleLayer::onPlay( CCObject* sender, CCControlEvent event )
{
    m_app->playEff(E_CLICK);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f,LevelSelectLayer::scene()));
}

void TitleLayer::onOptions( CCObject* sender, CCControlEvent event )
{
    m_app->playEff(E_CLICK);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f,OptionLayer::scene()));
}

void TitleLayer::onHighscore( CCObject* sender, CCControlEvent event )
{
    m_app->playEff(E_CLICK);
    m_app->showLeaderboard();
}

void TitleLayer::onShop( CCObject* sender, CCControlEvent event )
{
    m_app->playEff(E_CLICK);
	g_bStoreFromGame = false;
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f,StoreLayer::scene()));
}
