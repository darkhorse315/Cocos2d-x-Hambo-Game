//
//  GrowButton.h
//  Game
//
//  Created by hrh on 9/6/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef GROWBUTTON_H
#define GROWBUTTON_H


#include "cocos2d.h"
#include "cocos-ext.h"
#include "GUI/CCControlExtension/CCScale9Sprite.h"

#define Tag_Item	1

USING_NS_CC;


class GrowButton : public cocos2d::extension::CCControlButton 
{    
public:
	static GrowButton* create(cocos2d::extension::CCScale9Sprite* sprite);
	static GrowButton* create(CCNode* label, cocos2d::extension::CCScale9Sprite* sprite);

	static GrowButton* buttonWithSpriteFileName(const char* normalName,const char * selectName, const char *disableImage, CCNode *target, cocos2d::extension::SEL_CCControlHandler sel, int index);
	static GrowButton* buttonWithSpriteFileName(const char* normalName,const char * selectName, CCNode *target, cocos2d::extension::SEL_CCControlHandler sel, int index);
	static GrowButton* buttonWithSpriteFileName(const char* normalName,const char * selectName, CCNode *target, cocos2d::extension::SEL_CCControlHandler sel);

	static GrowButton* buttonWithTitleAndSprite(const char* title, const char* normalName, CCNode *target, cocos2d::extension::SEL_CCControlHandler sel, int index = 0);
};

#endif