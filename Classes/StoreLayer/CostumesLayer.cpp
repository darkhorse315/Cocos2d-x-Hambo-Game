//
//  CostumesLayer.m
//  Hambo
//
//  Created by admin on 7/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CostumesLayer.h"
#include "ccframework/ResourceManager.h"
#include "Constants.h"
#include "ccframework/GrowButton.h"
#include "StoreLayer.h"
#include "AppDelegate.h"
#include "ModalAlert/ModalAlert.h"

USING_NS_CC_EXT;

int nPrice[5] = {0, 1000, 1000, 1000, 1000};

ConstumesLayer::ConstumesLayer()
{
	if ( ScaleLayer::init() )
	{
// 		m_nSelect = 0;
		m_nSelect = g_nHeroSkinIndex;
		CCAssert(m_nSelect >= 0 && m_nSelect < CHARACTER_TYPE_COUNT, "Character type error!");
		createBackgrounds();
		createButtons();
		updateCharacter();

		schedule(schedule_selector(ConstumesLayer::onTime));
	}
}

ConstumesLayer::~ConstumesLayer()
{
	unscheduleAllSelectors();
	removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void ConstumesLayer::createBackgrounds()
{
// 	CCSprite *sprBG = m_resMgr->getSpriteWithName("store/costumes_back");
// 	addChild(sprBG);
// 	sprBG->setPosition(ccp(640, 553));

	m_sprHero = m_resMgr->getSpriteWithName("store/chaecter1");
	addChild(m_sprHero);
	m_sprHero->setPosition(ccp(iDevPixelX(620), iDevPixelY(455)));

	CCSprite *sprCoins = m_resMgr->getSpriteWithName("store/money");
	addChild(sprCoins);
	sprCoins->setPosition(ccp(iDevPixelX(508), iDevPixelY(579)));

	m_lblCoins = CCLabelBMFont::create("0","font/MainMenuFont@2x.fnt");
	m_lblCoins->setPosition(ccp(iDevPixelX(562), iDevPixelY(579)));
	m_lblCoins->setAlignment(kCCTextAlignmentLeft);
	m_lblCoins->setAnchorPoint( ccp(0.0f, 0.5f) );
	m_lblCoins->setScale( iDevSize(1.3f) );
	addChild(m_lblCoins);

	CCString*	string = CCString::createWithFormat("%d", nPrice[m_nSelect]);
	m_lblCoins->setString(string->getCString());
}

void ConstumesLayer::createButtons()
{
	GrowButton *btnPrevHero = GrowButton::buttonWithSpriteFileName("store/btnPrev1", "store/btnPrev2", this, cccontrol_selector(ConstumesLayer::onPrevHero));
	btnPrevHero->setPosition(ccp(iDevPixelX(454), iDevPixelY(460)));
	addChild(btnPrevHero);

	GrowButton *btnNextHero = GrowButton::buttonWithSpriteFileName("store/store_btnNext1", "store/store_btnNext2", this,cccontrol_selector(ConstumesLayer::onNextHero));
	btnNextHero->setPosition(ccp(iDevPixelX(810), iDevPixelY(460)));
	addChild(btnNextHero);

	//btnCommon
	m_btnSelect = GrowButton::buttonWithSpriteFileName("store/btnSelect1", "store/btnSelect2", this, cccontrol_selector(ConstumesLayer::onSelect));
	m_btnSelect->setPosition(ccp(iDevPixelX(637), iDevPixelY(280)));
	addChild(m_btnSelect);

	m_btnUnlock = GrowButton::buttonWithSpriteFileName("store/btnUnlock1", "store/btnUnlock2", this, cccontrol_selector(ConstumesLayer::onUnlock));
	m_btnUnlock->setPosition(ccp(iDevPixelX(637), iDevPixelY(280)));
	addChild(m_btnUnlock);

	m_sprSelected = m_resMgr->getSpriteWithName("store/btnSelected");
	addChild(m_sprSelected);
	m_sprSelected->setPosition(ccp(iDevPixelX(637), iDevPixelY(280)));
	//CCLabelTTF*	title;
	//title = CCLabelTTF::create("Selected", "Helvetica", 30);
	//title->setPosition(ccp(129, 37));
	//m_sprSelected->addChild(title);
}

void ConstumesLayer::onTime( float dt )
{
    if(g_nHeroUnlocked[m_nSelect])
	{
        m_btnUnlock->setVisible(false);
		m_btnUnlock->setEnabled(false);
        if(m_nSelect == g_nHeroSkinIndex)
		{
            m_sprSelected->setVisible(true);
            m_btnSelect->setVisible(false);
        }
		else
		{
            m_sprSelected->setVisible(false);
            m_btnSelect->setVisible(true);
        }
    }
	else
	{
        m_btnUnlock->setVisible(true);    
		m_btnUnlock->setEnabled(true);
        m_sprSelected->setVisible(false);
        m_btnSelect->setVisible(false);
    }
    
}

void ConstumesLayer::onPrevHero( CCObject * sender, CCControlEvent event )
{
	m_app->playEff(E_CLICK);

    m_nSelect --;
    if(m_nSelect < 0)
        m_nSelect = CHARACTER_TYPE_COUNT-1;
    updateCharacter();
}

void ConstumesLayer::onNextHero( CCObject * sender, CCControlEvent event )
{
	m_app->playEff(E_CLICK);

    m_nSelect ++;
    if(m_nSelect >= CHARACTER_TYPE_COUNT)
        m_nSelect = 0;
    updateCharacter();
}

void ConstumesLayer::onUnlock( CCObject * sender, CCControlEvent event )
{
	m_app->playEff(E_CLICK);

    if(g_nGameCoins > nPrice[m_nSelect])
	{
        g_nHeroUnlocked[m_nSelect] = true;
        g_nGameCoins -= nPrice[m_nSelect];
		m_app->saveSetting();
    }
	else
	{
        onSelectLess();
	}
}

void ConstumesLayer::onSelect( CCObject * sender, CCControlEvent event )
{
	m_app->playEff(E_CLICK);
    g_nHeroSkinIndex = m_nSelect;
    m_app->saveSetting();
}

void ConstumesLayer::updateCharacter()
{
	m_lblCoins->setString(CCString::createWithFormat("%d", nPrice[m_nSelect])->getCString());

	CCString *strSprite = CCString::createWithFormat("store/chaecter%d", m_nSelect + 1);
	const char*	szFile = m_resMgr->makeFileName(strSprite->getCString(),"png");
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(szFile);
    m_sprHero->setTexture(texture);
}

void ConstumesLayer::onSelectLess()
{
	ModalAlert::Tell("You do not have enough coins.",this,NULL);
}

