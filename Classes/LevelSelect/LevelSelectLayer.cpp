//
//  LevelSelectLayer.m
//  JarHead
//
//  Created by admin on 6/26/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "LevelSelectLayer.h"
#include "Constants.h"
#include "ccframework/ResourceManager.h"
//#include "LoadScene.h"
#include "AppDelegate.h"
#include "StageSelectLayer.h"
#include "ccframework/GrowButton.h"
#include "MainMenu/TitleLayer.h"
#include "ModalAlert/ModalAlert.h"

USING_NS_CC_EXT;

#define LEVEL_STEP iDevPixelX(668)

CCScene* LevelSelectLayer::scene()
{
	// 'scene' is an autorelease object.
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object.
	LevelSelectLayer *layer = LevelSelectLayer::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

bool LevelSelectLayer::init()
{
	if ( ScaleLayer::init() )
	{
		m_app->playBGM(B_TITLE);

        setTouchEnabled(true);
        
		m_arrayStageSprite = new CCArray();
		g_nCurMission = g_nCurLevel / PER_LEVEL_COUNT;
		if ( g_nCurMission >= STAGE_COUNT )
			g_nCurMission = STAGE_COUNT - 1;
		m_nCurStage = g_nCurMission;

		createBackground();
		createStageInfo();
        createStageButtons();

        adjustScale(0.0f);

		return true;
    }
    
    return false;
}

LevelSelectLayer::~LevelSelectLayer()
{
	CC_SAFE_DELETE(m_arrayStageSprite);

	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	removeAllChildrenWithCleanup(true);    
}

void LevelSelectLayer::createBackground()
{
	CCSprite *sprite = m_resMgr->getSpriteWithName("common_bg1");
	sprite->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	addChild(sprite, -100);

	sprite = m_resMgr->getSpriteWithName("common_sun");
	sprite->setPosition( ccp(iDevPixelX(747), iDevPixelY(700)) );
	addChild(sprite, -100);
	CCRepeatForever *repAct = CCRepeatForever::create( CCRotateBy::create(0.1f, 2.0f) );
	sprite->runAction(repAct);

	sprite = m_resMgr->getSpriteWithName("common_bg2");
	sprite->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	addChild(sprite, -100);

	sprite = m_resMgr->getSpriteWithName("common_bg3");
	sprite->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	addChild(sprite, -100);

	btnBack = GrowButton::buttonWithSpriteFileName("options/back_btn1", "options/back_btn2", this, cccontrol_selector(LevelSelectLayer::onBack));
	btnBack->setPosition(ccp(iDevPixelX(128), iDevPixelY(85)));
	addChild(btnBack);      

	btnClose = GrowButton::buttonWithSpriteFileName("mainmenu/btnClose1","mainmenu/btnClose2",this,cccontrol_selector(LevelSelectLayer::onClose));
	btnClose->setPosition( ccp(iDevPixelX(896), SCREEN_HEIGHT-iDevPixelY(672)) );
	addChild(btnClose);
}

void LevelSelectLayer::onClose( CCObject* sender, CCControlEvent event )
{
	m_app->playEff(E_CLICK);

	CCDirector::sharedDirector()->end();
	//ModalAlert::Confirm("Do you want to quit?", this, menu_selector(LevelSelectLayer::onQuitOK),NULL);
}

void LevelSelectLayer::onQuitOK( CCObject* sender )
{
	CCDirector::sharedDirector()->end();
}

void LevelSelectLayer::createStageInfo()
{
	CCSprite*	sprite;
	sprite = m_resMgr->getSpriteWithName("levelselect/levelinfo");
	sprite->setPosition(ccp(SCREEN_WIDTH/2, iDevPixelY(120)));
	addChild(sprite);

	CCSize	size = sprite->getContentSize();
	m_lblLevel = CCLabelBMFont::create("", "font/MainMenuFont@2x.fnt");
	m_lblLevel->setPosition( ccp(size.width/2, size.height/2) );
	sprite->addChild(m_lblLevel);

	m_lblLevel->setScaleX( iDevPixelX(1.0f) );
	m_lblLevel->setScaleY( iDevPixelY(1.0f) );

	updateInfo();
}

void LevelSelectLayer::createStageButtons()
{
// 	int nAvaiableStage = g_nAvailableLevel[g_nCurMission];
// 	CCAssert ( nAvaiableStage < STAGE_COUNT, __FUNCTION__ "Stage Count is error!" );
// 
// 	if (nAvaiableStage >= STAGE_COUNT)
// 		nAvaiableStage = STAGE_COUNT-1;

	int nStartPos = SCREEN_WIDTH/2 - g_nCurMission*LEVEL_STEP;

	CCString *strLevelMarkName;
	for (int i = 0; i < STAGE_COUNT; i++)
	{
		if (!g_nMissionsUnlocked[i])
			strLevelMarkName = CCString::createWithFormat("levelselect/levelmark%d_lock", i+1);
		else
			strLevelMarkName = CCString::createWithFormat("levelselect/levelmark%d", i+1);

		CCSprite* sprite = m_resMgr->getSpriteWithName(strLevelMarkName->getCString());
		sprite->setPosition(ccp(nStartPos + i * LEVEL_STEP, SCREEN_HEIGHT / 2));
		addChild(sprite, 0, i);

		m_arrayStageSprite->addObject(sprite);
	}
}

void LevelSelectLayer::updateInfo()
{
	CCString* str = CCString::createWithFormat( "Mission %d", m_nCurStage+1 );
	m_lblLevel->setString( str->getCString() );
}

void LevelSelectLayer::onBack( CCObject* sender, CCControlEvent event ) {
	m_app->playEff(E_CLICK);

	CCScene *sce = TitleLayer::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f, sce));
}

void LevelSelectLayer::onStage(int nStageNo)
{
	if (g_nAvailableLevel[nStageNo] < 0)
		return;

	g_nCurMission = nStageNo;

	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f,StageSelectLayer::scene()));
}

void LevelSelectLayer::adjustScale(float offset)
{
// 	for (unsigned int i = 0; i < m_arrayStageSprite->count(); i++)
// 	{
// 		CCSprite* sprite = (CCSprite*)m_arrayStageSprite->objectAtIndex(i);
// 		CCPoint pos = sprite->getPosition();
// 		if (i == 0 && pos.x > SCREEN_WIDTH/2)
// 			return;
// 		if (i == m_arrayStageSprite->count()-1 && pos.x < SCREEN_WIDTH/2)
// 			return;
// 	}

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_arrayStageSprite, pObj)
	{
		CCSprite* sprite = (CCSprite*)pObj;
		float fInitScale = 1.0f;
		if (UI_USER_INTERFACE_IDIOM() != UIUserInterfaceIdiomPad)
		{
			if (CC_CONTENT_SCALE_FACTOR() != 2.0f)
			{
				fInitScale = 0.5f;
			}
		}

		CCPoint pos = sprite->getPosition();
		pos = ccpAdd(pos, ccp(offset, 0));

		float fScale = SCREEN_WIDTH / 2 - pos.x;

		if (fScale < 0) {
			fScale = -fScale;
		}

		if (fScale > 200) {
			fScale = 200;
		}

		float fAddScale = 0.4f * (200.0f - fScale) / 200.0f;
		float scale = (0.6f + fAddScale) * fInitScale;

		//sprite->setScale(scale);
		sprite->setPosition(pos);
	}
}

bool LevelSelectLayer::isTap(CCPoint ptDown, CCPoint ptUp)
{
	float dist = ccpDistance(ptDown, ptUp);

	if ( dist > 10 )
		return false;

	return true;
}

CCSprite* LevelSelectLayer::getTouchedSprite(CCPoint point)
{
	CCObject* pObj = NULL;
	CCSprite* pRet = NULL;
	CCARRAY_FOREACH(m_arrayStageSprite, pObj)
	{
		CCSprite* pSprite = (CCSprite*)pObj;
		if (pSprite->boundingBox().containsPoint(point))
		{
			pRet = pSprite;
			break;
		}
	}

	return pRet;
}

// Find the nearest stage images from screen's point!
CCSprite* LevelSelectLayer::getNearestSprite(CCPoint point)
{
	float	fMin = 10000000000;

	CCSprite* pRet = NULL;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_arrayStageSprite, pObj)
	{
		CCSprite* pSprite = (CCSprite*)pObj;
		CCPoint	delta = ccpSub(point, pSprite->getPosition());
		float dist = abs(delta.x);
		if ( dist < fMin )
		{
			fMin = dist;
			pRet = pSprite;
		}
	}

	return pRet;
}

// Move all stage images!
void LevelSelectLayer::moveAllStageImage(CCPoint delta)
{
	float		offsest = delta.x;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_arrayStageSprite, pObj)
	{
		CCSprite* pSprite = (CCSprite*)pObj;
		CCPoint		newPos = ccpAdd(pSprite->getPosition(), ccp(offsest, 0));
		CCMoveTo*	moveAction = CCMoveTo::create(0.1f, newPos);
//		CCActionInterval* actionEase = CCEaseSineOut::create(moveAction);
		pSprite->runAction(moveAction);
	}
}
void LevelSelectLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool LevelSelectLayer::ccTouchBegan(CCTouch* touch, CCEvent *event)
{
	m_pointTouchDown = touch->getLocation();

	//CCSprite* pSprite = getTouchedSprite(m_pointTouchDown);
	//if (pSprite)
	if ( btnBack->boundingBox().containsPoint( m_pointTouchDown ) )
		return false;

	if ( btnClose->boundingBox().containsPoint( m_pointTouchDown ) )
		return false;

	return true;
}

void LevelSelectLayer::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    CCPoint delta = touch->getDelta();
    adjustScale( delta.x );
}

void LevelSelectLayer::ccTouchEnded(CCTouch * touch, CCEvent *event)
{
	CCPoint location = touch->getLocation();

    bool bTap = isTap(m_pointTouchDown, location);

	CCPoint		ptCenter = ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	CCSprite*	pSprite = NULL;
	if ( bTap == true )
	{
		pSprite = getTouchedSprite(location);
		if (pSprite)
		{
			int nStageNo = pSprite->getTag();

			if (m_nCurStage == nStageNo)
			{
				onStage(nStageNo);
				return;
			}
		}
	}
	else
	{
		pSprite = getNearestSprite(ptCenter);
	}

	if (pSprite)
	{
		CCPoint	delta = ccpSub(ptCenter, pSprite->getPosition());

		moveAllStageImage(delta);
		m_nCurStage = pSprite->getTag();
		updateInfo();
	}
}

