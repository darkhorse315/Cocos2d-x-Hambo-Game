
#ifndef _MYLISTENER_H_
#define _MYLISTENER_H_

#include "cocos2d.h"
#include <Box2D/Box2D.h>
//#include "GLES-Render.h"

const int32 k_maxContactPoints = 2048;

class MyListener : public b2ContactListener
{
public:
	MyListener();
	virtual ~MyListener();
	
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(impulse);
	}
	void processSubLife();
	void processBomb(cocos2d::CCPoint pos);
protected:
	void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};

class MyDestructionListener : public b2DestructionListener {
public:
	void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
	void SayGoodbye(b2Joint* joint);
};

#endif // _MYLISTENER_H_
