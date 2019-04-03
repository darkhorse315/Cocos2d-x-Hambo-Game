//
//  BombEffect.m
//  OutZone_iphone
//
//  Created by admin on 2/10/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BombEffect.h"

BombEffect::BombEffect(CCAnimation *animation)
{
	if ( !CCSprite::init() )
		return;
    m_bEnd = false;
	animation->setDelayPerUnit(0.1f);
	CCAnimate * ani = CCAnimate::create(animation);
	runAction(CCSequence::create(ani, CCCallFunc::create(this,callfunc_selector(BombEffect::onFinishBombEffect)), NULL));
}

void BombEffect::onFinishBombEffect() {
    m_bEnd = true;
    stopAllActions();
}

BombEffect::~BombEffect() {
    stopAllActions();
    unscheduleAllSelectors();
    removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}