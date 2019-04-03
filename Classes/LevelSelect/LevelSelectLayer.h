//
//  LevelSelectLayer.h
//  JarHead
//
//  Created by admin on 6/26/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "cocos-ext.h"
#include "ccframework/GrowButton.h"

class LevelSelectLayer : public ScaleLayer 
{
private:
    CCArray*		m_arrayStageSprite;
	CCPoint			m_pointTouchDown;
	CCLabelBMFont*	m_lblLevel;
	int				m_nCurStage;
	GrowButton *	btnBack;
	GrowButton *	btnClose;

public:
	~LevelSelectLayer();
	bool init();
	static CCScene* scene();
	CREATE_FUNC(LevelSelectLayer);

private:
	void createBackground();
	void createStageInfo();
	void createStageButtons();

	void updateInfo();
	void onStage(int nStageNo);
	void adjustScale(float offset);
	bool isTap(CCPoint ptDown, CCPoint ptUp);
	CCSprite* getTouchedSprite(CCPoint point);
	CCSprite* getNearestSprite(CCPoint point);
	void moveAllStageImage(CCPoint delta);

	void onClose( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onQuitOK( CCObject* sender );
	void onBack( CCObject* sender, cocos2d::extension::CCControlEvent event );

	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch* touch, CCEvent *event);
	void ccTouchMoved(CCTouch* touch, CCEvent *event);
	void ccTouchEnded(CCTouch* touch, CCEvent *event);
};