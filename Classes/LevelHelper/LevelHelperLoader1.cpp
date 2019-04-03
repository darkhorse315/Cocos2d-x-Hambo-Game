//  http://www.levelhelper.org
//
//  Created by Bogdan Vladu
//  Copyright 2011 Bogdan Vladu. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//  The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//  Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//  This notice may not be removed or altered from any source distribution.
//  By "software" the author refers to this code file and not the application 
//  that was used to generate this file.
//  You do not have permission to use this code or any part of it if you don't
//  own a license to LevelHelper application.

#include "LevelHelperLoader.h"
#include "Constants.h"
#include "nodes/LHSettings.h"

#include "nodes/LHSprite.h"
#include "nodes/LHAnimationNode.h"
#include "nodes/LHJoint.h"
#include "nodes/LHBatch.h"
#include "nodes/LHPathNode.h"
#include "nodes/LHBezierNode.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

CCString* LHStringWithRect(CCRect rect)
{
	return CCString::stringWithFormat("{{%f, %f}, {%f, %f}}",rect.getMinX(),rect.getMinY(),rect.getMaxX(),rect.getMaxY());
}
CCString* LHStringWithCCPoint(CCPoint pt)
{
	return CCString::stringWithFormat("{%f, %f}",pt.x,pt.y);
}


void LevelHelperLoader::initObjects()
{
	batchNodesInLevel = new CCDictionary();	
	spritesInLevel = new CCDictionary();
    jointsInLevel = new CCDictionary();
	beziersInLevel = new CCDictionary();
    parallaxesInLevel = new CCDictionary();
    animationsInLevel = new CCDictionary();
    physicBoundariesInLevel = new CCDictionary();
    
	addSpritesToLayerWasUsed = false;
	addObjectsToWordWasUsed = false;
    
	LHSettings::sharedInstance()->lhPtmRatio = 32.0f;
	
    notifOnLoopForeverAnim = false;
    
    
	wb = new CCDictionary();
	CCString *strVal;
	strVal = CCString::create("{{0,0}{480,320}}");wb->setObject(strVal,"WBRect");
	strVal = CCString::create("100");wb->setObject(strVal,"TagLeft");
	strVal = CCString::create("100");wb->setObject(strVal,"TagRight");
	strVal = CCString::create("100");wb->setObject(strVal,"TagTop");
	strVal = CCString::create("100");wb->setObject(strVal,"TagBottom");
	strVal = CCString::create("false");wb->setObject(strVal,"CanSleep");
}
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::initWithContentOfFile(CCString* levelFile)
{
	CCAssert(NULL!=levelFile, "Invalid file given to LevelHelperLoader");
	
	initObjects();
	loadLevelHelperSceneFile(levelFile,CCString::create(""),CCString::create(""));	
}
//////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::initWithContentOfFileFromInternet(CCString* webAddress)
{
	CCAssert(NULL!=webAddress, "Invalid file given to LevelHelperLoader");
	
	initObjects();
	//loadLevelHelperSceneFileFromWebAddress(webAddress);	
}
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::initWithContentOfFile(CCString* levelFile,CCString* levelFolder)
{
	CCAssert(NULL!=levelFile, "Invalid file given to LevelHelperLoader");
	
	initObjects();
	
	loadLevelHelperSceneFile(levelFile,levelFolder,CCString::create(""));
}
// ////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::initWithContentOfDictionary(CCDictionary* levelDictionary,CCString* imgFolder)
{
	CCAssert(NULL!=levelDictionary, "Invalid dictionary given to LevelHelperLoader");
	
	initObjects();
	
	loadLevelHelperSceneFromDictionary(levelDictionary,imgFolder);	
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::addSpritesToLayer(CCLayer* _cocosLayer)
{	
	CCAssert(addObjectsToWordWasUsed!=true, "You can't use method addSpritesToLayer because you already used addObjectToWorld. Only one of the two can be used."); 
	CCAssert(addSpritesToLayerWasUsed!=true, "You can't use method addSpritesToLayer again. You can only use it once. Create a new LevelHelperLoader object if you want to load the level again."); 
	
	addSpritesToLayerWasUsed = true;
	
	cocosLayer = _cocosLayer;
	
	addBatchNodesToLayer(cocosLayer);
	
    createAllAnimationsInfo();
    
    //we need to first create the path so we can assign the path to sprite on creation
    //    for(CCDictionary* bezierDict in lhBeziers)
    //    {
    //        //CCString* uniqueName = [bezierDict->objectForKey("UniqueName");
    //        if([[bezierDict->objectForKey("IsPath"] boolValue])
    //        {
    //            createBezierPath:bezierDict);
    //        }
    //    }
    
    CCObject *obj;
	CCARRAY_FOREACH(lhSprites,obj)
	{
		CCDictionary* dictionary = (CCDictionary*)obj;
		CCDictionary* spriteProp = (CCDictionary*)dictionary->objectForKey("GeneralProperties");
		
		//find the coresponding batch node for this sprite
        LHBatch* bNode = (LHBatch*)batchNodesInLevel->objectForKey(spriteProp->valueForKey("Image")->getCString());
		CCSpriteBatchNode *batch = bNode->spriteBatchNode();
		
		if(NULL != batch)
		{
			LHSprite* ccsprite = spriteWithBatchFromDictionary(spriteProp,bNode);
			if(NULL != ccsprite)
			{
				batch->addChild(ccsprite);
				spritesInLevel->setObject(ccsprite,spriteProp->valueForKey("UniqueName")->getCString());
                
                setCustomAttributesForNonPhysics(spriteProp,ccsprite);
			}
            
            if(!spriteProp->valueForKey("PathName")->m_sString.compare("None"))
            {
                //we have a path we need to follow
                createPathOnSprite(ccsprite,spriteProp);
            }
            
			createAnimationFromDictionary(spriteProp,ccsprite);
		}
	}
    
//     for(CCDictionary* parallaxDict in lhParallax)
//     {
        //CCDictionary* nodeInfo = [CCDictionary::create() autorelease);
        //       CCNode* node = parallaxNodeFromDictionary:parallaxDict layer:cocosLayer);
        
        //   if(NULL != node)
        // {
        //[nodeInfo setObject:[parallaxDict->objectForKey("ContinuousScrolling"] forKey:"ContinuousScrolling");
        //[//nodeInfo setObject:[parallaxDict->objectForKey("Speed"] forKey:"Speed");
        //[nodeInfo setObject:[parallaxDict->objectForKey("Direction"] forKey:"Direction");
        //[nodeInfo setObject:node forKey:"Node");
        //         [ccParallaxInScene setObject:node forKey:[parallaxDict->objectForKey("UniqueName"]);
        //}
//    }
}

float LevelHelperLoader::convertAngleByScreen( float angle )
{
	float dx = 1 / 480.0 * SCREEN_WIDTH;
	float dy = tanf(angle) / 320.0 * SCREEN_HEIGHT;
	float newAngle = atan( dy / dx );

	int angleDegree = int(angle*180/M_PI + 720) % 360;
	int newAngleDegree  = int(newAngle*180/M_PI + 720) % 360;

	if (abs(angleDegree-newAngleDegree) > 90 )
		newAngle = newAngle+M_PI;

	return newAngle;
}

////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::addObjectsToWorld(b2World* world ,CCLayer* _cocosLayer)
{
	CCAssert(addSpritesToLayerWasUsed!=true, "You can't use method addObjectsToWorld because you already used addSpritesToLayer. Only one of the two can be used."); 
	CCAssert(addObjectsToWordWasUsed!=true, "You can't use method addObjectsToWorld again. You can only use it once. Create a new LevelHelperLoader object if you want to load the level again."); 
	
	addObjectsToWordWasUsed = true;
	
	cocosLayer = _cocosLayer;
    box2dWorld = world;
	
    //order is important
	addBatchNodesToLayer(cocosLayer);
    createAllAnimationsInfo();    
    createAllBeziers();
    createSpritesWithPhysics();
    createParallaxes();
	createJoints();
	
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CCPoint LevelHelperLoader::gameScreenSize()
{
    return safeFrame;
}

//------------------------------------------------------------------------------
CCRect LevelHelperLoader::gameWorldSize()
{
	CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio;
	
    CCRect ws = gameWorldRect;
    
    ws.origin.x *= wbConv.x;
    ws.origin.y *= wbConv.y;
    ws.size.width *= wbConv.x;
    ws.size.height *= wbConv.y;
    
    return ws;
}
//------------------------------------------------------------------------------
unsigned int LevelHelperLoader::numberOfBatchNodesUsed()
{
	return (int)batchNodesInLevel->count() -1;
}



////////////////////////////////////////////////////////////////////////////////

LevelHelperLoader::~LevelHelperLoader()
{
	releasePhysicBoundaries();
	removeAllBezierNodes();	
	releaseAllParallaxes();
    releaseAllJoints();	
    releaseAllSprites();
    releaseAllBatchNodes();
    
	lhSprites->release();
	lhJoints->release();
    lhParallax->release();
    lhBeziers->release();    
}
////////////////////////////////////////////////////////////////////////////////
///////////////////////////PRIVATE METHODS//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::addBatchNodesToLayer(CCLayer* _cocosLayer)
{
	CCArray *keys = batchNodesInLevel->allKeys();
	int tag = 0;
	CCObject* obj;
	CCARRAY_FOREACH(keys,obj){
		CCString* key = (CCString*)obj;
		LHBatch* info = (LHBatch*)batchNodesInLevel->objectForKey(key->getCString());
		_cocosLayer->addChild(info->spriteBatchNode(),info->z,tag);
		tag++;
	}
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::releaseAllBatchNodes() {
	batchNodesInLevel->removeAllObjects();
    batchNodesInLevel->release();
    batchNodesInLevel = NULL;
}
// ////////////////////////////////////////////////////////////////////////////////
// //ANIMATIONS
// ////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::createAllAnimationsInfo()
{
	CCObject* obj;
    CCARRAY_FOREACH(lhAnims, obj)
    {
		CCDictionary* animInfo = (CCDictionary*)obj;
        CCString* uniqueAnimName = (CCString*)animInfo->objectForKey("UniqueName");
        
        CCArray* framesInfo = (CCArray*)animInfo->objectForKey("Frames");
        
        bool loop           = ((CCString*)animInfo->objectForKey("LoopForever"))->boolValue();
        float animSpeed     = ((CCString*)animInfo->objectForKey("Speed"))->floatValue();
        int repetitions     = ((CCString*)animInfo->objectForKey("Repetitions"))->intValue();
        bool startAtLaunch  = ((CCString*)animInfo->objectForKey("StartAtLaunch"))->boolValue();
        
        CCString* image = (CCString*)animInfo->objectForKey("Image");
		CCString* newImage = LHSettings::sharedInstance()->imagePath(image,false);

		if ( g_nHeroSkinIndex == 0 && ( newImage->m_sString == "hero2_anim-hd.png" || newImage->m_sString == "hero3_anim-hd.png" ) ) continue;
		if ( g_nHeroSkinIndex == 1 && ( newImage->m_sString == "hero_anim-hd.png" || newImage->m_sString == "hero3_anim-hd.png" ) ) continue;
		if ( g_nHeroSkinIndex == 2 && ( newImage->m_sString == "hero_anim-hd.png" || newImage->m_sString == "hero2_anim-hd.png" ) ) continue;

        LHBatch* bNode = (LHBatch*)batchNodesInLevel->objectForKey(newImage->getCString());
		CCSpriteBatchNode *batch = bNode->spriteBatchNode();
        
		CCString* imgRetina = LHSettings::sharedInstance()->imagePath(image);
        
		CCArray *ccframes = CCArray::array();        
        CCARRAY_FOREACH(framesInfo,obj)
        {
			CCDictionary* frm = (CCDictionary*)obj;
            CCRect rect = LHRectFromString(frm->valueForKey("FrameRect")->getCString());
            if(LHSettings::sharedInstance()->shouldScaleImageOnRetina(imgRetina)){
                rect.origin.x *=2.0f;
                rect.origin.y *=2.0f;
                rect.size.width *=2.0f;
                rect.size.height *=2.0f;
            }
            
			CCSpriteFrame* frame = CCSpriteFrame::frameWithTexture(batch->getTexture(),rect);
            ccframes->addObject(frame);      
        }
        
		LHAnimationNode* animNode = new LHAnimationNode(uniqueAnimName);
        animNode->setLoop(loop);
        animNode->setSpeed(animSpeed);
        animNode->setRepetitions(repetitions);
        animNode->setStartAtLaunch(startAtLaunch);
        animNode->setBatchNode(batch);
        animNode->setFrames(ccframes);
        
        animationsInLevel->setObject(animNode,uniqueAnimName->getCString());
    }
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::createAnimationFromDictionary(CCDictionary* spriteProp ,LHSprite* ccsprite)
{
	CCString* animName = CCString::stringWithFormat("%s",spriteProp->valueForKey("AnimName")->getCString());
	if(!animName->m_sString.compare(""))
	{
        LHAnimationNode* animNode = (LHAnimationNode*)animationsInLevel->objectForKey(animName->getCString());
        if(NULL != animNode)
        {
            if(animNode->getStartAtLaunch())
            {
                animNode->runAnimationOnSprite(ccsprite,animNotifierId,animNotifierSel ,notifOnLoopForeverAnim);
            }
            else
            {
                prepareAnimationWithUniqueName(animName,ccsprite);
            }
        }
	}
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::startAnimationWithUniqueName(CCString * animName ,LHSprite* ccsprite){    
    LHAnimationNode* animNode = (LHAnimationNode*)animationsInLevel->objectForKey(animName->getCString());
    if(NULL != animNode){
        animNode->runAnimationOnSprite(ccsprite ,animNotifierId ,animNotifierSel ,notifOnLoopForeverAnim);
    }
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::stopAnimationOnSprite(LHSprite*ccsprite){
    if(NULL != ccsprite){
        ccsprite->stopActionByTag(LH_ANIM_ACTION_TAG);
        ccsprite->setAnimation(NULL);
    }    
}
//------------------------------------------------------------------------------
void LevelHelperLoader::prepareAnimationWithUniqueName(CCString* animName ,LHSprite* sprite){
    LHAnimationNode* animNode = (LHAnimationNode*)animationsInLevel->objectForKey(animName->getCString());
    if(animNode == NULL)
        return;
    sprite->setAnimation(animNode);
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::registerNotifierOnAllAnimationEnds(CCNode *obj,SEL_CallFuncND sel){
    animNotifierId = obj;
    animNotifierSel = sel;
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::enableNotifOnLoopForeverAnimations(){
    notifOnLoopForeverAnim = true;
}
// ////////////////////////////////////////////////////////////////////////////////
// //GRAVITY
// ////////////////////////////////////////////////////////////////////////////////
bool LevelHelperLoader::isGravityZero(){
    if(gravity.x == 0 && gravity.y == 0)
        return true;
    return false;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createGravity(b2World* world){
	if(isGravityZero())
		CCLog("LevelHelper Warning: Gravity is not defined in the level. Are you sure you want to set a zero gravity?");
    world->SetGravity(b2Vec2(gravity.x, gravity.y));
}
////////////////////////////////////////////////////////////////////////////////
//PHYSIC BOUNDARIES
////////////////////////////////////////////////////////////////////////////////
b2Body* LevelHelperLoader::physicBoundarieForKey(CCString* key){
    LHSprite* spr = (LHSprite*)physicBoundariesInLevel->objectForKey(key->getCString());
    if(NULL == spr)
        return 0;
    return spr->getBody();
}
b2Body* LevelHelperLoader::leftPhysicBoundary(){
	return physicBoundarieForKey(CCString::stringWithCString("LHPhysicBoundarieLeft"));
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::rightPhysicBoundary(){
	return physicBoundarieForKey(CCString::stringWithCString("LHPhysicBoundarieRight"));
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::topPhysicBoundary(){
    return physicBoundarieForKey(CCString::stringWithCString("LHPhysicBoundarieTop"));
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::bottomPhysicBoundary(){
    return physicBoundarieForKey(CCString::stringWithCString("LHPhysicBoundarieBottom"));
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::hasPhysicBoundaries(){
	if(wb == NULL){
		return false;
	}
    CCRect rect = LHRectFromString(wb->valueForKey("WBRect")->getCString());    
    if(rect.size.width == 0 || rect.size.height == 0)
        return false;
	return true;
}
//------------------------------------------------------------------------------
CCRect LevelHelperLoader::physicBoundariesRect(){
    CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio;
    CCRect rect = LHRectFromString(wb->valueForKey("WBRect")->getCString());    
    rect.origin.x = rect.origin.x*wbConv.x,
    rect.origin.y = rect.origin.y*wbConv.y;
    rect.size.width = rect.size.width*wbConv.x;
    rect.size.height= rect.size.height*wbConv.y;
    return rect;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createPhysicBoundaries(b2World* _world){
	if(!hasPhysicBoundaries()){
        CCLog("LevelHelper WARNING - Please create physic boundaries in LevelHelper in order to call method \"createPhysicBoundaries\"");
        return;
    }	
    
    CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio;
    
    b2BodyDef bodyDef;		
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0.0f, 0.0f);
    b2Body* wbBodyT = _world->CreateBody(&bodyDef);
	b2Body* wbBodyL = _world->CreateBody(&bodyDef);
	b2Body* wbBodyB = _world->CreateBody(&bodyDef);
	b2Body* wbBodyR = _world->CreateBody(&bodyDef);
	
	{
		LHSprite* spr = new LHSprite();
		spr->setTag(wb->valueForKey("TagLeft")->intValue()); 
		spr->setVisible(false);
		spr->setUniqueName(CCString::create("LHPhysicBoundarieLeft"));
        spr->setBody(wbBodyL);    
        wbBodyL->SetUserData(spr);
        physicBoundariesInLevel->setObject(spr,"LHPhysicBoundarieLeft");
	}
	
	{
		LHSprite* spr = new LHSprite();

		spr->setTag(wb->valueForKey("TagRight")->intValue()); 
		spr->setVisible(false);
		spr->setUniqueName(CCString::create("LHPhysicBoundarieRight"));
		spr->setBody(wbBodyR);    
		wbBodyL->SetUserData(spr);
		physicBoundariesInLevel->setObject(spr,"LHPhysicBoundarieRight");
	}
	
	{
		LHSprite* spr = new LHSprite();
		spr->setTag(wb->valueForKey("TagTop")->intValue()); 
		spr->setVisible(false);
		spr->setUniqueName(CCString::create("LHPhysicBoundarieTop"));
		spr->setBody(wbBodyT);    
		wbBodyL->SetUserData(spr);
		physicBoundariesInLevel->setObject(spr,"LHPhysicBoundarieTop");		
	}
	
	{
		LHSprite* spr = new LHSprite();
		spr->setTag(wb->valueForKey("TagBottom")->intValue()); 
		spr->setVisible(false);
		spr->setUniqueName(CCString::create("LHPhysicBoundarieBottom"));
		spr->setBody(wbBodyB);    
		wbBodyL->SetUserData(spr);
		physicBoundariesInLevel->setObject(spr,"LHPhysicBoundarieBottom");	
	}
	
	wbBodyT->SetSleepingAllowed(wb->valueForKey("CanSleep")->boolValue());  
	wbBodyL->SetSleepingAllowed(wb->valueForKey("CanSleep")->boolValue());  
	wbBodyB->SetSleepingAllowed(wb->valueForKey("CanSleep")->boolValue());  
	wbBodyR->SetSleepingAllowed(wb->valueForKey("CanSleep")->boolValue());  
	
	
    CCRect rect = LHRectFromString(wb->valueForKey("WBRect")->getCString());    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
    {//TOP
        b2EdgeShape shape;
		
        b2Vec2 pos1 = b2Vec2(rect.origin.x/LHSettings::sharedInstance()->lhPtmRatio*wbConv.x,
							 (winSize.height - rect.origin.y*wbConv.y)/LHSettings::sharedInstance()->lhPtmRatio);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x + rect.size.width)*wbConv.x/LHSettings::sharedInstance()->lhPtmRatio, 
							 (winSize.height - rect.origin.y*wbConv.y)/LHSettings::sharedInstance()->lhPtmRatio);		
		shape.Set(pos1, pos2);
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb,&fixture);
        fixture.shape = &shape;
        fixture.isSensor = false;
        wbBodyT->CreateFixture(&fixture);
    }
	
    {//LEFT
        b2EdgeShape shape;
		
		b2Vec2 pos1 = b2Vec2(rect.origin.x*wbConv.x/LHSettings::sharedInstance()->lhPtmRatio,
							 (winSize.height - rect.origin.y*wbConv.y)/LHSettings::sharedInstance()->lhPtmRatio);
        
		b2Vec2 pos2 = b2Vec2((rect.origin.x*wbConv.x)/LHSettings::sharedInstance()->lhPtmRatio, 
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/LHSettings::sharedInstance()->lhPtmRatio);
        shape.Set(pos1, pos2);
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb,&fixture);
        fixture.shape = &shape;
        wbBodyL->CreateFixture(&fixture);
    }
	
    {//RIGHT
        b2EdgeShape shape;
        
        b2Vec2 pos1 = b2Vec2((rect.origin.x + rect.size.width)*wbConv.x/LHSettings::sharedInstance()->lhPtmRatio,
							 (winSize.height - rect.origin.y*wbConv.y)/LHSettings::sharedInstance()->lhPtmRatio);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x+ rect.size.width)*wbConv.x/LHSettings::sharedInstance()->lhPtmRatio, 
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/LHSettings::sharedInstance()->lhPtmRatio);
        shape.Set(pos1, pos2);
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb,&fixture);
        fixture.shape = &shape;
        wbBodyR->CreateFixture(&fixture);
    }
	
    {//BOTTOM
        b2EdgeShape shape;
        
        b2Vec2 pos1 = b2Vec2(rect.origin.x*wbConv.x/LHSettings::sharedInstance()->lhPtmRatio,
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/LHSettings::sharedInstance()->lhPtmRatio);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x+ rect.size.width)*wbConv.x/LHSettings::sharedInstance()->lhPtmRatio, 
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/LHSettings::sharedInstance()->lhPtmRatio);
        shape.Set(pos1, pos2);
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb,&fixture);
        fixture.shape = &shape;
        fixture.isSensor = false;
        wbBodyB->CreateFixture(&fixture);
    }
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removePhysicBoundaries(){    
    physicBoundariesInLevel->removeAllObjects();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::releasePhysicBoundaries(){
    removePhysicBoundaries();
    physicBoundariesInLevel->release();
    physicBoundariesInLevel = NULL;
}
////////////////////////////////////////////////////////////////////////////////
//PHYSICS
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::setMeterRatio(float ratio){
	LHSettings::sharedInstance()->lhPtmRatio = ratio;
}
//------------------------------------------------------------------------------
float LevelHelperLoader::meterRatio(){
	return LHSettings::sharedInstance()->lhPtmRatio;
}
//------------------------------------------------------------------------------
float LevelHelperLoader::pixelsToMeterRatio(){
    return LHSettings::sharedInstance()->lhPtmRatio*LHSettings::sharedInstance()->convertRatio.x;
}
//------------------------------------------------------------------------------
float LevelHelperLoader::pointsToMeterRatio(){
    return LHSettings::sharedInstance()->lhPtmRatio;
}
//------------------------------------------------------------------------------
b2Vec2 LevelHelperLoader::pixelToMeters(CCPoint point){
    return b2Vec2(point.x / pixelsToMeterRatio(), point.y / pixelsToMeterRatio());
}
//------------------------------------------------------------------------------
b2Vec2 LevelHelperLoader::pointsToMeters(CCPoint point){
    return b2Vec2(point.x / LHSettings::sharedInstance()->lhPtmRatio, point.y / LHSettings::sharedInstance()->lhPtmRatio);
}
//------------------------------------------------------------------------------
CCPoint LevelHelperLoader::metersToPoints(b2Vec2 vec){
    return CCPointMake(vec.x*LHSettings::sharedInstance()->lhPtmRatio, vec.y*LHSettings::sharedInstance()->lhPtmRatio);
}
//------------------------------------------------------------------------------
CCPoint LevelHelperLoader::metersToPixels(b2Vec2 vec){
	return ccpMult(CCPointMake(vec.x, vec.y), pixelsToMeterRatio());
}
////////////////////////////////////////////////////////////////////////////////
//BEZIERS
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::createAllBeziers(){
	CCObject* obj;
	CCARRAY_FOREACH(lhBeziers,obj){
		CCDictionary* bezierDict = (CCDictionary*)obj;
		LHBezierNode* node = new LHBezierNode();
		node->initWithDictionary(bezierDict ,cocosLayer,box2dWorld);
		
        CCString* uniqueName = (CCString*)bezierDict->objectForKey("UniqueName");
		if(NULL != node){
			beziersInLevel->setObject(node,uniqueName->getCString());
			int z = ((CCString*)bezierDict->objectForKey("ZOrder"))->intValue();
			cocosLayer->addChild(node,z);
		}		
    }
}
// //------------------------------------------------------------------------------
LHBezierNode* LevelHelperLoader::bezierNodeWithUniqueName(CCString* name){
	return (LHBezierNode*)beziersInLevel->objectForKey(name->getCString());
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeAllBezierNodes(){
    CCArray* keys = beziersInLevel->allKeys();
	CCObject* strObj;
    CCARRAY_FOREACH(keys,strObj){
		CCString* key = (CCString*)strObj;
        LHBezierNode* node = (LHBezierNode*)beziersInLevel->objectForKey(key->getCString());
        if(NULL != node){
            node->removeFromParentAndCleanup(true);
        }
    }
    beziersInLevel->removeAllObjects();
    beziersInLevel->release();	
    beziersInLevel = NULL;
}
////////////////////////////////////////////////////////////////////////////////
//PATH
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::createPathOnSprite(LHSprite* ccsprite ,CCDictionary* spriteProp){
    if(NULL == ccsprite || NULL == spriteProp)
        return;
    
	CCString* uniqueName = CCString::stringWithFormat("%s",spriteProp->valueForKey("PathName")->getCString());
    bool isCyclic = spriteProp->valueForKey("PathIsCyclic")->boolValue();
    float pathSpeed = spriteProp->valueForKey("PathSpeed")->floatValue();
    int startPoint = spriteProp->valueForKey("PathStartPoint")->intValue(); //0 is first 1 is end
    bool pathOtherEnd = spriteProp->valueForKey("PathOtherEnd")->boolValue(); //false means will restart where it finishes
    int axisOrientation = spriteProp->valueForKey("PathOrientation")->intValue(); //false means will restart where it finishes
	
    moveSprite(ccsprite,uniqueName ,pathSpeed ,startPoint ,isCyclic ,pathOtherEnd,axisOrientation);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::moveSprite(LHSprite *ccsprite,CCString* uniqueName ,float pathSpeed ,bool startAtEndPoint,bool isCyclic,bool restartOtherEnd,int axis)
{
    if(NULL == ccsprite || uniqueName == NULL)
        return;
	
	LHBezierNode* node = bezierNodeWithUniqueName(uniqueName);
	
	if(NULL != node)
	{
		LHPathNode* pathNode = node->addSpriteOnPath(ccsprite,pathSpeed,startAtEndPoint,isCyclic ,restartOtherEnd,axis);
        
        if(NULL != pathNode)
        {
            pathNode->setPathNotifierObject(pathNotifierId);
            pathNode->setPathNotifierSelector(pathNotifierSel);
        }
	}
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::registerNotifierOnAllPathEndPoints(CCNode*obj,SEL_CallFuncND sel){
    pathNotifierId = obj;
    pathNotifierSel = sel;
}
// ////////////////////////////////////////////////////////////////////////////////
// //SPRITES
// ////////////////////////////////////////////////////////////////////////////////
LHSprite* LevelHelperLoader::spriteWithUniqueName(CCString* name){
    return (LHSprite*)spritesInLevel->objectForKey(name->getCString());	
}
// //------------------------------------------------------------------------------
CCArray* LevelHelperLoader::spritesWithTag(enum LevelHelper_TAG tag)
{
	CCArray* array = CCArray::create();
	CCArray *keys = spritesInLevel->allKeys();
	CCObject* obj;
	CCARRAY_FOREACH(keys,obj){
		CCString *key = (CCString*)obj;
		LHSprite* ccSprite = (LHSprite*)spritesInLevel->objectForKey(key->getCString());
		if(NULL != ccSprite && ccSprite->getTag() == (int)tag){
			array->addObject(ccSprite);
		}
	}
	return array;
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::createSpritesWithPhysics() 
{
	CCObject*obj;    
    CCARRAY_FOREACH(lhSprites,obj)
	{
		CCDictionary* dictionary = (CCDictionary*)obj;
		CCDictionary* spriteProp = (CCDictionary*)dictionary->objectForKey("GeneralProperties");
		CCDictionary* physicProp = (CCDictionary*)dictionary->objectForKey("PhysicProperties");
		
		CCString *image = (CCString*)spriteProp->valueForKey("Image");
		if ( image->m_sString == "hero1_body.png" )
			image = CCString::stringWithFormat("hero%d_body.png",g_nHeroSkinIndex+1);
		CCString *newImage = LHSettings::sharedInstance()->imagePath(image,false);

		LHBatch* bNode = (LHBatch*)batchNodesInLevel->objectForKey(newImage->getCString());
		CCSpriteBatchNode *batch = bNode->spriteBatchNode();
		
		if(NULL != batch)
		{
            LHSprite* ccsprite = spriteWithBatchFromDictionary(spriteProp,bNode);
            batch->addChild(ccsprite,((CCString*)spriteProp->objectForKey("ZOrder"))->intValue());
            
            CCString* uniqueName = (CCString*)spriteProp->objectForKey("UniqueName");
            if(physicProp->valueForKey("Type")->intValue() != 3) //3 means no physic
            {
                b2Body* body = b2BodyFromDictionary(physicProp,spriteProp,ccsprite ,box2dWorld);
                
                if(0 != body)
                    ccsprite->setBody(body);
                
                spritesInLevel->setObject(ccsprite,uniqueName->getCString());			
            }
            else {
                spritesInLevel->setObject(ccsprite,uniqueName->getCString());
                setCustomAttributesForNonPhysics(spriteProp,ccsprite);
            }
            
            if(!spriteProp->valueForKey("IsInParallax")->boolValue() &&
               !spriteProp->valueForKey("PathName")->m_sString.compare("None"))
            {
                createPathOnSprite(ccsprite,spriteProp);
            }
            
            createAnimationFromDictionary(spriteProp,ccsprite);
		}
	}
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::setFixtureDefPropertiesFromDictionary(CCDictionary* spritePhysic ,b2FixtureDef* shapeDef)
{
	shapeDef->density = spritePhysic->valueForKey("Density")->floatValue();
	shapeDef->friction = spritePhysic->valueForKey("Friction")->floatValue();
	shapeDef->restitution = spritePhysic->valueForKey("Restitution")->floatValue();
	
	shapeDef->filter.categoryBits = spritePhysic->valueForKey("Category")->intValue();
	shapeDef->filter.maskBits = spritePhysic->valueForKey("Mask")->intValue();
	shapeDef->filter.groupIndex = spritePhysic->valueForKey("Group")->intValue();
    
    shapeDef->isSensor = spritePhysic->valueForKey("IsSenzor")->boolValue();
}
// //------------------------------------------------------------------------------
b2Body* LevelHelperLoader::b2BodyFromDictionary(CCDictionary* spritePhysic,CCDictionary*spriteProp,LHSprite*ccsprite ,b2World* _world)
{
	b2BodyDef bodyDef;	
	
	int bodyType = spritePhysic->valueForKey("Type")->intValue();
	if(bodyType == 3) //in case the user wants to create a body with a sprite that has type as "NO_PHYSIC"
		bodyType = 2;
	bodyDef.type = (b2BodyType)bodyType;
	
	CCPoint pos = ccsprite->getPosition();	
	bodyDef.position.Set(pos.x/LHSettings::sharedInstance()->lhPtmRatio,pos.y/LHSettings::sharedInstance()->lhPtmRatio);
    
	bodyDef.angle = convertAngleByScreen(CC_DEGREES_TO_RADIANS(-1*spriteProp->valueForKey("Angle")->floatValue()));
    bodyDef.userData = ccsprite;
    
	b2Body* body = _world->CreateBody(&bodyDef);
    
	body->SetFixedRotation(spritePhysic->valueForKey("FixedRot")->boolValue());
	
	CCPoint linearVelocity = LHPointFromString(spritePhysic->valueForKey("LinearVelocity")->getCString());
	
    float linearDamping = spritePhysic->valueForKey("LinearDamping")->floatValue(); 
    float angularVelocity = spritePhysic->valueForKey("AngularVelocity")->floatValue();
    float angularDamping = spritePhysic->valueForKey("AngularDamping")->floatValue();     
    
    bool isBullet = spritePhysic->valueForKey("IsBullet")->boolValue();
    bool canSleep = spritePhysic->valueForKey("CanSleep")->boolValue();
	
	
	CCArray* fixtures = (CCArray*)spritePhysic->objectForKey("ShapeFixtures");
	CCPoint scale = LHPointFromString(spriteProp->valueForKey("Scale")->getCString()); 
    
    CCPoint size = LHPointFromString(spriteProp->valueForKey("Size")->getCString());
    
    CCPoint border = LHPointFromString(spritePhysic->valueForKey("ShapeBorder")->getCString());
    
	CCPoint offset = LHPointFromString(spritePhysic->valueForKey("ShapePositionOffset")->getCString());
	
	float gravityScale = spritePhysic->valueForKey("GravityScale")->floatValue();
	
    scale.x *= LHSettings::sharedInstance()->convertRatio.x;
    scale.y *= LHSettings::sharedInstance()->convertRatio.y;        
    
	if(scale.x == 0)
		scale.x = 0.01;
	if(scale.y == 0)
		scale.y = 0.01;
	
	if(fixtures == NULL || fixtures->count() == 0 || ((CCArray*)fixtures->objectAtIndex(0))->count() == 0)
	{
		b2PolygonShape shape;
		b2FixtureDef fixture;
		b2CircleShape circle;
		setFixtureDefPropertiesFromDictionary(spritePhysic,&fixture);
		
		if(spritePhysic->valueForKey("IsCircle")->boolValue())
		{
            if(LHSettings::sharedInstance()->convertLevel)
            {
				//    NSLog("convert circle");
                //this is for the ipad scale on circle look weird if we dont do this
                float scaleSpr = ccsprite->getScaleX();
                ccsprite->setScaleY(scaleSpr);
            }
            
			float circleScale = scale.x; //if we dont do this we dont have collision
			if(circleScale < 0)
				circleScale = -circleScale;
			
			float radius = (size.x*circleScale/2.0f - border.x/2.0f*circleScale)/LHSettings::sharedInstance()->lhPtmRatio;
			
			if(radius < 0)
				radius *= -1;
			circle.m_radius = radius; 
			
			circle.m_p.Set(offset.x/2.0f/LHSettings::sharedInstance()->lhPtmRatio, -offset.y/2.0f/LHSettings::sharedInstance()->lhPtmRatio);
			
			fixture.shape = &circle;
            body->CreateFixture(&fixture);
		}
		else
		{
            //THIS WAS ADDED BECAUSE I DISCOVER A BUG IN BOX2d
            //that makes linearImpulse to not work the body is in contact with
            //a box object
            int vsize = 4;
			b2Vec2 *verts = new b2Vec2[vsize];
			b2PolygonShape shape;

			float xRatio = LHSettings::sharedInstance()->lhPtmRatio;// LHSettings::sharedInstance()->convertRatio.y * LHSettings::sharedInstance()->convertRatio.x;
			float yRatio = LHSettings::sharedInstance()->lhPtmRatio;
			
            verts[0].x = ( (-1* size.x + border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/xRatio;
            verts[0].y = ( (-1* size.y + border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/yRatio;
			
            verts[1].x = ( (+ size.x - border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/xRatio;
            verts[1].y = ( (-1* size.y + border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/yRatio;
			
            verts[2].x = ( (+ size.x - border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/xRatio;
            verts[2].y = ( (+ size.y - border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/yRatio;
			
            verts[3].x = ( (-1* size.x + border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/xRatio;
            verts[3].y = ( (+ size.y - border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/yRatio;
			
			shape.Set(verts, vsize);		
            
			fixture.shape = &shape;
            body->CreateFixture(&fixture);
            delete verts;
		}
	}
	else
	{
		CCObject *obj;
		CCARRAY_FOREACH( fixtures,obj)
		{
			CCArray* curFixture = (CCArray*)obj;
			int size = (int)curFixture->count();
			b2Vec2 *verts = new b2Vec2[size];
			b2PolygonShape shape;
			int i = 0;
			CCObject *strObj;
			CCARRAY_FOREACH(curFixture,strObj)
			{
				CCString* pointStr = (CCString*)strObj;
				CCPoint point = LHPointFromString(pointStr->getCString());
				verts[i] = b2Vec2((point.x*(scale.x)+offset.x/2.0f)/LHSettings::sharedInstance()->lhPtmRatio, 
								  (point.y*(scale.y)-offset.y/2.0f)/LHSettings::sharedInstance()->lhPtmRatio);
				++i;
			}
			shape.Set(verts, size);		
			b2FixtureDef fixture;
			setFixtureDefPropertiesFromDictionary(spritePhysic,&fixture);
			fixture.shape = &shape;
			body->CreateFixture(&fixture);
			delete[] verts;
		}
	}
	
    setCustomAttributesForPhysics(spriteProp ,body,ccsprite);
	
	body->SetGravityScale(gravityScale);
	body->SetSleepingAllowed(canSleep);    
    body->SetBullet(isBullet);
    body->SetLinearVelocity(b2Vec2(linearVelocity.x, linearVelocity.y));
    body->SetAngularVelocity(angularVelocity);
    body->SetLinearDamping(linearDamping);
    body->SetAngularDamping(angularDamping);
	
	
	return body;
	
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::releaseAllSprites()
{
    removeAllSprites();
	spritesInLevel->removeAllObjects();
    spritesInLevel->release();
    spritesInLevel = NULL;
}
// //------------------------------------------------------------------------------
bool LevelHelperLoader::removeSprite(LHSprite* ccsprite){
	if(NULL == ccsprite)
		return false;
    
    //if(ccsprite->respondsToSelector:selector(uniqueName)]){
        spritesInLevel->removeObjectForKey(ccsprite->getUniqueName()->getCString());
    //}
	ccsprite->removeBodyFromWorld();
    ccsprite->removeFromParentAndCleanup(true);	
	return true;
}
// //------------------------------------------------------------------------------
bool LevelHelperLoader::removeSpritesWithTag(enum LevelHelper_TAG tag){
	CCArray *keys = spritesInLevel->allKeys();
    if(NULL == keys)
        return false;
	CCObject* strObj;
	CCARRAY_FOREACH( keys,strObj)
	{
		CCString* key = (CCString*)strObj;
        LHSprite* spr = spriteWithUniqueName(key);
        if(NULL != spr){
            if(tag == spr->getTag()){
                removeSprite(spr);
            }
        }
	}
	return true;	
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::removeAllSprites()
{	
	CCArray *keys = spritesInLevel->allKeys();
    if(keys == NULL)
        return false;
	CCObject* strObj;
	CCARRAY_FOREACH( keys,strObj)
	{
		CCString* key = (CCString*)strObj;
        if(key != NULL){
            LHSprite* spr = spriteWithUniqueName(key);
            removeSprite(spr);
        }
	}
	return true;	
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::newSpriteWithUniqueName(CCString *name)
{
	CCObject *obj;
    CCARRAY_FOREACH(lhSprites,obj){
		CCDictionary* dictionary = (CCDictionary*)obj;
		CCDictionary* spriteProp = (CCDictionary*)dictionary->objectForKey("GeneralProperties");
		if(spriteProp->valueForKey("UniqueName")->m_sString.compare(name->m_sString)){            
            LHSprite* ccsprite =  spriteFromDictionary(spriteProp);
			CCString* uName = CCString::stringWithFormat("%s_LH_NEW_SPRITE_%d", 
                               name->getCString(), LHSettings::sharedInstance()->getNewBodyId());
            ccsprite->setUniqueName(uName);
            return ccsprite;
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::newPhysicalSpriteWithUniqueName(CCString* name)
{
	CCObject *obj;
	CCARRAY_FOREACH(lhSprites,obj){
		CCDictionary* dictionary = (CCDictionary*)obj;
		CCDictionary* spriteProp = (CCDictionary*)dictionary->objectForKey("GeneralProperties");
		if (spriteProp->valueForKey("UniqueName")->m_sString.compare(name->m_sString))
		{            
            CCDictionary* physicProp = (CCDictionary*)dictionary->objectForKey("PhysicProperties");
            LHSprite* ccsprite = spriteFromDictionary(spriteProp);

            b2Body* body =  b2BodyFromDictionary(physicProp,spriteProp,ccsprite ,box2dWorld);

            if(0 != body)
                ccsprite->setBody(body);

			CCString* uName = CCString::stringWithFormat("%s_LH_NEW_BODY_%d", 
                               name->getCString(), LHSettings::sharedInstance()->getNewBodyId());
            ccsprite->setUniqueName(uName);

            return ccsprite;
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::newBatchSpriteWithUniqueName(CCString *name)
{
	CCObject *obj;
	CCARRAY_FOREACH(lhSprites,obj){
		CCDictionary* dictionary = (CCDictionary*)obj;
		CCDictionary* spriteProp = (CCDictionary*)dictionary->objectForKey("GeneralProperties");
		if (spriteProp->valueForKey("UniqueName")->m_sString.compare(name->m_sString)){            
            //find the coresponding batch node for this sprite
            LHBatch* bNode = (LHBatch*)batchNodesInLevel->objectForKey(spriteProp->valueForKey("Image")->getCString());
            if(NULL != bNode)
			{
                CCSpriteBatchNode *batch = bNode->spriteBatchNode();
                if(NULL != batch){
                    LHSprite* ccsprite = spriteWithBatchFromDictionary(spriteProp,bNode);
                    batch->addChild(ccsprite,spriteProp->valueForKey("ZOrder")->intValue());
                    
					CCString* uName = CCString::stringWithFormat("%s_LH_NEW_BATCH_SPRITE_%d", 
                                       name->getCString(), LHSettings::sharedInstance()->getNewBodyId());
                    ccsprite->setUniqueName(uName);
                    return ccsprite;
                }
            }
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::newPhysicalBatchSpriteWithUniqueName(CCString *name)
{
	CCObject *obj;
	CCARRAY_FOREACH(lhSprites,obj){
		CCDictionary* dictionary = (CCDictionary*)obj;
		CCDictionary* spriteProp = (CCDictionary*)dictionary->objectForKey("GeneralProperties");
		if (spriteProp->valueForKey("UniqueName")->m_sString.compare(name->m_sString)){            
            //find the coresponding batch node for this sprite
			LHBatch* bNode = (LHBatch*)batchNodesInLevel->objectForKey(spriteProp->valueForKey("Image")->getCString());
			if(NULL != bNode)
			{
				CCSpriteBatchNode *batch = bNode->spriteBatchNode();
                if(NULL != batch){
                    LHSprite* ccsprite = spriteWithBatchFromDictionary(spriteProp ,bNode);
                    batch->addChild(ccsprite ,spriteProp->valueForKey("ZOrder")->intValue());
                    
                    CCDictionary* physicProp = (CCDictionary*)dictionary->objectForKey("PhysicProperties");
                    b2Body* body =  b2BodyFromDictionary(physicProp,spriteProp,ccsprite ,box2dWorld);
                    
                    if(0 != body)
                        ccsprite->setBody(body);
                    
					CCString* uName = CCString::stringWithFormat("%s_LH_NEW_BATCH_BODY_%d", 
                                       name->getCString(), LHSettings::sharedInstance()->getNewBodyId());
                    ccsprite->setUniqueName(uName);
                    return ccsprite;
                }
            }
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::spriteFromDictionary(CCDictionary* spriteProp)
{
    CCRect uv = LHRectFromString(spriteProp->valueForKey("UV")->getCString());
  
	CCString* img = CCString::stringWithFormat("%s",LHSettings::sharedInstance()->imagePath((CCString*)spriteProp->valueForKey("Image"))->getCString());
    
    if(LHSettings::sharedInstance()->shouldScaleImageOnRetina(img))
    {
        uv.origin.x *=2.0f;
        uv.origin.y *=2.0f;
        uv.size.width *=2.0f;
        uv.size.height *=2.0f;
    }
	LHSprite *ccsprite = LHSprite::spriteWithFile(img,uv);
	setSpriteProperties(ccsprite,spriteProp);
	return ccsprite;
}
// //------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::spriteWithBatchFromDictionary(CCDictionary* spriteProp ,LHBatch* lhBatch)
{
    CCRect uv = LHRectFromString(spriteProp->valueForKey("UV")->getCString());
    
    if(lhBatch == NULL)
        return NULL;
    
    CCSpriteBatchNode* batch = lhBatch->spriteBatchNode();
    
    if(batch == NULL)
        return NULL;
    
    CCString* img = LHSettings::sharedInstance()->imagePath(lhBatch->getUniqueName());
    
    if(LHSettings::sharedInstance()->shouldScaleImageOnRetina(img))
    {
        uv.origin.x *=2.0f;
        uv.origin.y *=2.0f;
        uv.size.width *=2.0f;
        uv.size.height *=2.0f;
    }
    
	LHSprite *ccsprite = LHSprite::spriteWithBatchNode(batch ,uv);
	setSpriteProperties(ccsprite,spriteProp);
	return ccsprite;	
}
// //------------------------------------------------------------------------------
void LevelHelperLoader::setSpriteProperties(LHSprite* ccsprite, CCDictionary* spriteProp)
{
	//convert position from LH to Cocos2d coordinates
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint position = LHPointFromString(((CCString*)spriteProp->objectForKey("Position"))->getCString());
	
	position.x *= LHSettings::sharedInstance()->convertRatio.x;
	position.y *= LHSettings::sharedInstance()->convertRatio.y;
    
    position.y = winSize.height - position.y;
    
	ccsprite->setPosition(position);
	ccsprite->setRotation(((CCString*)spriteProp->objectForKey("Angle"))->floatValue());
	ccsprite->setOpacity(255*((CCString*)spriteProp->objectForKey("Opacity"))->floatValue()*LHSettings::sharedInstance()->customAlpha);
	CCRect color = LHRectFromString(((CCString*)spriteProp->objectForKey("Color"))->getCString());
	ccsprite->setColor(ccc3(255*color.origin.x, 255*color.origin.y, 255*color.size.width));
	CCPoint scale = LHPointFromString(((CCString*)spriteProp->objectForKey("Scale"))->getCString());
	ccsprite->setVisible(((CCString*)spriteProp->objectForKey("Scale"))->boolValue());
    ccsprite->setTag(((CCString*)spriteProp->objectForKey("Tag"))->intValue());
    
	//scale.x *= LHSettings::sharedInstance()->convertRatio.x;
	//scale.y *= LHSettings::sharedInstance()->convertRatio.y;
	//Scale Recalc
 	float x = LHSettings::sharedInstance()->convertRatio.x * cos(ccsprite->getRotation()*M_PI/180);
 	float y = LHSettings::sharedInstance()->convertRatio.y * sin(ccsprite->getRotation()*M_PI/180);
 	scale.x *= sqrtf(x*x+y*y);
 	x = LHSettings::sharedInstance()->convertRatio.x * sin(ccsprite->getRotation()*M_PI/180);
 	y = LHSettings::sharedInstance()->convertRatio.y * cos(ccsprite->getRotation()*M_PI/180);
 	scale.y *= sqrtf(x*x+y*y);
    
    CCString* img = LHSettings::sharedInstance()->imagePath((CCString*)spriteProp->objectForKey("Image"));
    
    if(LHSettings::sharedInstance()->shouldScaleImageOnRetina(img))
    {
        scale.x /=2.0f;
        scale.y /=2.0f;
    }
    
    //this is to fix a noise issue on cocos2d.
    scale.x += 0.015f*scale.x;
    scale.y += 0.015f*scale.y;
   
	ccsprite->setScaleX(scale.x);
	ccsprite->setScaleY(scale.y);
	
    ccsprite->setUniqueName((CCString*)spriteProp->objectForKey("UniqueName"));
}
// ////////////////////////////////////////////////////////////////////////////////
// //PARALLAX
// ////////////////////////////////////////////////////////////////////////////////
LHParallaxNode* LevelHelperLoader::paralaxNodeWithUniqueName(CCString* uniqueName){
    return (LHParallaxNode*)parallaxesInLevel->objectForKey(uniqueName->getCString());
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createParallaxes()
{
	CCObject* obj;
    CCARRAY_FOREACH(lhParallax,obj){
		CCDictionary* parallaxDict = (CCDictionary*)obj;
		LHParallaxNode* node = (LHParallaxNode*)parallaxNodeFromDictionary(parallaxDict,cocosLayer);
        if(NULL != node){
			parallaxesInLevel->setObject(node ,parallaxDict->valueForKey("UniqueName")->getCString());
		}
    }
}
//------------------------------------------------------------------------------
LHParallaxNode* LevelHelperLoader::parallaxNodeFromDictionary(CCDictionary* parallaxDict,CCLayer* layer)
{
	LHParallaxNode* node = new LHParallaxNode(parallaxDict);
    
    if(layer != NULL && node != NULL){
        int z = parallaxDict->valueForKey("ZOrder")->intValue();
        layer->addChild(node,z);
    }
    CCArray* spritesInfo = (CCArray*)parallaxDict->objectForKey("Sprites");

	CCObject* obj;
    CCARRAY_FOREACH(spritesInfo,obj){
		CCDictionary* sprInf = (CCDictionary*)obj;
        float ratioX = sprInf->valueForKey("RatioX")->floatValue();
        float ratioY = sprInf->valueForKey("RatioY")->floatValue();
		CCString* sprName = CCString::stringWithFormat("%s",sprInf->valueForKey("SpriteName")->getCString());
        
		LHSprite* spr = spriteWithUniqueName(sprName);
		if(NULL != node && spr != NULL){
			node->addChild(spr,ccp(ratioX, ratioY));
		}
    }
    return node;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::releaseAllParallaxes()
{
	CCArray* keys = parallaxesInLevel->allKeys();
    
	CCObject* obj;
	CCARRAY_FOREACH(keys,obj){
		CCString* key = (CCString*)obj;
		LHParallaxNode* par = (LHParallaxNode*)parallaxesInLevel->objectForKey(key->getCString());
		if(NULL != par){
            par->removeFromParentAndCleanup(true);
		}
	}
	parallaxesInLevel->removeAllObjects();
    parallaxesInLevel->release();
    parallaxesInLevel = NULL;
}
////////////////////////////////////////////////////////////////////////////////
//JOINTS
////////////////////////////////////////////////////////////////////////////////
LHJoint* LevelHelperLoader::jointFromDictionary(CCDictionary* joint,b2World* world)
{
    b2Joint* boxJoint = 0;
    
	if(NULL == joint)
		return 0;
	
	if(world == 0)
		return 0;
    
    LHSprite* sprA = (LHSprite*)spritesInLevel->objectForKey(joint->valueForKey("ObjectA")->getCString());
    b2Body* bodyA = sprA->getBody();
	
    LHSprite* sprB = (LHSprite*)spritesInLevel->objectForKey(joint->valueForKey("ObjectB")->getCString());
    b2Body* bodyB = sprB->getBody();
	
	if(NULL == bodyA || 
       NULL == bodyB )
		return NULL;
	
	CCPoint anchorA = LHPointFromString(joint->valueForKey("AnchorA")->getCString());
	CCPoint anchorB = LHPointFromString(joint->valueForKey("AnchorB")->getCString());
	
	bool collideConnected = joint->valueForKey("CollideConnected")->boolValue();
	
    int tag = joint->valueForKey("Tag")->intValue();
    int type = joint->valueForKey("Type")->intValue();
    
	b2Vec2 posA, posB;
	
	float convertX = LHSettings::sharedInstance()->convertRatio.x;
	float convertY = LHSettings::sharedInstance()->convertRatio.y;
    
    if(!joint->valueForKey("CenterOfMass")->boolValue())
    {
        posA = b2Vec2((bodyA->GetWorldCenter().x*LHSettings::sharedInstance()->lhPtmRatio + convertX*anchorA.x)/LHSettings::sharedInstance()->lhPtmRatio, 
                      (bodyA->GetWorldCenter().y*LHSettings::sharedInstance()->lhPtmRatio - convertY*anchorA.y)/LHSettings::sharedInstance()->lhPtmRatio);
        
        posB = b2Vec2((bodyB->GetWorldCenter().x*LHSettings::sharedInstance()->lhPtmRatio + convertX*anchorB.x)/LHSettings::sharedInstance()->lhPtmRatio, 
                      (bodyB->GetWorldCenter().y*LHSettings::sharedInstance()->lhPtmRatio - convertY*anchorB.y)/LHSettings::sharedInstance()->lhPtmRatio);
    }
    else {		
        posA = bodyA->GetWorldCenter();
        posB = bodyB->GetWorldCenter();
    }
	
	if(0 != bodyA && 0 != bodyB)
	{
		switch (type)
		{
			case LH_DISTANCE_JOINT:
			{
				b2DistanceJointDef jointDef;
				
				jointDef.Initialize(bodyA, 
									bodyB, 
									posA,
									posB);
				
				jointDef.collideConnected = collideConnected;
				
				jointDef.frequencyHz = joint->valueForKey("Frequency")->floatValue();
				jointDef.dampingRatio = joint->valueForKey("Damping")->floatValue();
				
				if(0 != world)
				{
					boxJoint = (b2DistanceJoint*)world->CreateJoint(&jointDef);
				}
			}	
				break;
				
			case LH_REVOLUTE_JOINT:
			{
				b2RevoluteJointDef jointDef;
				
				jointDef.lowerAngle = CC_DEGREES_TO_RADIANS(joint->valueForKey("LowerAngle")->floatValue());
				jointDef.upperAngle = CC_DEGREES_TO_RADIANS(joint->valueForKey("UpperAngle")->floatValue());
				jointDef.motorSpeed = joint->valueForKey("MotorSpeed")->floatValue(); //Usually in radians per second. ?????
				jointDef.maxMotorTorque = joint->valueForKey("MaxTorque")->floatValue(); //Usually in N-m.  ?????
				jointDef.enableLimit = joint->valueForKey("EnableLimit")->boolValue();
				jointDef.enableMotor = joint->valueForKey("EnableMotor")->boolValue();
				jointDef.collideConnected = collideConnected;    
				
				jointDef.Initialize(bodyA, bodyB, posA);
				
				if(0 != world)
				{
					boxJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;
				
			case LH_PRISMATIC_JOINT:
			{
				b2PrismaticJointDef jointDef;
				
				// Bouncy limit
				CCPoint axisPt = LHPointFromString(joint->valueForKey("Axis")->getCString());
				
				b2Vec2 axis(axisPt.x, axisPt.y);
				axis.Normalize();
				
				jointDef.Initialize(bodyA, bodyB, posA, axis);
				
				jointDef.motorSpeed = joint->valueForKey("MotorSpeed")->floatValue();
				jointDef.maxMotorForce = joint->valueForKey("MaxMotorForce")->floatValue();
				
				
				jointDef.lowerTranslation =  CC_DEGREES_TO_RADIANS(joint->valueForKey("LowerTranslation")->floatValue());
				jointDef.upperTranslation = CC_DEGREES_TO_RADIANS(joint->valueForKey("UpperTranslation")->floatValue());
				
				jointDef.enableMotor = joint->valueForKey("EnableMotor")->boolValue();
				jointDef.enableLimit = joint->valueForKey("EnableLimit")->boolValue();
				jointDef.collideConnected = collideConnected;   
				if(0 != world)
				{
					boxJoint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);
				}
			}	
				break;
				
			case LH_PULLEY_JOINT:
			{
				b2PulleyJointDef jointDef;
				
				CCPoint grAnchorA = LHPointFromString(joint->valueForKey("GroundAnchorA")->getCString());
				CCPoint grAnchorB = LHPointFromString(joint->valueForKey("GroundAnchorB")->getCString());
				
				CCSize winSize = CCDirector::sharedDirector()->getWinSize();/* displaySizeInPixels);*/
				
				grAnchorA.y = winSize.height - convertY*grAnchorA.y;
				grAnchorB.y = winSize.height - convertY*grAnchorB.y;
				
				b2Vec2 groundAnchorA = b2Vec2(convertX*grAnchorA.x/LHSettings::sharedInstance()->lhPtmRatio, 
											  grAnchorA.y/LHSettings::sharedInstance()->lhPtmRatio);
				
				b2Vec2 groundAnchorB = b2Vec2(convertX*grAnchorB.x/LHSettings::sharedInstance()->lhPtmRatio, 
											  grAnchorB.y/LHSettings::sharedInstance()->lhPtmRatio);
				
				float ratio = joint->valueForKey("Ratio")->floatValue();
				jointDef.Initialize(bodyA, bodyB, groundAnchorA, groundAnchorB, posA, posB, ratio);				
				jointDef.collideConnected = collideConnected;   
				
				if(0 != world)
				{
					boxJoint = (b2PulleyJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;
				
			case LH_GEAR_JOINT:
			{
				b2GearJointDef jointDef;
				
				jointDef.bodyA = bodyB;
				jointDef.bodyB = bodyA;
				
				if(bodyA == 0)
					return 0;
				if(bodyB == 0)
					return 0;
				
                LHJoint* jointAObj = jointWithUniqueName((CCString*)joint->valueForKey("JointA"));
                b2Joint* jointA = jointAObj->getJoint();
                
                LHJoint* jointBObj = jointWithUniqueName((CCString*)joint->valueForKey("JointB"));
                b2Joint* jointB = jointBObj->getJoint();
                
				if(jointA == 0)
					return 0;
				if(jointB == 0)
					return 0;
				
				
				jointDef.joint1 = jointA;
				jointDef.joint2 = jointB;
				
				jointDef.ratio =joint->valueForKey("Ratio")->floatValue();
				jointDef.collideConnected = collideConnected;
				if(0 != world)
				{
					boxJoint = (b2GearJoint*)world->CreateJoint(&jointDef);
				}
			}	
				break;
				
				
			case LH_WHEEL_JOINT: //aka line joint
			{
				b2WheelJointDef jointDef;
				
				CCPoint axisPt = LHPointFromString(joint->valueForKey("Axis")->getCString());
				b2Vec2 axis(axisPt.x, axisPt.y);
				axis.Normalize();
				
				jointDef.motorSpeed = joint->valueForKey("MotorSpeed")->floatValue(); //Usually in radians per second. ?????
				jointDef.maxMotorTorque = joint->valueForKey("MaxTorque")->floatValue(); //Usually in N-m.  ?????
				jointDef.enableMotor = joint->valueForKey("EnableMotor")->boolValue();
				jointDef.frequencyHz = joint->valueForKey("Frequency")->floatValue();
				jointDef.dampingRatio = joint->valueForKey("Damping")->floatValue();
				
				jointDef.Initialize(bodyA, bodyB, posA, axis);
				jointDef.collideConnected = collideConnected; 
				
				if(0 != world)
				{
					boxJoint = (b2WheelJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;				
			case LH_WELD_JOINT:
			{
				b2WeldJointDef jointDef;
				
				jointDef.frequencyHz = joint->valueForKey("Frequency")->floatValue();
				jointDef.dampingRatio = joint->valueForKey("Damping")->floatValue();
				
				jointDef.Initialize(bodyA, bodyB, posA);
				jointDef.collideConnected = collideConnected; 
				
				if(0 != world)
				{
					boxJoint = (b2WheelJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;
				
			case LH_ROPE_JOINT: //NOT WORKING YET AS THE BOX2D JOINT FOR THIS TYPE IS A TEST JOINT
			{
				
				b2RopeJointDef jointDef;
				
				jointDef.localAnchorA = bodyA->GetPosition();
				jointDef.localAnchorB = bodyB->GetPosition();
				jointDef.bodyA = bodyA;
				jointDef.bodyB = bodyB;
				jointDef.maxLength = joint->valueForKey("MaxLength")->floatValue();
				jointDef.collideConnected = collideConnected; 
				
				if(0 != world)
				{
					boxJoint = (b2RopeJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;
				
			case LH_FRICTION_JOINT:
			{
				b2FrictionJointDef jointDef;
				
				jointDef.maxForce = joint->valueForKey("MaxForce")->floatValue();
				jointDef.maxTorque = joint->valueForKey("MaxTorque")->floatValue();
				
				jointDef.Initialize(bodyA, bodyB, posA);
				jointDef.collideConnected = collideConnected; 
				
				if(0 != world)
				{
					boxJoint = (b2FrictionJoint*)world->CreateJoint(&jointDef);
				}
				
			}
				break;
				
			default:
				CCLog("Unknown joint type in LevelHelper file.");
				break;
		}
	}
    
	LHJoint* levelJoint = new LHJoint();
	levelJoint->initWithUniqueName((CCString*)joint->valueForKey("UniqueName"));
    levelJoint->tag = tag;
    levelJoint->type = (LH_JOINT_TYPE)type;
    levelJoint->setJoint(boxJoint);
    boxJoint->SetUserData(levelJoint);
    
	return levelJoint;
}
// //------------------------------------------------------------------------------
LHJoint* LevelHelperLoader::jointWithUniqueName(CCString* name){
	CCDictElement*obj;
	CCDICT_FOREACH(jointsInLevel,obj)
	{
		LHJoint *joint = (LHJoint*)obj;
		joint = joint;
	}
    return (LHJoint*)jointsInLevel->objectForKey(name->getCString());
}
// //------------------------------------------------------------------------------
CCArray* LevelHelperLoader::jointsWithTag(enum LevelHelper_TAG tag)
{
	CCArray *keys = jointsInLevel->allKeys();
	CCArray* jointsWithTag = CCArray::create();

	CCObject* obj;
	CCARRAY_FOREACH(keys,obj){
		CCString* key = (CCString*)obj;
        LHJoint* levelJoint = (LHJoint*)jointsInLevel->objectForKey(key->getCString());
        if(levelJoint->tag == tag){
            jointsWithTag->addObject(levelJoint);
        }
	}
    return jointsWithTag;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createJoints()
{
	CCObject *obj;    
    CCARRAY_FOREACH(lhJoints,obj)
	{
		CCDictionary* jointDict = (CCDictionary*)obj;
		LHJoint* boxJoint = jointFromDictionary(jointDict ,box2dWorld);
		
		if(NULL != boxJoint)
        {
			jointsInLevel->setObject(boxJoint,jointDict->valueForKey("UniqueName")->getCString());	
		}
	}	
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::removeAllJoints()
{
	jointsInLevel->removeAllObjects();
    return true;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::releaseAllJoints(){
    removeAllJoints();
    jointsInLevel->release();
    jointsInLevel = NULL;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeJointsWithTag(enum LevelHelper_TAG tag){
	CCArray *keys = jointsInLevel->allKeys();
	CCObject*obj;
	CCARRAY_FOREACH(keys,obj){
		CCString* key = (CCString*)obj;
		LHJoint* joint = (LHJoint*)jointsInLevel->objectForKey(key->getCString());
        if(joint->tag == tag){
            jointsInLevel->removeObjectForKey(key->getCString());
        }
	}
}
// //------------------------------------------------------------------------------
bool LevelHelperLoader::removeJoint(LHJoint* joint){
	if(0 == joint)
		return false;
    jointsInLevel->removeObjectForKey(joint->getUniqueName()->getCString());
	return true;
}
// 
// ////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////
// 
// 
void LevelHelperLoader::setCustomAttributesForPhysics(CCDictionary*spriteProp ,b2Body* body,LHSprite* sprite)
{
    
}
void LevelHelperLoader::setCustomAttributesForNonPhysics(CCDictionary* spriteProp ,LHSprite* sprite)
{
    
}

void LevelHelperLoader::setCustomAttributesForBezierBodies(CCDictionary* bezierProp ,CCNode* sprite ,b2Body* body)
{
    
}
//////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::loadLevelHelperSceneFile(CCString* levelFile,CCString* subfolder,CCString* imgFolder)
{
	//CCString *path = [[NSBundle mainBundle] pathForResource:levelFile ofType:"plhs" inDirectory:subfolder); 
	CCString *path = CCString::stringWithFormat("%s%s%s.plhs",subfolder->getCString(),imgFolder->getCString(),levelFile->getCString());
	
	CCAssert(NULL!=path, "Invalid level file. Please add the LevelHelper scene file to Resource folder. Please do not add extension in the given string.");
	
	const char* realPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(path->getCString());
	CCDictionary *dictionary = CCDictionary::dictionaryWithContentsOfFile(realPath);
	
	processLevelFileFromDictionary(dictionary);
}

// void)loadLevelHelperSceneFileFromWebAddress:(CCString* webaddress
// {	
// 	NSURL *url = [NSURL URLWithString:webaddress);
// 	
// 	CCDictionary *dictionary = [CCDictionary dictionaryWithContentsOfURL:url);
// 	
// 	if(dictionary == NULL)
// 		NSLog("Provided web address is wrong or connection error.");
// 	
// 	processLevelFileFromDictionary:dictionary);
// }

void LevelHelperLoader::loadLevelHelperSceneFromDictionary(CCDictionary* levelDictionary ,CCString* imgFolder)
{	
	LHSettings::sharedInstance()->setImageFolder(imgFolder);
	processLevelFileFromDictionary(levelDictionary);
}

void LevelHelperLoader::processLevelFileFromDictionary(CCDictionary* dictionary)
{
	if(NULL == dictionary)
		return;
	
	bool fileInCorrectFormat =	(dictionary->valueForKey("Author"))->m_sString.compare("Bogdan Vladu") &&
								(dictionary->valueForKey("CreatedWith"))->m_sString.compare("LevelHelper");
	if(fileInCorrectFormat == false)
		CCLog("This file was not created with LevelHelper or file is damaged.");
	
    CCDictionary* scenePref = (CCDictionary*)dictionary->objectForKey("ScenePreference");
    safeFrame = LHPointFromString(scenePref->valueForKey("SafeFrame")->getCString());
    gameWorldRect = LHRectFromString(scenePref->valueForKey("GameWorld")->getCString());
	
    
	CCRect color = LHRectFromString(scenePref->valueForKey("BackgroundColor")->getCString());
	glClearColor(color.origin.x, color.origin.y, color.size.width, 1);
    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    LHSettings::sharedInstance()->setConvertRatio(CCPointMake(winSize.width/safeFrame.x, winSize.height/safeFrame.y));
    
    float safeFrameDiagonal = sqrtf(safeFrame.x* safeFrame.x + safeFrame.y* safeFrame.y);
    float winDiagonal = sqrtf(winSize.width* winSize.width + winSize.height*winSize.height);
    float PTM_conversion = winDiagonal/safeFrameDiagonal;
    
	LevelHelperLoader::setMeterRatio(LHSettings::sharedInstance()->lhPtmRatio*PTM_conversion);
    
	////////////////////////LOAD WORLD BOUNDARIES//////////////////////////////////////////////
	if(NULL != dictionary->objectForKey("WBInfo"))
	{
		wb = (CCDictionary*)dictionary->objectForKey("WBInfo");
	}
	
	////////////////////////LOAD SPRITES////////////////////////////////////////////////////
	lhSprites = new CCArray();
	lhSprites->initWithArray((CCArray*)dictionary->objectForKey("SPRITES_INFO"));
	
	///////////////////////////LOAD BATCH IMAGES////////////////////////////////////////////
	CCArray* batchImages = (CCArray*)dictionary->objectForKey("LoadedImages");
	CCObject* obj;
	CCARRAY_FOREACH(batchImages,obj)
	{
		CCDictionary* imageInfo = (CCDictionary*)obj;
        CCString* image = (CCString*)imageInfo->valueForKey("Image");		
		
		if ( image->m_sString == "hero1_body.png" )
			image = CCString::stringWithFormat("hero%d_body.png",g_nHeroSkinIndex+1);
		
		CCSpriteBatchNode *batch = CCSpriteBatchNode::batchNodeWithFile(LHSettings::sharedInstance()->imagePath(image)->getCString());
		
		LHBatch* bNode = new LHBatch();
		bNode->batchWithUniqueName(image);
        bNode->setSpriteBatchNode(batch);
        bNode->z = imageInfo->valueForKey("OrderZ")->intValue();
        batchNodesInLevel->setObject(bNode,image->getCString());		
	}
	
	///////////////////////LOAD JOINTS//////////////////////////////////////////////////////////
	lhJoints = new CCArray();
	lhJoints->initWithArray((CCArray*)dictionary->objectForKey("JOINTS_INFO"));	
	
    //////////////////////LOAD PARALLAX/////////////////////////////////////////
    lhParallax = new CCArray();
	lhParallax->initWithArray((CCArray*)dictionary->objectForKey("PARALLAX_INFO"));
    
    ////////////////////LOAD BEZIER/////////////////////////////////////////////
    lhBeziers = new CCArray();
	lhBeziers->initWithArray((CCArray*)dictionary->objectForKey("BEZIER_INFO"));
    
    ////////////////////LOAD ANIMS//////////////////////////////////////////////
    lhAnims = new CCArray();
	lhAnims->initWithArray((CCArray*)dictionary->objectForKey("ANIMS_INFO"));
    
    gravity = LHPointFromString(dictionary->valueForKey("Gravity")->getCString());
}