

#include "ScaleLayer.h"
#include "AppDelegate.h"
#include "ccframework/ResourceManager.h"

bool ScaleLayer::init()
{
	if( CCLayer::init() )
	{
		m_app = (AppDelegate*)CCApplication::sharedApplication();
		m_resMgr = ResourceManager::sharedResourceManager();
	}
	return true;
}

ScaleLayer::~ScaleLayer()
{
	CCDirector::sharedDirector()->purgeCachedData();
}

// void ScaleLayer::addChild(CCNode* child,int z,int aTag,bool bScale)
// {
// 	CCLayer::addChild(child,z,aTag);
// 	if ( bScale )
// 		processCommonScale(child);
// }
// 
// void ScaleLayer::processCommonScale(CCNode* child) {
// 
// //     if(UI_USER_INTERFACE_IDIOM() != UIUserInterfaceIdiomPad){
// //         if (CC_CONTENT_SCALE_FACTOR() != 2.0f) {
// //             child.scaleX *= 0.5f;
// //             child.scaleY *= 0.5f;
// //         }
// //     }
// 
// 	child->setScaleX( kScaleX );
// 	child->setScaleY( kScaleY );
// }
