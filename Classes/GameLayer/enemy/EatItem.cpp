//
//  EatItem.m
//  Chicken
//
//  Created by admin on 5/29/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "EatItem.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "LevelHelper/nodes/LHSettings.h"
#include "Constants.h"

EatItem::EatItem() {
	CCSprite::init();
}

void EatItem::initInfo(CCPoint ptPos,b2World* world,int nIndex)
{
	CCString *strItemImageName = CCString::createWithFormat("ipad_res/game/enemy_die_effect/item%d.png", nIndex + 1);
   
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(strItemImageName->getCString());
    setTexture(texture);
    setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));        
    setAnchorPoint(ccp((rand() % 10) * 0.1f, 0.5));        
	schedule(schedule_selector(EatItem::onTime),1.0f / 30.0f);
    
    m_world = world;
    setPosition(ptPos);
	setTag( 200000 );
    b2BodyDef bd;
    bd.position.Set(getPosition().x / LHSettings::sharedInstance()->lhPtmRatio, getPosition().y / LHSettings::sharedInstance()->lhPtmRatio);
    bd.type = b2_dynamicBody;
    bd.userData = this;
//    bd.angle = -CC_DEGREES_TO_RADIANS(fAngle);
    m_body = world->CreateBody(&bd);
    
    
    b2Vec2 verts[4];
    CCSize selfSize = CCSizeMake(6, 11);
    verts[0] = b2Vec2(-selfSize.width / LHSettings::sharedInstance()->lhPtmRatio, -selfSize.height / LHSettings::sharedInstance()->lhPtmRatio);
    verts[1] = b2Vec2(selfSize.width / LHSettings::sharedInstance()->lhPtmRatio, -selfSize.height / LHSettings::sharedInstance()->lhPtmRatio);
    verts[2] = b2Vec2(selfSize.width / LHSettings::sharedInstance()->lhPtmRatio, selfSize.height / LHSettings::sharedInstance()->lhPtmRatio);
    verts[3] = b2Vec2(-selfSize.width / LHSettings::sharedInstance()->lhPtmRatio, selfSize.height / LHSettings::sharedInstance()->lhPtmRatio);
    
    b2PolygonShape shape;
    shape.Set(verts, 4);
    
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.isSensor = true;
    m_body->CreateFixture(&fixture);
    
    m_body->SetAngularVelocity(rand() % 10);
    int nSymbol = (rand() % 2)==0?-1:1;
    m_body->SetLinearVelocity(b2Vec2((rand() % 10 ) * 0.2f * nSymbol, (rand() % 14) * 0.2f));
}

void EatItem::onTime(float dt) {
    if(getPosition().y < -64)
        setTag(DELETED_TAG);
    if(getTag() == DELETED_TAG){
        destroy();
    }    
}

void EatItem::destroy()
{
    m_world->DestroyBody(m_body);
    removeFromParentAndCleanup(true);
}