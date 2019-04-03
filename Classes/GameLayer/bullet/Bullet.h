//
//  Bullet.h
//  OutZone_iphone
//
//  Created by admin on 1/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BULLET_H
#define BULLET_H

#include "cocos2d.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "Box2D/Dynamics/b2World.h"

#define OUT_OFFSETY 96
#define OUT_OFFSETX 85

class Bullet : public CCSprite {
public:
    b2World *m_world;
    b2Body              *m_body;
    b2Vec2               m_vecVel;
    CCString            *m_strTargetName;
    CCPoint             m_ptStartedPos;
    int                 m_nBombTick;

	CCSprite*			m_thunder;
	LevelHelperLoader *m_level;
	bool		bFirstMove;
    int         m_nForce;
    int         m_nType;
    CCPoint     m_ptPos;
	float		m_sprAngle;
	Bullet(int nType,LevelHelperLoader *hLevel,CCPoint ptStartPos,CCPoint ptTargetPos,float fAngle,int nBulletImgType,int nForce,int nBulletTag,CCPoint ptBulletSize ,float fVel);
	~Bullet();
	float getAngle(CCPoint ptStartPos,CCPoint ptTargetPos);
	float absf(float fValue);
	void processRotation();
	void destroy();

	void onMoving(float dt);
	void onHideAction( float dt );

	void collison( LHSprite* sprObj );
	void processThunder();
};

#endif
