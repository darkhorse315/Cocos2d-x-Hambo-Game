#ifndef SCALELAYER_H
#define SCALELAYER_H

#include "cocos2d.h"
#include "Constants.h"

USING_NS_CC;

class AppDelegate;
class ResourceManager;

class ScaleLayer : public CCLayer {
public:
	virtual ~ScaleLayer();
	virtual bool init(); 

// 	void addChild(CCNode* child,int z = 0,int aTag = 0,bool bScale = true);
// 	void processCommonScale(CCNode* child);

protected:
	AppDelegate*		m_app;
	ResourceManager*	m_resMgr;
};

#endif