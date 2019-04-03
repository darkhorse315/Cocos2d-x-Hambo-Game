//
//  BombEffectManager.m
//  OutZone_iphone
//
//  Created by admin on 2/10/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "gamelayer/bullet/BombEffectManager.h"
#include "Constants.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "gamelayer/bullet/BombEffect.h"
#include "ccframework/ResourceManager.h"

BombEffectManager::BombEffectManager(){
	if ( !ScaleLayer::init() )
		return;
	
	m_arrBullets = new CCArray();
	schedule(schedule_selector(BombEffectManager::onTime),1.0f / 30.0f); 
}

void BombEffectManager::onTime(float t)
{
    for(int i = 0; i < (int)m_arrBullets->count(); i++){
        if(((BombEffect*)m_arrBullets->objectAtIndex(i))->m_bEnd){
            removeChild((CCNode*)m_arrBullets->objectAtIndex(i),true);
            m_arrBullets->removeObjectAtIndex(i);
        }
    }
}

void BombEffectManager::makeBombEffect(CCPoint ptPos,int nType)
{
    int nSize = 0;
	CCAnimation* animation = CCAnimation::create();
    if(nType == 1){
        nSize = 6;
        for (int i = 0; i < nSize; i++) {
			animation->addSpriteFrameWithFileName(m_resMgr->makeFileName(CCString::createWithFormat("game/bomb_effect/bomb_effect%d", i + 1)->getCString(),"png"));
        }
    }else if(nType == 2){
        nSize = 7;
        for (int i = 0; i < nSize; i++) {
            animation->addSpriteFrameWithFileName(m_resMgr->makeFileName(CCString::createWithFormat("game/enemy_die_effect/enemy_die_effect%d", i + 1)->getCString(),"png"));
        }
    }else if(nType == 3){
        nSize = 5;
        for (int i = 0; i < nSize; i++) {
            animation->addSpriteFrameWithFileName(m_resMgr->makeFileName(CCString::createWithFormat("game/ice_break_effect/ice_break%d", i + 1)->getCString(),"png"));
        }
    }
	else if(nType == 4){
		nSize = 10;
		for (int i = 0; i < nSize; i+=2) {
			animation->addSpriteFrameWithFileName(m_resMgr->makeFileName(CCString::createWithFormat("game/bomb_effect/ba%d", i + 1)->getCString(),"png"));
		}
	}
	BombEffect *be = new BombEffect(animation);
	addChild(be);
    be->setPosition(ptPos);
    
    m_arrBullets->addObject(be);
}

void BombEffectManager::moveBulletsEffect(CCPoint ptOffset) {
    for(int i = 0; i < (int)m_arrBullets->count(); i++){
        if(!((BombEffect*)m_arrBullets->objectAtIndex(i))->m_bEnd){
            CCPoint pt = ((BombEffect*)m_arrBullets->objectAtIndex(i))->getPosition();
            ((CCNode*)m_arrBullets->objectAtIndex(i))->setPosition(ccpAdd(pt, ptOffset));
        }
    }
}

BombEffectManager::~BombEffectManager(){
    m_arrBullets->removeAllObjects();
    m_arrBullets->release();
    unscheduleAllSelectors();
    removeAllChildrenWithCleanup(true);
}