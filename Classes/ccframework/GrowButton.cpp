//
//  GrowButton.m
//  Game
//
//  Created by hrh on 9/6/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
 
#include "GrowButton.h"
// #include "ResourceManager.h"
// #include "Constants.h"
#include "GUI/CCControlExtension/CCScale9Sprite.h"

USING_NS_CC_EXT;

GrowButton* GrowButton::create(cocos2d::extension::CCScale9Sprite* sprite)
{
	GrowButton *pRet = new GrowButton();
	pRet->initWithBackgroundSprite(sprite);
	pRet->autorelease();
	return pRet;
}

GrowButton* GrowButton::create(CCNode* label, cocos2d::extension::CCScale9Sprite* sprite)
{
	GrowButton *pRet = new GrowButton();
	pRet->initWithLabelAndBackgroundSprite(label, sprite);
	pRet->autorelease();
	return pRet;
}

GrowButton* GrowButton::buttonWithSpriteFileName(const char* normalName, const char * selectName, const char *disableImage, CCNode *target, SEL_CCControlHandler selector, int index)
{
	if (strlen(disableImage) == 0)
        disableImage = selectName;

	CCString*	strNorm = CCString::createWithFormat("ipad_res/%s-ipad.png", normalName);
	CCString*	strHigh = CCString::createWithFormat("ipad_res/%s-ipad.png", selectName);
	CCString*	strDisa = CCString::createWithFormat("ipad_res/%s-ipad.png", disableImage);

	CCScale9Sprite *spriteNorm = CCScale9Sprite::create( strNorm->getCString() );
	CCScale9Sprite *spriteHigh = CCScale9Sprite::create( strHigh->getCString() );
	CCScale9Sprite *spriteDisa = CCScale9Sprite::create( strDisa->getCString() );
	
	CCAssert(spriteNorm != NULL, __FUNCTION__ " Don't create CCScale9Sprite!!!");
	CCAssert(spriteHigh != NULL, __FUNCTION__ " Don't create CCScale9Sprite!!!");
	CCAssert(spriteDisa != NULL, __FUNCTION__ " Don't create CCScale9Sprite!!!");

//	SEL_CCControlHandler selector = cccontrol_selector(GameLayer::onPause);

	GrowButton *button = GrowButton::create(spriteNorm);
	button->setBackgroundSpriteForState(spriteHigh, CCControlStateHighlighted);
	button->setBackgroundSpriteForState(spriteDisa, CCControlStateDisabled);
	button->setAdjustBackgroundImage(false);
	button->addTargetWithActionForControlEvents(target, selector, CCControlEventTouchUpInside);
	button->setTag(index);

	return button;
}

GrowButton* GrowButton::buttonWithTitleAndSprite(const char* title, const char* normalName, CCNode *target, cocos2d::extension::SEL_CCControlHandler sel, int index)
{
	CCString*	strNorm = CCString::createWithFormat("ipad_res/%s-ipad.png", normalName);

	CCScale9Sprite *spriteNorm = CCScale9Sprite::create( strNorm->getCString() );

	CCAssert(spriteNorm != NULL, __FUNCTION__ " Don't create CCScale9Sprite!!!");

	CCLabelTTF *titleButton = CCLabelTTF::create(title, "Marker Felt", 30);

	GrowButton *button = GrowButton::create(titleButton, spriteNorm);
	button->setTitleColorForState(ccWHITE, CCControlStateHighlighted);
	button->setTitleColorForState(ccGRAY, CCControlStateDisabled);
	button->setAdjustBackgroundImage(false);
	button->addTargetWithActionForControlEvents(target, sel, CCControlEventTouchUpInside);
	button->setTag(index);

	return button;
}

GrowButton* GrowButton::buttonWithSpriteFileName(const char* normalName, const char * selectName, CCNode *target, SEL_CCControlHandler selector, int index)
{
    return buttonWithSpriteFileName(normalName, selectName, "", target, selector, index);
}
 
GrowButton* GrowButton::buttonWithSpriteFileName(const char* normalName, const char * selectName, CCNode *target, SEL_CCControlHandler selector)
{
    return buttonWithSpriteFileName(normalName, selectName, target, selector, 0);
}
