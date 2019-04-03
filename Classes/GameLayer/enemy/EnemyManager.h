//
//  EnemyManager.h
//  OutZone_iphone
//
//  Created by admin on 2/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "LevelHelper/LevelHelperLoader.h"

class EnemyManager : public CCNode {
    LevelHelperLoader *m_level;
    bool            m_bExist;

public:
	~EnemyManager();
	void initWithLevelHelper(LevelHelperLoader *hLevel);
	void runEnemies();
	void manageEnemies(LHSprite* sprEnemy,int nLife ,bool bIsIce);
	void checkDestoriedAllEnemies();
	bool isDestroyAllEnemies();

};