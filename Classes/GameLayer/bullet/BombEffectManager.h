//
//  BombEffectManager.h
//  OutZone_iphone
//
//  Created by admin on 2/10/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BOMBEFFECTMANAGER_H
#define BOMBEFFECTMANAGER_H

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"

USING_NS_CC;

class BombEffectManager : public ScaleLayer {
    CCArray *m_arrBullets;	
public:
	BombEffectManager();
	~BombEffectManager();
	void onTime(float t);
	void moveBulletsEffect(CCPoint ptOffset);
	void makeBombEffect(CCPoint ptPos,int nType);
};

#endif
