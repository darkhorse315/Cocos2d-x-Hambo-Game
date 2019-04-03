//
//  HeroManager.h
//  OutZone_iphone
//
//  Created by admin on 1/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef HEROMANAGER_H
#define HEROMANAGER_H


#include "cocos2d.h"
#include "LevelHelper/Nodes/LHSprite.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "gamelayer/bullet/BulletManager.h"

class HeroManager :public  CCNode
{
private:
    LHSprite *m_sprHero;
	CCSprite *m_sprHeroArm;
	CCSprite *m_sprHeroHead;
	CCSprite *m_sprHeroWeapon;

	CCSprite *m_sprHeroWeapons[7];


    LevelHelperLoader *m_level;
    BulletManager       *m_bulManager;
	int m_nWeaponType;
    
public:
    bool    m_bDie;
    float m_fAngle;
	Bullet* m_BombBullet;

public:
	~HeroManager();
	void initWithLevelHelper (LevelHelperLoader *hLevel);
	void createBullet(float fAngle,float fForce,CCPoint ptTouchedPos);
	void replaceWeapon();
	void dieAnimation();
	float getBulletAngleByHeroPos(CCPoint ptTouchedPos);
	CCPoint getHeroPos();
	int getHeroTag();

private:
	void reverseDirectX();

	void setHeroPos(CCPoint ptPos);
	void processDirectionAnim();
	void onTime(float dt);
};

#endif