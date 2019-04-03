//
//  StageSelectLayer.h
//  JarHead
//
//  Created by admin on 6/26/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "cocos-ext.h"

class StageSelectLayer : public ScaleLayer
{
private:
    ScaleLayer*	m_stageLayer;
	CCLabelBMFont*	m_lblLevel;

public:
	~StageSelectLayer();
	bool init();
	static CCScene* scene();
	CREATE_FUNC(StageSelectLayer);

private:
	void createBackground();
	void createInfo();
	void createStage();

	void onClose( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onQuitOK( CCObject* sender );
	void onStage(CCObject* sender, cocos2d::extension::CCControlEvent event);
	void onBack(CCObject* sender, cocos2d::extension::CCControlEvent event);
};