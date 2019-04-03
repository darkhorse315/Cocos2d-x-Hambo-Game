//
//  PauseLayer.h
//  BalloonBonanza_iphone
//
//  Created by admin on 1/4/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "cocos-ext.h"

class  GameLayer;

class PauseLayer : public ScaleLayer
{
private:
	CCLayerColor* backLayer;

	CCLabelBMFont* levelCount;
	CCLabelBMFont* scoreCount;
	CCLabelBMFont* coinCount;
	CCLabelBMFont* bestScoreCount;

public:
	PauseLayer(GameLayer* parent,int score);
	PauseLayer() {};
	~PauseLayer();

	GameLayer* m_Parent;
	int nGameScore;
	bool init();
	void setInfo(GameLayer* p,int nscore);
private:
	void createBackground();
	void createInfo();
	void createButtons();

	void onSelectLess();
	void onMainmenu(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onResume(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onTutorial(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onShop(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onSkip(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onRestart(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onSolution(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void close(float dt);
public:
	CREATE_FUNC(PauseLayer);
};