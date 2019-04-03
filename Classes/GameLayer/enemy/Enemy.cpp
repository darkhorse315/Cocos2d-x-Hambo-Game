//
//  Enemy.m
//  OutZone_iphone
//
//  Created by admin on 2/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Enemy.h"
#include "Constants.h"
#include "gamelayer/hero/HeroManager.h"
#include "gamelayer/bullet/Bullet.h"
#include "gamelayer/GameLayer.h"
#include "LevelHelper/Nodes/LHSettings.h"
#include "ccframework/ResourceManager.h"
#include "AppDelegate.h"
#include "EatItem.h"

#define ENEMY_MOVE_TIME 50

#define ENEMY_IDLE_IMAGE_COUNT 13
#define ENEMY_IDLE_ANIMATION_DELAY_MIN 5.0f
#define ENEMY_IDLE_ANIMATION_DELAY_MAX 10.0f
#define ENEMY_IDLE_ANIMATION_FRAME_DELAY 0.2f

void Enemy::initInfo(LevelHelperLoader* hLevel,LHSprite* sprEnemy,int nLife ,bool bIsIce)
{
	//    NSLog(@"%@ was created", [spr uniqueName]);
    m_spr = NULL;
    m_nType = sprEnemy->getTag();
    m_nInitLife = m_nLife = nLife;
    m_spr = sprEnemy; 
    m_level = hLevel; 
    m_bIsIce = bIsIce; 
	m_bDestroyed = false;
	bAfraid = false;
	m_bFallDie = false;
	m_helpArrow = NULL;

	if ( m_spr->getPositionX() < SCREEN_WIDTH / 2 )
		m_spr->setScaleX( -m_spr->getScaleX() );
	
	m_afraidSprite = NULL;

	//Afraid Image Loadding
	if ( !m_bIsIce && !isHeroFriend(m_spr) )
	{
		int ind = m_spr->getUniqueName()->m_sString[5]-'0';
		CCAnimation* animation = CCAnimation::create();
		ResourceManager*	resMgr = ResourceManager::sharedResourceManager();
		for ( int i = 0; i < 13;i++ )
			animation->addSpriteFrameWithFileName(resMgr->makeFileName(CCString::createWithFormat("game/enemy/enemy%d/afraid/A_000%02d", ind,i+1)->getCString(),"png"));

		animation->setDelayPerUnit(0.1f);
		CCAnimate *afraidAnim = CCAnimate::create(animation);
		m_AfraidAnimation = CCRepeatForever::create(afraidAnim);

		if ( !m_afraidSprite )
		{
			m_afraidSprite = CCSprite::create();
			m_level->cocosLayer->addChild(m_afraidSprite,m_spr->getZOrder() + 1);
			m_afraidSprite->setPosition( m_spr->getPosition() );
			m_afraidSprite->setScaleX( m_spr->getScaleX() );
			m_afraidSprite->setScaleY( m_spr->getScaleY() );
			m_afraidSprite->setVisible(false);
			m_afraidSprite->runAction(m_AfraidAnimation);
		}

		m_AfraidAnimation->retain();

		//////////////////////////////////////////////////////////////////////////
		//Idle Image Loading
		animation = CCAnimation::create();
		for ( int i = 0; i < ENEMY_IDLE_IMAGE_COUNT;i++ )
			animation->addSpriteFrameWithFileName(resMgr->makeFileName(CCString::createWithFormat("game/enemy/enemy%d/idle/A_idle_000%02d", ind,i+1)->getCString(),"png"));

		animation->setDelayPerUnit(ENEMY_IDLE_ANIMATION_FRAME_DELAY);
		m_IdleAnimation = CCSequence::createWithTwoActions(CCAnimate::create(animation),CCCallFunc::create(this, callfunc_selector(Enemy::completeAnimation)) );
		m_IdleAnimation->retain();
		onTimeForIdle(0);
		//////////////////////////////////////////////////////////////////////////
	}
	bAfraid = false;
	
	schedule(schedule_selector(Enemy::onTime)/*,1.0f / 10.0f*/);
}

void Enemy::onTime(float t)
{
    if(g_bGamePause)
        return;
    if(!m_spr->isVisible())
        return;    
    checkingSubLife();
    checkDestroy();
	enemyAfraidAnim();
}

void Enemy::onTimeForIdle(float t)
{
	float dt = RandomFloat( ENEMY_IDLE_ANIMATION_DELAY_MIN, ENEMY_IDLE_ANIMATION_DELAY_MAX );
	schedule( schedule_selector(Enemy::onTimeForIdle),dt );
	if ( t == 0 || bAfraid ) return;
	
	if ( m_afraidSprite )
	{
		m_afraidSprite->setVisible(true);
		m_spr->setOpacity(0);
		m_afraidSprite->stopAllActions();
		m_afraidSprite->runAction(m_IdleAnimation);
	}
}

void Enemy::processIce() {
    
}

//#pragma mark -
//#pragma mark sub life.

void Enemy::checkingSubLife() {
    if(m_bIsIce)
        return;
    if(m_nLife < 0)
        return;
    for(int i = 0; i < (int)g_arrBombEffect->count(); i++){
        CCString *strInfo = (CCString*)g_arrBombEffect->objectAtIndex(i);
        CCArray *arrCom = componentsSeparatedByString(strInfo,",");
        if(((CCString*)arrCom->objectAtIndex(4))->m_sString.compare(m_spr->getUniqueName()->m_sString) != 0)
            continue;
        //enemyCrashedAnim();
        m_nLife -= ((CCString*)arrCom->objectAtIndex(5))->intValue();
        if(m_nLife <= 0){
            hideEnemy();
			
            //[[SoundManager sharedSoundManager] playEffect:@"die.mp3" bForce: YES];
        }
        g_arrBombEffect->removeObjectAtIndex(i);
        break;
    }
}

void Enemy::hideEnemy() {
	if(m_bIsIce)
		return;
	m_spr->setVisible(false);
	m_spr->setTag(DELETED_TAG);
	unscheduleAllSelectors();
	checkDestroy();
}


void Enemy::enemyCrashedAnim() {
	if(m_bIsIce)
        return;
    CCString *strEnemyName = m_spr->getUniqueName();
	CCString *strRootName = CCString::create(strEnemyName->m_sString.substr(6));    
    CCString *strCrashAnimName;
    if(strRootName->m_sString.compare("enemy1"))
		strCrashAnimName = CCString::create("enemy1_crash_anim");
    else if(strRootName->m_sString.compare("enemy2"))
        strCrashAnimName = CCString::create("enemy2_crash_anim");
    else 
        strCrashAnimName = CCString::create("enemy3_crash_anim");
    m_level->startAnimationWithUniqueName(strCrashAnimName,m_spr);
}

void Enemy::enemyAfraidAnim()
{
	if(m_bIsIce || isHeroFriend(m_spr))
		return;

	if ( m_afraidSprite )
	{
		m_afraidSprite->setPosition( m_spr->getPosition() );
		m_afraidSprite->setRotation( m_spr->getRotation() );
	}
	
	if ( m_spr->afraidState )
	{
		if ( !bAfraid )
		{
			if ( m_spr->getTag() == ENEMY_TAG )
			{
				m_afraidSprite->setVisible(true);
				m_afraidSprite->stopAllActions();
				m_afraidSprite->runAction(m_AfraidAnimation);
				m_spr->setOpacity(0);
				bAfraid = true;
			}
		}
	}
	else
	{
		if ( bAfraid )
		{
			m_afraidSprite->stopAllActions();
			m_afraidSprite->setVisible(false);
			m_spr->setOpacity(255);
			bAfraid = false;
		}
	}
}

void Enemy::completeAnimation()
{
	m_afraidSprite->stopAllActions();
	m_afraidSprite->setVisible(false);
	m_spr->setOpacity(255);
}

void Enemy::checkDestroy() {
    if (m_spr->getPosition().y < -64 ||
		m_spr->getPosition().x < -64 ||
		m_spr->getPosition().x > SCREEN_WIDTH + 64)
	{
        m_spr->setTag(DELETED_TAG);		
		m_bFallDie = true;
	}

    if(m_spr->getTag() == DELETED_TAG)
	{
        destroy();
        processIce();
    }
}

void Enemy::makeEatItem() {
    int nCount = 4;
    for(int i = 0; i < nCount; i++){
        EatItem *ei = new EatItem();
        ei->initInfo(m_spr->getPosition(),m_level->box2dWorld,i);
        m_level->cocosLayer->addChild(ei);
    }
}

void Enemy::destroy() {  
	if ( m_bDestroyed ) return;

	//Tutorial Insert
	int *sprInd = (int*)m_spr->getUserData();
	if ( sprInd )
		((GameLayer*)m_level->cocosLayer)->m_TutotialWrite.addTutorial( ((GameLayer*)m_level->cocosLayer)->m_nTick+0.07f,BODYDELETE,CCPointZero,CCPointZero,*sprInd );
	//////////////////////////////////////////////////////////////////////////

	m_bDestroyed = true;
	//  [(AppDelegate*)[[UIApplication sharedApplication] delegate] playEff:E_ENEMY_DIE];
    if(!m_bIsIce){
        //((GameLayer*)getParent())->m_bmbManager->makeBombEffect(m_spr->getPosition(),1);

		if ( !isHeroFriend(m_spr) )
		{
			makeEatItem();
			((GameLayer*)m_level->cocosLayer)->m_bmbManager->makeBombEffect(m_spr->getPosition(),1);
			((AppDelegate*)CCApplication::sharedApplication())->playEff(E_ENEMY_DIE);
			if ( m_bFallDie )
				((GameLayer*)getParent())->createScore( m_spr->getPosition(),7000 );
			else
				((GameLayer*)getParent())->createScore( m_spr->getPosition(),9000 );
		}
		else
		{
			((GameLayer*)m_level->cocosLayer)->m_bmbManager->makeBombEffect(m_spr->getPosition(),2);
			((AppDelegate*)CCApplication::sharedApplication())->playEff(E_HAMBO_DIE);
			((GameLayer*)m_level->cocosLayer)->m_bHeroFriendDestroyed = true;
		}
    }else
	{
        ((GameLayer*)getParent())->m_bmbManager->makeBombEffect(m_spr->getPosition(),3);
		((GameLayer*)getParent())->createScore( m_spr->getPosition(),3000 );
		((AppDelegate*)CCApplication::sharedApplication())->playEff(E_ICE_BREAK);
	}
	CCString *strJointName = CCString::createWithFormat("%s_1", m_spr->getUniqueName()->getCString());
    LHJoint *joint = m_level->jointWithUniqueName(strJointName);
    int nIndex = 1;
    while(joint){
        nIndex ++;
        m_level->removeJoint(joint);
		strJointName = CCString::createWithFormat("%s_%d", m_spr->getUniqueName()->getCString(), nIndex);
        joint = m_level->jointWithUniqueName(strJointName);
    }
    
	CCString *strEyeName = CCString::createWithFormat("%s_eye", m_spr->getUniqueName()->getCString());
    m_level->removeSprite(m_level->spriteWithUniqueName(strEyeName));
    
    m_level->removeSprite(m_spr);
    if (m_afraidSprite) m_afraidSprite->setVisible(false);

	if ( m_helpArrow )
	{
		CCFadeIn *fIn = CCFadeIn::create(0.2f);
		m_helpArrow->runAction(fIn);
	}
    
    unscheduleAllSelectors();
    
//    [m_level removeSprite:m_spr];
//    m_spr = NULL;
    
    removeFromParentAndCleanup(true);
}

Enemy::~Enemy() {
    unscheduleAllSelectors();    
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}