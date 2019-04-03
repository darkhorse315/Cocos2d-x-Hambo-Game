//
//  SkipLayer.h
//  Hambo
//
//  Created by admin on 7/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "cocos-ext.h"

class SkipLayer : public ScaleLayer
{
public:
	SkipLayer();
	~SkipLayer();

private:
	void createButtons();
	void onSelectLess();
	void onSkip1(CCObject* sender, cocos2d::extension::CCControlEvent event);
	void onSkip2(CCObject* sender, cocos2d::extension::CCControlEvent event);
	void onSkip3(CCObject* sender, cocos2d::extension::CCControlEvent event);
	void onSkip4(CCObject* sender, cocos2d::extension::CCControlEvent event);
};