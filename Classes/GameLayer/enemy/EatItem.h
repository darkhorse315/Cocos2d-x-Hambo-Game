//
//  EatItem.h
//  Chicken
//
//  Created by admin on 5/29/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "Box2D/Dynamics/b2World.h"

USING_NS_CC;

class EatItem : public CCSprite {
    b2World *m_world;
    b2Body *m_body;
public:
	EatItem();
	void onTime(float dt);
	void initInfo(CCPoint ptPos,b2World* world,int nIndex);
	void destroy();
};