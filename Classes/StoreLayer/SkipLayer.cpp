//
//  SkipLayer.m
//  Hambo
//
//  Created by admin on 7/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SkipLayer.h"
#include "ccframework/GrowButton.h"
#include "Constants.h"
#include "AppDelegate.h"
#include "ModalAlert/ModalAlert.h"

USING_NS_CC_EXT;

SkipLayer::SkipLayer()
{
	if ( ScaleLayer::init() )
	{
		createButtons();
    }
}

SkipLayer::~SkipLayer()
{
	unscheduleAllSelectors();
	removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void SkipLayer::createButtons()
{
	GrowButton *btnSol1 = GrowButton::buttonWithSpriteFileName("store/skip_1", "store/skip_1", this, cccontrol_selector(SkipLayer::onSkip1));
	btnSol1->setPosition(ccp(iDevPixelX(518), iDevPixelY(502)));
	addChild(btnSol1);

	GrowButton *btnSol2 = GrowButton::buttonWithSpriteFileName("store/skip_2", "store/skip_2", this, cccontrol_selector(SkipLayer::onSkip2));
	btnSol2->setPosition(ccp(iDevPixelX(764), iDevPixelY(502)));
	addChild(btnSol2);

	GrowButton *btnSol3 = GrowButton::buttonWithSpriteFileName("store/skip_3", "store/skip_3", this, cccontrol_selector(SkipLayer::onSkip3));
	btnSol3->setPosition(ccp(iDevPixelX(518), iDevPixelY(322)));
	addChild(btnSol3);

	GrowButton *btnSol4 = GrowButton::buttonWithSpriteFileName("store/skip_4", "store/skip_4", this, cccontrol_selector(SkipLayer::onSkip4));
	btnSol4->setPosition(ccp(iDevPixelX(764), iDevPixelY(322)));
	addChild(btnSol4);
}

void SkipLayer::onSkip1(CCObject* sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

    if(g_nGameCoins < 1500)
	{
        onSelectLess();
	}
    else
	{
        g_nSkipsNumber += 3;
        g_nGameCoins -= 1500;
    }
    m_app->saveSetting();
}

void SkipLayer::onSkip2(CCObject* sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

    if(g_nGameCoins < 2700)
	{
        onSelectLess();
	}
    else
	{
        g_nSkipsNumber += 6;
        g_nGameCoins -= 2700;
    }
    m_app->saveSetting();
}

void SkipLayer::onSkip3(CCObject* sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

    if(g_nGameCoins < 4000)
	{
        onSelectLess();
	}
    else
	{
        g_nSkipsNumber += 9;
        g_nGameCoins -= 4000;
    }
    m_app->saveSetting();
}

void SkipLayer::onSkip4(CCObject* sender, CCControlEvent event)
{
	m_app->playEff(E_CLICK);

    if(g_nGameCoins < 5200)
	{
        onSelectLess();
	}
    else
	{
        g_nSkipsNumber += 12;
        g_nGameCoins -= 5200;
    }
    m_app->saveSetting();
}

void SkipLayer::onSelectLess()
{
	ModalAlert::Tell("You do not have enough coins.",this,NULL);
}

