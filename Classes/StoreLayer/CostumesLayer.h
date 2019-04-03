//
//  CostumesLayer.h
//  Hambo
//
//  Created by admin on 7/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CONSTUMESLAYER_H
#define CONSTUMESLAYER_H

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "ccframework/GrowButton.h"
#include "cocos-ext.h"

USING_NS_CC;

class ConstumesLayer : public ScaleLayer
{
private:
    CCSprite*		m_sprHero;
    GrowButton*		m_btnSelect;
    GrowButton*		m_btnUnlock;
    int				m_nSelect;
    CCSprite*		m_sprSelected;
    CCLabelBMFont*	m_lblCoins;

public:
	ConstumesLayer();
	~ConstumesLayer();

private:
	void createBackgrounds();
	void createButtons();
	void onTime(float dt);
	void updateCharacter();
	void onSelectLess();
	void onPrevHero(CCObject* sender, cocos2d::extension::CCControlEvent event);
	void onNextHero(CCObject* sender, cocos2d::extension::CCControlEvent event);
	void onSelect( CCObject * sender, cocos2d::extension::CCControlEvent event );
	void onUnlock( CCObject * sender, cocos2d::extension::CCControlEvent event );
};

#endif
