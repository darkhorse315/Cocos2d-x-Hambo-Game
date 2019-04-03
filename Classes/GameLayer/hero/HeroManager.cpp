//
//  HeroManager.m
//  OutZone_iphone
//
//  Created by admin on 1/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "HeroManager.h"
#include "Constants.h"
#include "LevelHelper/Nodes/LHSettings.h"
#include "AppDelegate.h"
#include "../GameLayer.h"

#define HERO_HEAD_HEIGHT		48
#define HERO_ARM_DISTANCE		43
#define HERO_WEAPON_DISTANCE	85

void HeroManager::initWithLevelHelper (LevelHelperLoader *hLevel)
{
    m_level = hLevel;
    m_sprHero = (LHSprite*)m_level->spritesWithTag(HERO_TAG)->objectAtIndex(0);
	m_sprHero->setAnchorPoint( ccp(0.5,1) );

	CCFileUtils*	fileUtils = CCFileUtils::sharedFileUtils();
	CCString *str = CCString::createWithFormat("ipad_res/game/hero/hero%d_arm.png", g_nHeroSkinIndex+1);
	m_sprHeroArm = CCSprite::create(fileUtils->fullPathFromRelativePath(str->getCString()));
	str = CCString::createWithFormat("ipad_res/game/hero/hero%d_head.png", g_nHeroSkinIndex+1);
	m_sprHeroHead = CCSprite::create(fileUtils->fullPathFromRelativePath(str->getCString()));
	
	m_level->cocosLayer->addChild(m_sprHeroArm);
	m_level->cocosLayer->addChild(m_sprHeroHead);
	m_sprHeroArm->setScaleY( m_sprHero->getScaleY() );
	m_sprHeroHead->setScaleY( m_sprHero->getScaleY() );
	for ( int i = 0; i < 7; i++ )
	{
		m_sprHeroWeapons[i] = CCSprite::create(fileUtils->fullPathFromRelativePath(CCString::createWithFormat("ipad_res/game/hero/weapon%d.png",i+1)->getCString()));
		m_level->cocosLayer->addChild(m_sprHeroWeapons[i]);
		m_sprHeroWeapons[i]->setAnchorPoint( ccp(-0.7, 0.5) );
		m_sprHeroWeapons[i]->setVisible(false);
		m_sprHeroWeapons[i]->setScaleY( m_sprHero->getScaleY() );
	}
	
// 	m_sprHeroWeaponPistol = CCSprite::create(fileUtils->fullPathFromRelativePath(CCString::createWithFormat("ipad_res/game/hero/pistol.png")->getCString()));
// 	m_sprHeroWeaponUzi = CCSprite::create(fileUtils->fullPathFromRelativePath(CCString::createWithFormat("ipad_res/game/hero/uzi.png")->getCString()));
// 	m_sprHeroWeaponGrenade = CCSprite::create(fileUtils->fullPathFromRelativePath(CCString::createWithFormat("ipad_res/game/hero/grenade.png")->getCString()));

	reverseDirectX();
	reverseDirectX();

	m_sprHeroWeapon = NULL;

	m_BombBullet = NULL;
	
// 	m_level->cocosLayer->addChild(m_sprHeroWeaponPistol);
// 	m_sprHeroWeaponPistol->setAnchorPoint( ccp(-0.7, 0.5) );
// 	m_sprHeroWeaponPistol->setVisible(false);
// 	
// 	m_level->cocosLayer->addChild(m_sprHeroWeaponUzi);
// 	m_sprHeroWeaponUzi->setAnchorPoint( ccp(-0.7, 0.5) );
// 	m_sprHeroWeaponUzi->setVisible(false);
// 
// 	m_level->cocosLayer->addChild(m_sprHeroWeaponGrenade);
// 	m_sprHeroWeaponGrenade->setAnchorPoint( ccp(-0.7, 0.5) );
// 	m_sprHeroWeaponGrenade->setVisible(false);

	replaceWeapon();

	m_sprHeroHead->setAnchorPoint( ccp( 0.5, 0.3 ) );
	m_sprHeroArm->setAnchorPoint( ccp(-0.5, 0.5) );

	m_bDie = false;
	m_bulManager = new BulletManager(hLevel);
    hLevel->cocosLayer->addChild(m_bulManager);
	m_fAngle = getBulletAngleByHeroPos( ccp(SCREEN_WIDTH/2,SCREEN_HEIGHT/2) );
	schedule(schedule_selector(HeroManager::onTime));

// 	m_sprHero->setOpacity( 100 );
// 	m_sprHeroArm->setOpacity(100);
// 	m_sprHeroWeapon->setOpacity( 100 );
// 	m_sprHeroHead->setOpacity( 100 );
}

void HeroManager::onTime(float dt)
{
    processDirectionAnim();
    if(m_sprHero->getPosition().y < -64)
	{
        m_sprHero->setTag(DELETED_TAG);        
    }

    if(m_sprHero->getTag() == DELETED_TAG)
	{
        m_sprHero->setVisible(false);
		m_sprHeroArm->setVisible(false);
		m_sprHeroHead->setVisible(false);

		for ( int i = 0; i < 7; i++ )
			m_sprHeroWeapons[i]->setVisible(false);
		
// 		m_sprHeroWeaponPistol->setVisible(false);
// 		m_sprHeroWeaponUzi->setVisible(false);
// 		m_sprHeroWeaponGrenade->setVisible(false);

		g_nHeroWeaponType = HERO_PISTOL_BULLET;

		((GameLayer*)getParent())->m_bmbManager->makeBombEffect(m_sprHero->getPosition(),2);

		((AppDelegate*)CCApplication::sharedApplication())->playEff(E_HAMBO_DIE);
        unscheduleAllSelectors();
    }
}

void HeroManager::processDirectionAnim()
{
    if(m_sprHero == NULL)
        return;

	int nSkinIndex = -1;
    int nSkinSortIndex = -1;
    float fHeroRotation = m_fAngle;

	CCPoint	pos = m_sprHero->getPosition();
	m_sprHeroArm->setPosition( pos );
	m_sprHeroHead->setPosition( pos );
	m_sprHeroWeapon->setPosition( pos );

	fHeroRotation -= m_sprHero->getRotation();
	if ( m_sprHero->getRotation() > 180 )
		fHeroRotation += 360;
	
    if(fabsf(fHeroRotation) > 90)
	{
		fHeroRotation += 180;
        if(m_sprHero->getScaleX() > 0)
		{
			reverseDirectX();
		}
    }
	else
	{
		if(m_sprHero->getScaleX() < 0)
		{
			reverseDirectX();
		}
	}

	fHeroRotation += m_sprHero->getRotation();
	m_sprHeroArm->setRotation( fHeroRotation );
	m_sprHeroHead->setRotation( fHeroRotation );
	m_sprHeroWeapon->setRotation( fHeroRotation );

}

void HeroManager::reverseDirectX()
{
	float	scaleX = m_sprHero->getScaleX();
	scaleX *= -1;
	m_sprHero->setScaleX( scaleX );
	m_sprHeroArm->setScaleX( scaleX );
	m_sprHeroHead->setScaleX( scaleX );
	for ( int i = 0; i < 7; i++ )
		m_sprHeroWeapons[i]->setScaleX( scaleX );
	
// 	m_sprHeroWeaponPistol->setScaleX( scaleX );
// 	m_sprHeroWeaponGrenade->setScaleX( scaleX );
// 	m_sprHeroWeaponUzi->setScaleX( scaleX );
}

int HeroManager::getHeroTag()
{
    return m_sprHero->getTag();
}

CCPoint HeroManager::getHeroPos() {
    if(m_bDie)
        return ccp(0, 0);
    return m_sprHero->getPosition();
}

void HeroManager::setHeroPos(CCPoint ptPos) {
    if(m_bDie)
        return;
	m_sprHero->getBody()->SetTransform(b2Vec2(ptPos.x / LHSettings::sharedInstance()->lhPtmRatio, ptPos.y / LHSettings::sharedInstance()->lhPtmRatio), m_sprHero->getBody()->GetAngle());
}

float HeroManager::getBulletAngleByHeroPos(CCPoint ptTouchedPos)
{
    float fRadion = acos(abs(ptTouchedPos.x - m_sprHero->getPosition().x) / 
						 sqrt((ptTouchedPos.x - m_sprHero->getPosition().x) * (ptTouchedPos.x - m_sprHero->getPosition().x) + 
                              (ptTouchedPos.y - m_sprHero->getPosition().y) * (ptTouchedPos.y - m_sprHero->getPosition().y)));
	float fAngle = CC_RADIANS_TO_DEGREES(fRadion);
    if(ptTouchedPos.x > m_sprHero->getPosition().x){
        if(ptTouchedPos.y > m_sprHero->getPosition().y)
            fAngle *= -1;
    }else{
        if(ptTouchedPos.y > m_sprHero->getPosition().y)
            fAngle = fAngle - 180;
        else
            fAngle = 180 - fAngle;
    }
    m_fAngle = fAngle;
    return fAngle;
}
#define BULLET_OFFSET	iDevSize(64)
#define STARTPOS_OFFSET	iDevSize(10)
void HeroManager::createBullet(float fAngle,float fForce,CCPoint ptTouchedPos)
{
    if(m_bDie)
        return;

	g_CanKillEnemy = true;

	EFFECT_ID eID = E_SHOOT_UZI;
	switch (g_nHeroWeaponType)
	{
	case HERO_HAND_GRENADE_BULLET:
	case HERO_KNIFE_BULLET:
	case HERO_BOMB_BULLET:
	case HERO_FIRE_ARROW_BULLET:		eID = E_SHOOT_KNIFE;		break;
	case HERO_PISTOL_BULLET:			eID = E_SHOOT_PISTOL;		break;
	case HERO_UZI_BULLET:				eID = E_SHOOT_UZI;			break;
	}
	AppDelegate*	app = (AppDelegate*)CCApplication::sharedApplication();
	app->playEff( eID );
    
    CCPoint ptOffset = ccp(BULLET_OFFSET * cos(CC_DEGREES_TO_RADIANS(fAngle * -1)), BULLET_OFFSET * sin(CC_DEGREES_TO_RADIANS(fAngle * -1)));
	float angle = CC_DEGREES_TO_RADIANS( m_sprHero->getRotation() + 90 );
	CCPoint startPos = ccp(STARTPOS_OFFSET * cos( angle ),STARTPOS_OFFSET * sin(angle));
	startPos = ccpAdd( getHeroPos(),startPos );
	
    if ( g_nHeroWeaponType == HERO_BOMB_BULLET )
		m_BombBullet = m_bulManager->createBullets(g_nHeroWeaponType,fAngle,ccpAdd(startPos, ptOffset),ccp(0,0),fForce);
	else
		m_bulManager->createBullets(g_nHeroWeaponType,fAngle,ccpAdd(startPos, ptOffset),ccp(0,0),fForce);
}

void HeroManager::dieAnimation()
{
    
}

void HeroManager::replaceWeapon()
{
	if ( m_sprHeroWeapon )
		m_sprHeroWeapon->setVisible(false);

	if ( g_nHeroWeaponType == HERO_BOMB_BULLET && m_BombBullet != NULL )
		m_sprHeroWeapon = m_sprHeroWeapons[ g_nHeroWeaponType - HERO_HAND_GRENADE_BULLET + 1 ];
	else
		m_sprHeroWeapon = m_sprHeroWeapons[ g_nHeroWeaponType - HERO_HAND_GRENADE_BULLET ];
	
	m_sprHeroWeapon->setVisible(true);
}

HeroManager::~HeroManager()
{
    removeAllChildrenWithCleanup(true);
    unscheduleAllSelectors();
    stopAllActions();    
}
