//
//  MissionsLayer.h
//  Hambo
//
//  Created by admin on 7/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#ifndef MISSIONLAYER_H
#define MISSIONLAYER_H

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "ccframework/GrowButton.h"
#include "cocos-ext.h"
//#include "CCLabelFX.h"

USING_NS_CC;

class MissionsLayer : public ScaleLayer
{
private:
    CCSprite *m_sprHero;
    GrowButton *m_btnUnlock;
    CCLabelBMFont  *m_lblCoins;
    int         m_nSelect;

public:
	MissionsLayer();
	~MissionsLayer();

private:
	void createBackgrounds();
	void createButtons();
	void changeMissions();
	void onSelectLess();
	void onPrev(CCObject*sender, cocos2d::extension::CCControlEvent event);
	void onNext(CCObject*sender, cocos2d::extension::CCControlEvent event);
	void onUnlock(CCObject*sender, cocos2d::extension::CCControlEvent event);
	void onTime(float dt);
	void onSelect();
};

#endif
