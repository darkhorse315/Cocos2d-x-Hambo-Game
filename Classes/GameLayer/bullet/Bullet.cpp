//
//  Bullet.m
//  OutZone_iphone
//
//  Created by admin on 1/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Bullet.h"
#include "AppDelegate.h"
#include "Constants.h"
#include "LevelHelper/Nodes/LHSettings.h"
#include "ccframework/ResourceManager.h"
#include "gamelayer/GameLayer.h"

Bullet::Bullet(int nType,LevelHelperLoader *hLevel,CCPoint ptStartPos,CCPoint ptTargetPos,float fAngle,int nBulletImgType,int nForce,int nBulletTag,CCPoint ptBulletSize ,float fVel)
{   
	if ( !CCSprite::init() )
		return;
	
    g_arrBulletsCount ++;
    
    m_level = hLevel;
    m_world = hLevel->box2dWorld;
    m_nForce = nForce;
	bFirstMove = true;
    
	ResourceManager*	resMgr = ResourceManager::sharedResourceManager();
	const char *bulletPath = resMgr->makeFileName(CCString::createWithFormat("game/bullets/bullet%d", nBulletImgType)->getCString(),"png");
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(resMgr->makeFileName(CCString::createWithFormat("game/bullets/bullet%d", nBulletImgType)->getCString(),"png"));
    setTexture(texture);
    setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));	
	m_ptStartedPos = ptStartPos;
	setPosition( m_ptStartedPos );
    //setAnchorPoint(ccp(0, 0.5));
    setTag(nBulletTag);
  
    if(fAngle == UNDEFINED_VALUE){
        fAngle = getAngle(ptStartPos,ptTargetPos);
    }
    setRotation(fAngle);
    m_nType = nType;
	m_thunder = NULL;
	m_sprAngle = fAngle;
	if ( nType == HERO_PISTOL_BULLET || nType == HERO_UZI_BULLET )
	{
		bulletPath = resMgr->makeFileName("game/bullets/thunder","png");
		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage( bulletPath );
		m_thunder = CCSprite::createWithTexture( texture );
		m_thunder->setAnchorPoint(ccp(1,-1));
		m_thunder->setScaleX(0.01f);
		addChild(m_thunder);
	}
    m_ptPos = ptTargetPos;
    
	b2BodyDef bd;
	bd.position.Set(getPosition().x / LHSettings::sharedInstance()->lhPtmRatio, getPosition().y / LHSettings::sharedInstance()->lhPtmRatio);
    bd.type = b2_dynamicBody;
	bd.userData = this;
	bd.angle = -CC_DEGREES_TO_RADIANS(fAngle);
	m_body = m_world->CreateBody(&bd);

    b2Vec2 verts[4];
    
    verts[0] = b2Vec2(-ptBulletSize.x / LHSettings::sharedInstance()->lhPtmRatio, -ptBulletSize.y / LHSettings::sharedInstance()->lhPtmRatio);
    verts[1] = b2Vec2(ptBulletSize.x / LHSettings::sharedInstance()->lhPtmRatio, -ptBulletSize.y / LHSettings::sharedInstance()->lhPtmRatio);
    verts[2] = b2Vec2(ptBulletSize.x / LHSettings::sharedInstance()->lhPtmRatio, ptBulletSize.y / LHSettings::sharedInstance()->lhPtmRatio);
    verts[3] = b2Vec2(-ptBulletSize.x / LHSettings::sharedInstance()->lhPtmRatio, ptBulletSize.y / LHSettings::sharedInstance()->lhPtmRatio);
    
	b2PolygonShape shape;
    shape.Set(verts, 4);
    
	b2FixtureDef fixture;
    fixture.shape = &shape;
	fixture.density = 0.001f;
	
	if ( g_nCurMission >= 4 )
		fixture.density = 0.7f;

	fixture.restitution = 0.2f;
    m_body->CreateFixture(&fixture);
    
    m_vecVel = b2Vec2(fVel * cos(CC_DEGREES_TO_RADIANS(fAngle)), fVel * sin(CC_DEGREES_TO_RADIANS(fAngle)));
    if(fAngle < 0)
        m_vecVel.y = fabsf(m_vecVel.y);
    else
        m_vecVel.y = -fabsf(m_vecVel.y);
    m_body->SetLinearVelocity(m_vecVel);

	if ( m_nType != HERO_PISTOL_BULLET && m_nType != HERO_UZI_BULLET && m_nType != HERO_FIRE_ARROW_BULLET )
	{
		if ( abs(fAngle) > 90 )
			m_body->SetAngularVelocity(15);
		else
			m_body->SetAngularVelocity(-15);

		m_nBombTick = 0;
	}
    
	schedule(schedule_selector(Bullet::onMoving),1.0f / 30.0f);
	schedule(schedule_selector(Bullet::onHideAction),3.0f);
}

void Bullet::onHideAction( float dt ){
    unschedule(schedule_selector(Bullet::onHideAction));
	if ( m_nType == HERO_PISTOL_BULLET && m_nType == HERO_UZI_BULLET )
		setTag(DELETED_TAG);
	if ( m_nType == HERO_HAND_GRENADE_BULLET )
		((LHSprite*)this)->bomb(m_level);
}

void Bullet::collison( LHSprite* sprObj )
{
	if ( m_thunder )
	{
		m_thunder->removeFromParentAndCleanup(true);
		m_thunder = NULL;
		AppDelegate* app = (AppDelegate*)CCApplication::sharedApplication();
		//if ( sprObj->getTag() == DEFAULT_TAG && !isBarrel(sprObj) )
			app->playEff(E_BULLET_COLLISION);
	}
}

float Bullet::getAngle(CCPoint ptStartPos,CCPoint ptTargetPos){
    float fRadion = acos((ptTargetPos.x - ptStartPos.x) / 
						 sqrt((ptTargetPos.x - ptStartPos.x) * (ptTargetPos.x - ptStartPos.x) + 
							  (ptTargetPos.y - ptStartPos.y) * (ptTargetPos.y - ptStartPos.y)));
    float fAngle = CC_RADIANS_TO_DEGREES(fRadion);
	if(ptStartPos.y < ptTargetPos.y){
		fAngle = 360.0f - fAngle;
	}
    return fAngle;
}

void Bullet::onMoving(float dt) {
    if(g_bGamePause)
        return;

	if ( bFirstMove )
		bFirstMove = false;
	
    if(getTag() == DELETED_TAG){
        destroy();
        return;
    }
    
    //processBomb();
	if ( m_nType == HERO_FIRE_ARROW_BULLET )
		processRotation();

	processThunder();

	b2Vec2 vecPos = m_body->GetPosition();
    
    if((vecPos.y * LHSettings::sharedInstance()->lhPtmRatio > SCREEN_HEIGHT + OUT_OFFSETY) ||
		vecPos.x * LHSettings::sharedInstance()->lhPtmRatio < -OUT_OFFSETX ||
		vecPos.y * LHSettings::sharedInstance()->lhPtmRatio < -OUT_OFFSETY){
        setTag(DELETED_TAG);
        return;
    }
}


void Bullet::processRotation()
{
    float nTmpX = m_ptStartedPos.x - getPosition().x;
    float nTmpY = m_ptStartedPos.y - getPosition().y;
    
    float fRadion = acos(absf(nTmpX) / sqrt(nTmpX * nTmpX + nTmpY * nTmpY));
    
    float fAngle = CC_RADIANS_TO_DEGREES(fRadion);
	
    if(nTmpX < 0 && nTmpY < 0){
        fAngle = 180 - fAngle;
    }else if(nTmpX > 0 && nTmpY < 0){
    }else if(nTmpX < 0 && nTmpY > 0){
        fAngle = 180 + fAngle;
    }else if(nTmpX > 0 && nTmpY > 0){
        fAngle = 360 - fAngle;
    }
    fAngle = 180 - fAngle;
    m_body->SetTransform(m_body->GetPosition(), CC_DEGREES_TO_RADIANS(fAngle));
}

float Bullet::absf(float fValue)
{
	if(fValue < 0)
		return -fValue;
	return fValue;
}

void Bullet::processThunder()
{
	if ( m_thunder )
	{
		float dis = ccpDistance( getPosition(),m_ptStartedPos );
		float sc = dis / m_thunder->getContentSize().width;
		sc = sc > 3 ? 3 : sc;
		m_thunder->setScaleX(sc);

		if ( absf(getRotation() - m_sprAngle) >= 0.1 )
			m_thunder->setVisible(false);
	}
}

void Bullet::destroy() {

	//Tutorial Insert
	int *sprInd = (int*)getUserData();
	if ( sprInd )
		((GameLayer*)m_level->cocosLayer)->m_TutotialWrite.addTutorial( ((GameLayer*)m_level->cocosLayer)->m_nTick,BODYDELETE,CCPointZero,CCPointZero,*sprInd );
	//////////////////////////////////////////////////////////////////////////

    g_arrBulletsCount --;
    stopAllActions();
    unscheduleAllSelectors();
    removeFromParentAndCleanup(true);
	m_world->DestroyBody(m_body);
}

Bullet::~Bullet()
{    
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}