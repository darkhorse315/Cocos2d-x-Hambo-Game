//
//  Created by KCU on 6/13/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

// Import the interfaces
#include "LoadScene.h"
#include "MainMenu/TitleLayer.h"
#include "gamelayer/GameLayer.h"
#include "ccframework/ResourceManager.h"

CCScene* LoadScene::scene(WND_ID wndCur, WND_ID wndNew ,int param)
{
	CCScene *scene = CCScene::create();
	CCNode *layer = (CCNode*)new LoadScene(wndCur,wndNew,param);
    //LoadScene *layer = [LoadScene node]; 
	scene->addChild(layer);
    
   	return scene;
}

LoadScene::LoadScene(WND_ID wndCur,WND_ID wndNew,int param)
{
	if( ScaleLayer::init() )
	{
        m_nCurWnd = wndCur;
        m_nNewWnd = wndNew;
        m_nParam = param;
        
        m_tick = 0;
        
		CCLayerColor* backLayer = CCLayerColor::create(ccc4(0,0,0,255),3000,3000);
		backLayer->setAnchorPoint(ccp(0,0));
		addChild(backLayer,-1);

		CCSprite *sprBg = m_resMgr->getSpriteWithName("loading");
        sprBg->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
        addChild(sprBg);
		schedule( schedule_selector(LoadScene::draw),0.05f );
	}
}

LoadScene::~LoadScene()
{	
	unscheduleAllSelectors();
	removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void LoadScene::draw( float dt )
{
	m_tick ++;
	
	if (m_tick > 1)
	{
        if (m_tick == 15) {
            changeWindow();
        }
	}
}
             
void LoadScene::changeWindow()
{
	CCDirector*	director = CCDirector::sharedDirector();
    CCScene*	sce = NULL;
    
    switch ( m_nNewWnd )
	{
        case WND_TITLE:	sce = TitleLayer::scene();	break;
        case WND_GAME:	sce = GameLayer::scene();	break;
        default:		break;
    }
    
    if( sce )
	{
		director->replaceScene(CCTransitionFade::create(1.0f, sce));
    }
}
