//
//  BombEffect.h
//  OutZone_iphone
//
//  Created by admin on 2/10/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
USING_NS_CC;
class BombEffect : public CCSprite {
public:
    bool m_bEnd;

	BombEffect(CCAnimation *animation);
	~BombEffect();
	void onFinishBombEffect();
};