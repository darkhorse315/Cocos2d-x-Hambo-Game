//
//  StageMoveLayer.m
//  JarHead
//
//  Created by admin on 6/26/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "StageMoveLayer.h"
#include "Constants.h"
#include "ccframework/ResourceManager.h"
//#include "LoadScene.h"
//#include "AppSettings.h"
#include "ccframework/GrowButton.h"
//#include "CCLabelFX.h"
#include "LevelSelect/LevelSelectLayer.h"
#include "AppDelegate.h"

#define STAGE_STEP iDevPixelX(100)

CCScene* StageMoveLayer::scene()
{
	// 'scene' is an autorelease object.
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object.
	StageMoveLayer *layer = StageMoveLayer::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

bool StageMoveLayer::init()
{
	if ( ScaleLayer::init() ) {
        setTouchEnabled(true);
        
        fWidth = 0;
		GrowButton *btnBack = GrowButton::buttonWithSpriteFileName("levelselect/btnBack1","levelselect/btnBack2",this,menu_selector(StageMoveLayer::onBack));
        btnBack->setPosition(ccp(iDevPixelX(60), iDevPixelX(40)));
        addChild(btnBack);
        
        createStage();
    }
	return true;
}

void StageMoveLayer::onBack( CCObject* sender ) {
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.7,LevelSelectLayer::scene()));
}

StageMoveLayer::~StageMoveLayer()
{
    removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void StageMoveLayer::createStage()
{
	ResourceManager *res = ResourceManager::sharedResourceManager();
    CCSprite* spriteBackground = res->getSpriteWithName("levelselect/levelselect_bg");
    spriteBackground->setAnchorPoint(ccp(0,1));
    spriteBackground->setPosition(ccp(0,SCREEN_HEIGHT));
    addChild(spriteBackground,-100);
    
//    CCSprite* sprBG = [CCSprite spriteWithFile:@"stage_bg.png"];
//    sprBG.position = ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);    
//    [self addChild:sprBG z:-100];
    
	stageLayer = (ScaleLayer*)ScaleLayer::create();    
    
    int nStage = g_nAvailableLevel;
    
    for (int i = 0; i < 42; i++) {
		GrowButton* btn = GrowButton::buttonWithSpriteFileName(i<=nStage ? "levelselect/stage_mark_d" : "levelselect/stage_mark_lock",
																i<=nStage ? "levelselect/stage_mark_f" : "levelselect/stage_mark_lock",
																this,
																menu_selector(StageMoveLayer::onStage),
																i);
        
        int nhexagon = i % 3;

        float x = STAGE_STEP;
        float y = STAGE_STEP;
        switch (nhexagon) {
            case 0:
                x = x * cosf(45 * M_PI / 180);
                y = 0;
                fWidth += x;
                break;
            case 1:
                x = x * cosf(45 * M_PI / 180);
                y = y * sinf(45 * M_PI / 180);
                fWidth += x;
                break;
            case 2:
                x = x * cosf(45 * M_PI / 180);
                y = -y * sinf(45 * M_PI / 180);
                break;
        }
        
        btn->setPosition(ccp(fWidth, SCREEN_HEIGHT / 2 + y));
        stageLayer->addChild(btn);

		CCLabelAtlas* label = CCLabelAtlas::labelWithString("00","iphone_res/levelselect/number.png",40,49,'0');
		label->setString(CCString::stringWithFormat("%d", i+1)->getCString());
        label->setAnchorPoint(ccp(0.5f, 0.6f));
        label->setPosition(ccp(fWidth, SCREEN_HEIGHT / 2 + y));
        stageLayer->addChild(label);
    }
    fWidth -= SCREEN_WIDTH - (cosf(60 * M_PI / 180) * STAGE_STEP);
	stageLayer->setTouchEnabled(false);
    addChild(stageLayer);
}

void StageMoveLayer::onStage( CCObject* sender )
{
    GrowButton* button = (GrowButton*)sender;
    int nStage = button->getTag();
    
    if (nStage > g_nAvailableLevel)
        return;

    g_nCurLevel = nStage;
    
	((AppDelegate*)CCApplication::sharedApplication())->changeWindow(WND_GAME);
}

// - (void) registerWithTouchDispatcher
// {
//     [[CCTouchDispatcher sharedDispatcher] addTargetedDelegate:self priority:0 swallowsTouches:true];
// }

bool StageMoveLayer::ccTouchBegan(CCTouch* touch, CCEvent *event)
{
    return true;
}

void StageMoveLayer::ccTouchMoved(CCTouch *touch,CCEvent *event)
{
    CCPoint location = touch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
    
    CCPoint prevLocation = touch->previousLocationInView();
	prevLocation = CCDirector::sharedDirector()->convertToGL(prevLocation);
    
    float offsetX = location.x - prevLocation.x;
    
    CCPoint pos = stageLayer->getPosition();
    
    pos = ccpAdd(pos, ccp(offsetX, 0));
    
    if (pos.x > 0) {
        pos.x = 0;
    } else if (pos.x < -fWidth) {
        pos.x = -fWidth;
    }
    
    stageLayer->setPosition(pos);
}

