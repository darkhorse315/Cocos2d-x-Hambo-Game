//
//  Enemy.h
//  OutZone_iphone
//
//  Created by admin on 2/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "gamelayer/bullet/BulletManager.h"

class Enemy : public CCNode {
    LevelHelperLoader *m_level;
    BulletManager       *m_bulManager;
    int                 m_nType;
    int                 m_nLife;
    int                 m_nInitLife;
    bool                m_bIsIce;
	bool				m_bDestroyed;
	bool				m_bFallDie;

	CCActionInterval *	m_AfraidAnimation;
	CCActionInterval *	m_IdleAnimation;

	bool				bAfraid;
	CCSprite*			m_afraidSprite;
public :
    LHSprite    *m_spr;
	CCSprite*			m_helpArrow;

	~Enemy();
	void initInfo(LevelHelperLoader* hLevel,LHSprite* sprEnemy,int nLife ,bool bIsIce);
	void hideEnemy();
	void processIce();
	void checkingSubLife();
	void checkDestroy();
	void enemyCrashedAnim();
	void enemyAfraidAnim();
	void makeEatItem();
	void destroy();
	void completeAnimation();

	void onTime(float t);
	void onTimeForIdle(float t);
};