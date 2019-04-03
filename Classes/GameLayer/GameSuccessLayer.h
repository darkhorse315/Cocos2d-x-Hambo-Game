//
//  GameSuccessLayer.h
//  hotair
//
//  Created by admin on 12/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "cocos-ext.h"

class GameLayer;

class SHParticleFireWorks : public cocos2d::CCParticleSystemQuad
{
public:
	SHParticleFireWorks();
};

class GameSuccessLayer : public ScaleLayer
{
private:
	CCLabelBMFont* levelCount;
	CCLabelBMFont* scoreCount;
	CCLabelBMFont* coinCount;
	CCLabelBMFont* bestScoreCount;

	int nGameScore;
public:
	GameSuccessLayer() {};
	void setInfo(GameLayer* parent,int score);
	bool init();
	CREATE_FUNC(GameSuccessLayer);
	~GameSuccessLayer();

private:
	void createBackground();
	void createInfo();
	void createButtons();

	void actionGameSuccess();
	void addBackEmitter();

	void onShop(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onMainmenu(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onRestart(CCObject *sender, cocos2d::extension::CCControlEvent event);
	void onNext(CCObject *sender, cocos2d::extension::CCControlEvent event);
};