
#include "BombLHSprite.h"
#include "LevelHelper/Nodes/LHSprite.h"
#include "LevelHelper/Nodes/LHSettings.h"
#include "LevelHelper/Nodes/LHAnimationNode.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "Constants.h"
#include "AppDelegate.h"
#include "GameLayer/GameLayer.h"

BombLHSprite::BombLHSprite()
{
	afraidState = 0;
}


void BombLHSprite::bomb(LevelHelperLoader *level)
{
	m_level = level;
	if ( getTag() == DELETED_TAG ) return;
	if ( getTag() == HERO_HAND_GRENADE_BULLET || getTag() == HERO_BOMB_BULLET )
		proccessBomb(0);
	else
		schedule(schedule_selector(LHSprite::proccessBomb),0.1f);
}

#define BREAK_BOMB_RADIUS	107
#define FLY_BYBOMB_RADIUS	107
#define BOMB_FORCE			100

void BombLHSprite::proccessBomb(float dt)
{
	if(getTag() != HERO_BOMB_BULLET && getTag() != HERO_HAND_GRENADE_BULLET && !isBarrel(this) )
		return;
	if((getTag() == HERO_HAND_GRENADE_BULLET) || (getTag() == HERO_BOMB_BULLET && g_nSwitch == BREAK_BOMB) 
		|| isBarrel(this)){
		CCArray* allEffectedObjs = CCArray::create();
		allEffectedObjs->addObjectsFromArray(m_level->spritesWithTag(ENEMY_TAG));
		allEffectedObjs->addObjectsFromArray(m_level->spritesWithTag(HERO_TAG));
		allEffectedObjs->addObjectsFromArray(m_level->spritesWithTag(DEFAULT_TAG));
		allEffectedObjs->addObjectsFromArray(m_level->spritesWithTag(ICE_TAG));

		for(int i = 0; i < (int)allEffectedObjs->count(); i++){
			LHSprite *sprObj = (LHSprite*)allEffectedObjs->objectAtIndex(i);

			//if ( sprObj->getTag() == DEFAULT_TAG && !isBarrel(sprObj) ) continue;
			
			CCPoint disPoi (sprObj->getPosition().x - getPosition().x,sprObj->getPosition().y - getPosition().y);	

			float dis = ccpDistance(CCPoint(0,0),disPoi);
			//dis -= (sprObj->getContentSize().width + sprObj->getContentSize().height) / 4;
			dis = abs(dis);

			CCRect rectBomb = CCRectMake(getPosition().x - BREAK_BOMB_RADIUS, getPosition().y - BREAK_BOMB_RADIUS, BREAK_BOMB_RADIUS * 2, BREAK_BOMB_RADIUS * 2);
			CCRect rectflybyBomb = CCRectMake(getPosition().x - FLY_BYBOMB_RADIUS, getPosition().y - FLY_BYBOMB_RADIUS, FLY_BYBOMB_RADIUS * 2, FLY_BYBOMB_RADIUS * 2);

			CCRect sprRect = sprObj->boundingBox();

			if ( isBarrel(sprObj) && rectBomb.intersectsRect(sprRect) )
			{
				sprObj->bomb(m_level);
			}
			else if ( sprObj->getTag() == DEFAULT_TAG && rectflybyBomb.intersectsRect(sprRect))
			{
				int nExp = 1;
				//dis = FLY_BYBOMB_RADIUS - dis;
				disPoi.x *= ( BOMB_FORCE / dis );
				disPoi.y *= ( BOMB_FORCE / dis );
				b2Vec2 F(disPoi.x*nExp,disPoi.y*nExp);
				sprObj->getBody()->ApplyForce(F,sprObj->getBody()->GetPosition());
			}
			else
			{
				if( rectBomb.intersectsRect(sprRect) ){
					if ( sprObj->getTag() == HERO_TAG || sprObj->getTag() == ICE_TAG || sprObj->getTag() == ENEMY_TAG )
						sprObj->setTag(DELETED_TAG);
				}
				else if(rectflybyBomb.intersectsRect(sprRect))
				{
					int nExp = 1;
					//dis = FLY_BYBOMB_RADIUS - dis;
					disPoi.x *= ( BOMB_FORCE / dis );
					disPoi.y *= ( BOMB_FORCE / dis );
					b2Vec2 F(disPoi.x*nExp,disPoi.y*nExp);
					sprObj->getBody()->ApplyForce(F,sprObj->getBody()->GetPosition());
				}
			}
		}
		
		if(getTag() == HERO_BOMB_BULLET)
			g_nSwitch = 0;

		((AppDelegate*)CCApplication::sharedApplication())->playEff(E_BOMB);
		((GameLayer*)m_level->cocosLayer)->m_bmbManager->makeBombEffect(getPosition(),4);

		if ( getTag() == DEFAULT_TAG )
		{
			stopAllActions();
			unscheduleAllSelectors();
			removeFromParentAndCleanup(true);
			removeBodyFromWorld();
		}
		setTag(DELETED_TAG);
	}
}
