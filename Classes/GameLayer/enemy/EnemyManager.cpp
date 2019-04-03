//
//  EnemyManager.m
//  OutZone_iphone
//
//  Created by admin on 2/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "EnemyManager.h"
#include "Constants.h"
#include "Enemy.h"

void EnemyManager::initWithLevelHelper (LevelHelperLoader *hLevel){
    m_level = hLevel;
    m_bExist = true;
	
    runEnemies();
}

void EnemyManager::runEnemies(){
    if(g_bGamePause)
        return;
    
    CCArray *m_arrEnemies = (CCArray*)m_level->spritesWithTag(ENEMY_TAG);    
    for(int i = 0; i < (int)m_arrEnemies->count(); i++){        
        LHSprite * sprEnemy = (LHSprite*)m_arrEnemies->objectAtIndex(i);
        manageEnemies(sprEnemy,1,false);
    }
    m_arrEnemies = (CCArray*)m_level->spritesWithTag(ICE_TAG);    
    for(int i = 0; i < (int)m_arrEnemies->count(); i++){        
        LHSprite * sprEnemy = (LHSprite*)m_arrEnemies->objectAtIndex(i);
        manageEnemies(sprEnemy,1,true);
    }
}

void EnemyManager::manageEnemies(LHSprite* sprEnemy ,int nLife ,bool bIsIce)
{
    Enemy *e = new Enemy();
    e->initInfo(m_level,sprEnemy ,nLife ,bIsIce);
    m_level->cocosLayer->addChild(e);
}

void EnemyManager::checkDestoriedAllEnemies() {
    m_bExist = false;
    for(int i = ENEMY_START_TAG + 1; i < ENEMY_END_TAG; i++){
        CCArray *arrEnemies = m_level->spritesWithTag((enum LevelHelper_TAG)i);

		CCObject* obj;
		CCARRAY_FOREACH(arrEnemies,obj)
		{
			LHSprite* spr = (LHSprite*)obj;
			if ( isHeroFriend(spr) )
				arrEnemies->removeObject(obj,false);
		}

        if(arrEnemies->count() > 0)
            m_bExist |= true;
    }
}

bool EnemyManager::isDestroyAllEnemies()
{
    checkDestoriedAllEnemies();
    return !m_bExist;
}

EnemyManager::~EnemyManager()
{
    unscheduleAllSelectors();
    removeAllChildrenWithCleanup(true);
}