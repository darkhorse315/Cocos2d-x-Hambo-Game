

#ifndef BOMBLHSPRITE_H
#define BOMBLHSPRITE_H


#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "LevelHelper/Nodes/LHSprite.h"

USING_NS_CC;

class LHPathNode;
class LHAnimationNode;
class LevelHelperLoader;


class BombLHSprite : public LHSprite
{
public:
	int afraidState;

public:
	BombLHSprite();

	void bomb(LevelHelperLoader* level);
	void proccessBomb(float dt);
};

#endif