//
//  GameOverLayer.h
//  hotair
//
//  Created by admin on 12/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "cocos-ext.h"

USING_NS_CC;

class GameLayer;

class GameOverLayer : public ScaleLayer
{
private:
	CCLabelBMFont* levelCount;
	CCLabelBMFont* scoreCount;
	CCLabelBMFont* coinCount;
	CCLabelBMFont* bestScoreCount;

	int nGameScore;
public:
	GameOverLayer() {};
	void setInfo(GameLayer* parent,int score);
	bool init();
	~GameOverLayer();
	CREATE_FUNC(GameOverLayer);

private:
	void createBackground();
	void createInfo();
	void createButtons();

	void onSelectLess();
	void onShop(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onMainMenu( CCObject *sender, cocos2d::extension::CCControlEvent event );
	void onRestart( CCObject *sender, cocos2d::extension::CCControlEvent event );
	void onSkip( CCObject *sender, cocos2d::extension::CCControlEvent event );
	void onTutorial( CCObject *sender, cocos2d::extension::CCControlEvent event );
};
