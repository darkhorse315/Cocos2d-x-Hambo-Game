
#include "logo.h"
#include "MainMenu/TitleLayer.h"
#include "ccframework/ResourceManager.h"
#include "Constants.h"

#define LOGO_LAYER_DURATION		0.5f

CCScene* logo::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	logo *layer = logo::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool logo::init()
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super" return value
	
	if( ScaleLayer::init() )
	{
		createBackground();
		schedule(schedule_selector(logo::logoTimer), 2.0f);
	}
	
	return true;
}

logo::~logo()
{
	unscheduleAllSelectors();
	removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();	
}

void logo::createBackground()
{
	CCLayerColor* backLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), 3000, 3000);
	backLayer->setAnchorPoint(ccp(0,0));
	addChild(backLayer,-1);

	CCSprite *sprBg = m_resMgr->getSpriteWithName("loading");
	sprBg->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	addChild(sprBg);
}

void logo::logoTimer(float dt){
	CCScene *sce = TitleLayer::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(LOGO_LAYER_DURATION, sce));
}
