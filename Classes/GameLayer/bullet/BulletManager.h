//
//  BulletManager.h
//  OutZone_iphone
//
//  Created by admin on 1/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H


#include "cocos2d.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "Box2D/Dynamics/b2World.h"
#include "Bullet.h"

class BulletManager : public CCNode {
    LevelHelperLoader *m_level;
    CCPoint             m_ptStart;
    CCPoint             m_ptEnd;   
public:
	BulletManager(LevelHelperLoader *hLevel);
	void initBulletType(LevelHelperLoader *hLevel);
	Bullet* createBullets(int nType,float fAngle,CCPoint ptStart,CCPoint ptTarget,float fBulletSpeed);
};

#endif