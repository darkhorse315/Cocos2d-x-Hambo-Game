//
//  TitleLayer.h
//  hotair
//
//  Created by admin on 12/6/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "ccframework/GrowButton.h"
#include "cocos-ext.h"

class AppDelegate;

class TitleLayer : public ScaleLayer
{
private:
	GrowButton *m_menuItem[6];
	CCSprite * titleSp;

public:
	static CCScene* scene();
	CREATE_FUNC(TitleLayer);
	virtual bool init(); 
	~TitleLayer();

private:
	void createBackground();
	void createButtons();

	void onClose( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onQuitOK( CCObject* sender );
	void onPlay( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onOptions( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onHighscore( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onShop( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onMoreGame( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onInitAction(float dt);
};
