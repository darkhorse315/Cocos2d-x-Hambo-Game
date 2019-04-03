//
//  MissionsLayer.m
//  Hambo
//
//  Created by admin on 7/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MissionsLayer.h"
#include "ccframework/ResourceManager.h"
#include "Constants.h"
#include "ccframework/GrowButton.h"
#include "StoreLayer.h"
#include "AppDelegate.h"
#include "ModalAlert/ModalAlert.h"

USING_NS_CC_EXT;

int nMissionsPrice[5] = {0, 8000, 8000, 8000, 15000};

MissionsLayer::MissionsLayer() 
{
	if ( ScaleLayer::init() )
	{
		m_nSelect = 0;

		createBackgrounds();
        createButtons();
		schedule(schedule_selector(MissionsLayer::onTime));
    }
}

MissionsLayer::~MissionsLayer()
{
	unscheduleAllSelectors();
	removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void MissionsLayer::createBackgrounds()
{
// 	CCSprite *sprBG = m_resMgr->getSpriteWithName("store/costumes_back");
// 	addChild(sprBG);
// 	sprBG->setPosition(ccp(640, 553));

	m_sprHero = m_resMgr->getSpriteWithName("store/store_levelmark1");
	addChild(m_sprHero);
	m_sprHero->setPosition(ccp(iDevPixelX(630), iDevPixelY(455)));
	m_sprHero->setScale( 0.5 );

	CCSprite *sprCoins = m_resMgr->getSpriteWithName("store/money");
	addChild(sprCoins);
	sprCoins->setPosition(ccp(iDevPixelX(508), iDevPixelY(579)));

	m_lblCoins = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	m_lblCoins->setPosition(ccp(iDevPixelX(562), iDevPixelY(579)));
	m_lblCoins->setAlignment(kCCTextAlignmentLeft);
	m_lblCoins->setAnchorPoint( ccp(0.0f, 0.5f) );
	m_lblCoins->setScale( iDevSize(1.3f) );
	addChild(m_lblCoins);

	CCString*	string = CCString::createWithFormat("%d", nMissionsPrice[m_nSelect]);
	m_lblCoins->setString(string->getCString());
}

void MissionsLayer::createButtons()
{
	GrowButton *btnPrev = GrowButton::buttonWithSpriteFileName("store/btnPrev1", "store/btnPrev2", this, cccontrol_selector(MissionsLayer::onPrev));
	btnPrev->setPosition(ccp(iDevPixelX(454), iDevPixelY(460)));
	addChild(btnPrev);

	GrowButton *btnNext = GrowButton::buttonWithSpriteFileName("store/store_btnNext1", "store/store_btnNext2", this, cccontrol_selector(MissionsLayer::onNext));
	btnNext->setPosition(ccp(iDevPixelX(810), iDevPixelY(460)));
	addChild(btnNext);

	m_btnUnlock = GrowButton::buttonWithSpriteFileName("store/btnUnlock1", "store/btnUnlock2", this, cccontrol_selector(MissionsLayer::onUnlock));
	m_btnUnlock->setPosition(ccp(iDevPixelX(637), iDevPixelY(280)));
	addChild(m_btnUnlock);
}

void MissionsLayer::onTime(float dt)
{
    if(g_nMissionsUnlocked[m_nSelect])
	{
        m_btnUnlock->setVisible(false);
    }
	else
	{
        m_btnUnlock->setVisible(true);    
    }
    
}

void MissionsLayer::onPrev(CCObject*sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

    m_nSelect --;
    if(m_nSelect < 0)
        m_nSelect = STAGE_COUNT-1;
    changeMissions();
}

void MissionsLayer::onNext(CCObject*sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

    m_nSelect ++;
    if(m_nSelect > STAGE_COUNT-1)
        m_nSelect = 0;
    changeMissions();
}

void MissionsLayer::onUnlock(CCObject*sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

    if(g_nGameCoins > nMissionsPrice[m_nSelect])
	{
        g_nMissionsUnlocked[m_nSelect] = true;

		if (g_nAvailableLevel[m_nSelect] < 0)
			g_nAvailableLevel[m_nSelect] = 0;

        g_nGameCoins -= nMissionsPrice[m_nSelect];
        changeMissions();
        m_app->saveSetting();
    }
	else
	{
        onSelectLess();
	}
}

void MissionsLayer::onSelect()
{
    g_nHeroSkinIndex = m_nSelect;
}

void MissionsLayer::changeMissions()
{
	m_lblCoins->setString(CCString::createWithFormat("%d", nMissionsPrice[m_nSelect])->getCString());
    
    CCString *strMarkName;
    if(g_nMissionsUnlocked[m_nSelect])
		strMarkName = CCString::createWithFormat("store/store_levelmark%d", m_nSelect + 1);
    else
		strMarkName = CCString::createWithFormat("store/store_levelmark%d_lock", m_nSelect + 1);

	const char*	szFile = m_resMgr->makeFileName(strMarkName->getCString(), "png");
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(szFile);
    m_sprHero->setTexture(texture);
}

void MissionsLayer::onSelectLess()
{
	ModalAlert::Tell("You do not have enough coins.",this,NULL);
}
