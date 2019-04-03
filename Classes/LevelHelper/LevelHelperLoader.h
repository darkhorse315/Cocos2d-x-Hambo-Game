//  This file is part of LevelHelper
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
////////////////////////////////////////////////////////////////////////////////
//
//  Version history
//  ...............
//  v0.1 First version for LevelHelper 1.4
////////////////////////////////////////////////////////////////////////////////

#ifndef LEVELHELPERLOADER_H
#define LEVELHELPERLOADER_H


#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "nodes/LHSprite.h"
#include "nodes/LHJoint.h"
#include "nodes/LHParallaxNode.h"
#include "nodes/LHBezierNode.h"
#include "nodes/LHBatch.h"

enum LevelHelper_TAG 
{ 
	DEFAULT_TAG 	= 0,
    HERO_TAG,
    
    HERO_BULLET_START_TAG,
    HERO_HAND_GRENADE_BULLET,
    HERO_KNIFE_BULLET,
    HERO_PISTOL_BULLET,
    HERO_UZI_BULLET,
    HERO_FIRE_ARROW_BULLET,
    HERO_BOMB_BULLET,
	HERO_BOMB_CONTROLER,
    HERO_BULLET_END_TAG,
    
    ENEMY_START_TAG = 10,
    ENEMY_TAG,
    ENEMY_END_TAG,
    OBSTACLE_TAG,
    ICE_TAG,
    CURSOR_TAG,
    TUTORIAL_TAG,
    DELETED_TAG,

	NUMBER_OF_TAGS 	= 1
};

enum LH_ACTIONS_TAGS
{
    LH_PATH_ACTION_TAG,
    LH_ANIM_ACTION_TAG
};

//#if TARGET_OS_EMBEDDED || TARGET_OS_IPHONE || TARGET_OS_WIN32 || NS_BUILD_32_LIKE_64

#define LHRectFromString(str) CCRectFromString(str)
#define LHPointFromString(str) CCPointFromString(str)
#define LHPoint CCPoint
#define LHRect  CCRect
#define LHMakePoint(x, y) CCPointMake(x, y)

CCString* LHStringWithRect(CCRect rect);
CCString* LHStringWithCCPoint(CCPoint pt);

/*#else

#define LHRectFromString(str) NSRectToCCRect(NSRectFromString(str))
#define LHPointFromString(str) NSPointToCCPoint(NSPointFromString(str))
#define LHPoint CCPoint
#define LHRect CCRect
#define LHMakePoint(x, y) CCPointMake(x, y)

#endif*/

/*
CCRect LHRectFromValue(NSValue* val);




protocol LevelHelperLoaderCustomCCSprite
optional

LHSprite* spriteFromDictionary(CCDictionary*)spriteProp;

LHSprite* spriteWithBatchFromDictionary(CCDictionary*)spriteProp 
								 batchNode(LHBatch*)batch;

//this 3 methods should be overloaded together
//first one is for physic sprites 
void setCustomAttributesForPhysics(CCDictionary*)spriteProp 
								 body(b2Body*body
							   sprite(LHSprite*sprite;
//this second one is for the case where you dont use physics or you have sprites
//with "NO PHYSIC" as physic type
void setCustomAttributesForNonPhysics(CCDictionary*)spriteProp 
								  sprite(LHSprite*sprite;
//the third one is for bezier shapes that are not paths
void setCustomAttributesForBezierBodies(CCDictionary*)bezierProp 
                                      node(CCNode*)sprite body(b2Body*body;


end*/

class LevelHelperLoader : public CCObject/*NSObject<LevelHelperLoaderCustomCCSprite>*/ 
{	
	CCArray* lhSprites;	//array of CCDictionary with keys GeneralProperties (CCDictionary) 
    //and PhysicProperties (CCDictionary)
	CCArray* lhJoints;	//array of CCDictionary
    CCArray* lhParallax;//array of CCDictionary 
    CCArray* lhBeziers; //array of CCDictionary
    CCArray* lhAnims;   //array of CCDictionary
	
    CCDictionary* animationsInLevel; //key - uniqueAnimationName value - LHAnimation*
	CCDictionary* spritesInLevel;	//key - uniqueSpriteName	value - LHSprite*
    CCDictionary* jointsInLevel;     //key - uniqueJointName     value - LHJoint*
    CCDictionary* parallaxesInLevel; //key - uniqueParallaxName  value - LHParallaxNode*
	CCDictionary* beziersInLevel;    //key - uniqueBezierName    value - LHBezierNode*
	CCDictionary* batchNodesInLevel;	//key - imageName			value - LHBatch*
    
    
	CCDictionary* wb; //world boundaries Info
    CCDictionary* physicBoundariesInLevel; //keys//LHPhysicBoundarieTop
                                                        //LHPhysicBoundarieLeft
                                                        //LHPhysicBoundarieBottom
                                                        //LHPhysicBoundarieRight 
                                                    //value - LHSprite*    
	
	bool addSpritesToLayerWasUsed;
	bool addObjectsToWordWasUsed;
    
    CCPoint safeFrame;
    CCRect  gameWorldRect;
    CCPoint gravity;
	
    CCNode*  pathNotifierId;
    SEL_CallFuncND pathNotifierSel;
    
    CCNode*      animNotifierId;
    SEL_CallFuncND     animNotifierSel;
    bool    notifOnLoopForeverAnim;
public:
	CCLayer* cocosLayer; //weak ptr
    b2World* box2dWorld; //weak ptr

 	~LevelHelperLoader();
	LHSprite* spriteFromDictionary(CCDictionary*spriteProp);

	LHSprite* spriteWithBatchFromDictionary(CCDictionary*spriteProp,LHBatch*batch);

	void convertScaleByScreen( CCPoint &scale, float angle );
	float convertAngleByScreen( float angle );
	//this 3 methods should be overloaded together
	//first one is for physic sprites 
	void setCustomAttributesForPhysics(CCDictionary* spriteProp ,b2Body*body,LHSprite*sprite);
	//this second one is for the case where you dont use physics or you have sprites
	//with "NO PHYSIC" as physic type
	void setCustomAttributesForNonPhysics(CCDictionary* spriteProp ,LHSprite*sprite);
	//the third one is for bezier shapes that are not paths
	void setCustomAttributesForBezierBodies(CCDictionary* bezierProp ,CCNode* spritem,b2Body*body);
// 
 	void initObjects();
 	void loadLevelHelperSceneFile(CCString* levelFile,CCString* subfolder,CCString* imgFolder);
	void addBatchNodesToLayer(CCLayer* _cocosLayer);
 	void createAllAnimationsInfo();
 	void loadLevelHelperSceneFromDictionary(CCDictionary* levelDictionary ,CCString* imgFolder);
// //------------------------------------------------------------------------------
 	void initWithContentOfFile(CCString* levelFile);
 	void initWithContentOfFileFromInternet(CCString*webAddress);
 	void initWithContentOfFile(CCString*levelFile ,CCString*levelFolder);
// 	//------------------------------------------------------------------------------
// 	//used by Scene Tester
 	void initWithContentOfDictionary(CCDictionary* levelDictionary,CCString* imgFolder);
// 	//------------------------------------------------------------------------------
// 	//LOADING
	void addObjectsToWorld(b2World* world,CCLayer* cocosLayer);
	void createAllBeziers();
 	void addSpritesToLayer(CCLayer* cocosLayer); //NO PHYSICS
// 	//------------------------------------------------------------------------------
// 	//SPRITES
 	LHSprite* spriteWithUniqueName(CCString *name); 
	CCArray*  spritesWithTag(enum LevelHelper_TAG tag); //returns array with LHSprite*
 	bool removeSprite(LHSprite*ccsprite);
 	bool removeSpritesWithTag(enum LevelHelper_TAG tag);
 	bool removeAllSprites();
// 	/*More methods in LHSpitesExt.h - download from http://www.levelhelper.org*/
// 	//------------------------------------------------------------------------------
// 	//CONVERSION
// 	/*More methods in LHConversionExt.h - download from http://www.levelhelper.org*/
// 	//------------------------------------------------------------------------------
// 	//CREATION
// 
// 	//New sprite and associated body will be released automatically
// 	//or you can use removeFromParentAndCleanup:YES, CCSprite method, to do it at a specific time
// 	//you must set the desired position after creation
// 
// 	//sprites returned needs to be added in the layer by you
// 	//new sprite unique name for the returned sprite will be
// 	//[OLDNAME]_LH_NEW__SPRITE_XX and [OLDNAME]_LH_NEW_BODY_XX
 	LHSprite* newSpriteWithUniqueName(CCString  *name); //no physic body
 	LHSprite* newPhysicalSpriteWithUniqueName(CCString *name);//with physic body
// 
// 	//sprites are added in the coresponding batch node automatically
// 	//new sprite unique name for the returned sprite will be
// 	//[OLDNAME]_LH_NEW_BATCH_SPRITE_XX and [OLDNAME]_LH_NEW_BATCH_BODY_XX
 	LHSprite* newBatchSpriteWithUniqueName(CCString *name); //no physic body
 	LHSprite* newPhysicalBatchSpriteWithUniqueName(CCString *name); //with physic body
// 
// 	/*More methods in LHCreationExt.h - download from http://www.levelhelper.org*/
// 	//------------------------------------------------------------------------------
// 	//JOINTS
	LHJoint* jointWithUniqueName(CCString *name);
 	CCArray* jointsWithTag(enum LevelHelper_TAG tag); //returns array with LHJoint*
 	void removeJointsWithTag(enum LevelHelper_TAG tag);
 	bool removeJoint(LHJoint*joint);
 	bool removeAllJoints();
	/*More methods in LHJointsExt.h - download from http://www.levelhelper.org*/
	//------------------------------------------------------------------------------
	//PARALLAX
// 	LHParallaxNode* paralaxNodeWithUniqueName(CCString*uniqueName);
// 	/*More methods in LHParallaxExt.h - download from http://www.levelhelper.org*/
// 	//------------------------------------------------------------------------------
// 	//BEZIER
// 	LHBezierNode* bezierNodeWithUniqueName(CCString *name);
	void          removeAllBezierNodes();
	//------------------------------------------------------------------------------
	//GRAVITY
	bool isGravityZero();
	void createGravity(b2World*world);
	//------------------------------------------------------------------------------
	//PHYSIC BOUNDARIES
	void createPhysicBoundaries(b2World*_world);
	CCRect physicBoundariesRect();
	bool hasPhysicBoundaries();

	b2Body* leftPhysicBoundary();
	b2Body* rightPhysicBoundary();
	b2Body* topPhysicBoundary();
	b2Body* bottomPhysicBoundary();
	void removePhysicBoundaries();
	//------------------------------------------------------------------------------
	//LEVEL INFO
	CCPoint gameScreenSize(); //the device size set in loaded level
	CCRect gameWorldSize(); //the size of the game world
	//------------------------------------------------------------------------------
	//BATCH
	unsigned int numberOfBatchNodesUsed();
	/*More methods in LHBatchExt.h - download from http://www.levelhelper.org*/
	//------------------------------------------------------------------------------
	//ANIMATION
 	void startAnimationWithUniqueName(CCString * animName ,LHSprite*ccsprite);

	void stopAnimationOnSprite(LHSprite*ccsprite);

	//this will not start the animation - it will just prepare it
	void prepareAnimationWithUniqueName(CCString* animName ,LHSprite*sprite);


	//needs to be called before addObjectsToWorld or addSpritesToLayer
	//signature for registered method should be like this: void spriteAnimHasEnded(CCSprite*)spr animationName(CCString*)animName
	//registration is done like this: [loader registerNotifierOnAnimationEnds:self selector:selector(spriteAnimHasEnded:animationName:)];
	//this will trigger for all type of animations even for the ones controlled by you will next/prevFrameFor...
	void registerNotifierOnAllAnimationEnds(CCNode* obj,SEL_CallFuncND sel);
// 	/*
// 	 by default the notification on animation end works only on non-"loop forever" animations
// 	 if you want to receive notifications on "loop forever" animations enable this behaviour
// 	 before addObjectsToWorld by calling the following function
// 	 */
	void enableNotifOnLoopForeverAnimations();

	/*More methods in LHAnimationsExt.h - download from http://www.levelhelper.org*/
	//------------------------------------------------------------------------------
	//PATH
	void moveSprite(LHSprite * ccsprite, CCString* uniqueName ,float pathSpeed ,bool startAtEndPoint,bool isCyclic,bool restartOtherEnd,int axis);
	void createPathOnSprite(LHSprite* ccsprite ,CCDictionary* spriteProp);
	void createAnimationFromDictionary(CCDictionary* spriteProp ,LHSprite* ccsprite);
	void createSpritesWithPhysics();
	void createParallaxes();
	void createJoints();

	void releaseAllBatchNodes();
	void releaseAllSprites();
	void releaseAllParallaxes();
	void releaseAllJoints();

	b2Body* physicBoundarieForKey(CCString* key);
	void setFixtureDefPropertiesFromDictionary(CCDictionary* spritePhysic ,b2FixtureDef* shapeDef);
	void releasePhysicBoundaries();
	LHBezierNode* bezierNodeWithUniqueName(CCString* name);
	b2Body* b2BodyFromDictionary(CCDictionary* spritePhysic,CCDictionary*spriteProp,LHSprite*ccsprite ,b2World* _world);
	LHParallaxNode* paralaxNodeWithUniqueName(CCString* uniqueName);
	LHParallaxNode* parallaxNodeFromDictionary(CCDictionary* parallaxDict,CCLayer* layer);
	LHJoint* jointFromDictionary(CCDictionary* joint,b2World* world);
	void processLevelFileFromDictionary(CCDictionary* dictionary);

	//DISCUSSION
	//signature for registered method should be like this: voidspriteMoveOnPathEnded(LHSprite*spr pathUniqueName(CCString*)str;
	//registration is done like this: [loader registerNotifierOnPathEndPoints:self selector:selector(spriteMoveOnPathEnded:pathUniqueName:)];
 	void registerNotifierOnAllPathEndPoints(CCNode* obj,SEL_CallFuncND sel);

	/*More methods in LHPathExt.h - download from http://www.levelhelper.org*/
	//------------------------------------------------------------------------------
	//PHYSICS
	static void setMeterRatio(float ratio); //default is 32.0f
	static float meterRatio();

	float pixelsToMeterRatio();
	float pointsToMeterRatio();

	b2Vec2 pixelToMeters(CCPoint point); //Cocos2d point to Box2d point
	b2Vec2 pointsToMeters(CCPoint point); //Cocos2d point to Box2d point

	CCPoint metersToPoints(b2Vec2 vec); //Box2d point to Cocos2d point
	CCPoint metersToPixels(b2Vec2 vec); //Box2d point to Cocos2d pixels
	//------------------------------------------------------------------------------
	//needed when deriving this class
	void setSpriteProperties(LHSprite*ccsprite ,CCDictionary* spriteProp);
	////////////////////////////////////////////////////////////////////////////////
};

#endif