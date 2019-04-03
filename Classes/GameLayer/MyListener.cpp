

#include "MyListener.h"
#include "Constants.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "gamelayer/bullet/Bullet.h"
#include "gamelayer/GameLayer.h"

MyListener::MyListener()
{
}

MyListener::~MyListener()
{
}

void MyListener::BeginContact(b2Contact* contact)
{
    if(g_bGamePause)
        return;
    LHSprite* dataA = (LHSprite*)contact->GetFixtureA()->GetBody()->GetUserData();
	LHSprite* dataB = (LHSprite*)contact->GetFixtureB()->GetBody()->GetUserData();
	
	if (dataA == NULL && dataB == NULL) {
		return;
	}
	int nType1 = dataA->getTag();
	int nType2 = dataB->getTag();
    
    if(g_bGameSuccess)
        return;
    if(g_bGameOver)
        return;
    
    CCPoint ptBulletPos = ccp(-213, -240);
    int nBulletType = -1;
    int nCraskType = -1;
    CCString *strEnemyName;
    int nBulletForce = -1;
//check hero's bullet and others
    if((nType1 > HERO_BULLET_START_TAG && nType1 < HERO_BULLET_END_TAG) && (nType2 > ENEMY_START_TAG && nType2 < ENEMY_END_TAG)){
        if(dataB->isVisible() && ( nType1 == HERO_KNIFE_BULLET || nType1 == HERO_PISTOL_BULLET || nType1 == HERO_UZI_BULLET )){
            ptBulletPos = dataB->getPosition();
            nBulletType = nType1;
            nCraskType = nType2;
            strEnemyName = dataB->getUniqueName();
            nBulletForce = ((Bullet*)dataA)->m_nForce;
        }
        if(nType1 == HERO_KNIFE_BULLET || nType1 == HERO_PISTOL_BULLET || nType1 == HERO_UZI_BULLET )            
            dataA->setTag(DELETED_TAG);
    }else if((nType1 > ENEMY_START_TAG && nType1 < ENEMY_END_TAG) && (nType2 > HERO_BULLET_START_TAG && nType2 < HERO_BULLET_END_TAG)){
        if(dataA->isVisible() && (nType2 == HERO_KNIFE_BULLET || nType2 == HERO_PISTOL_BULLET || nType2 == HERO_UZI_BULLET )){
            ptBulletPos = dataA->getPosition();
            nBulletType = nType2;
            nCraskType = nType1;
            strEnemyName = dataA->getUniqueName();
            nBulletForce = ((Bullet*)dataB)->m_nForce;
        }
        if(nType2 == HERO_KNIFE_BULLET || nType2 == HERO_PISTOL_BULLET || nType2 == HERO_UZI_BULLET )
            dataB->setTag(DELETED_TAG);
    }
	
	//bomb process -- RCH
	if ( nType1 == HERO_FIRE_ARROW_BULLET )
		dataA->bomb( ((Bullet*)dataA)->m_level );
	else if ( nType2 == HERO_FIRE_ARROW_BULLET )
		dataB->bomb( ((Bullet*)dataB)->m_level );
	
	if ( nType1 > HERO_BULLET_START_TAG && nType1 < HERO_BULLET_END_TAG ) ((Bullet*)dataA)->collison( dataB );
	if ( nType2 > HERO_BULLET_START_TAG && nType2 < HERO_BULLET_END_TAG ) ((Bullet*)dataB)->collison( dataA );

	if ( (nType1 == HERO_PISTOL_BULLET || nType1 == HERO_UZI_BULLET) && isBarrel(dataB) ) 
	{
		dataB->bomb( ((Bullet*)dataA)->m_level );
		dataA->setTag(DELETED_TAG);
	}
	if ( (nType2 == HERO_PISTOL_BULLET || nType2 == HERO_UZI_BULLET) && isBarrel(dataA) ) 
	{
		dataA->bomb( ((Bullet*)dataB)->m_level );
		dataB->setTag(DELETED_TAG);
	}

	//////////////////////////////////////////////////////////////////////////
	

    //compose bullets effect array
    if(nBulletType != -1 && g_CanKillEnemy)
	{
		CCString*	str = CCString::createWithFormat("%f,%f,%d,%d,%s,%d,0", ptBulletPos.x, ptBulletPos.y, nBulletType, nCraskType, strEnemyName->getCString(), nBulletForce);
		g_arrBombEffect->addObject(str);
	}
    
    if((nType1 > HERO_BULLET_START_TAG && nType1 < HERO_BULLET_END_TAG) && (nType2 == ICE_TAG || nType2 == HERO_TAG)){     
		if ( !(nType2 == HERO_TAG && ((Bullet*)dataA)->bFirstMove) )
		{
			if( nType2 == ICE_TAG && (nType1 == HERO_PISTOL_BULLET || nType1 == HERO_UZI_BULLET))
			{
				dataA->setTag(DELETED_TAG);
				dataB->setTag(DELETED_TAG);
			}
			if( nType2 == HERO_TAG && (nType1 == HERO_PISTOL_BULLET || nType1 == HERO_UZI_BULLET || nType1 == HERO_KNIFE_BULLET))            
			{
				dataA->setTag(DELETED_TAG);
				dataB->setTag(DELETED_TAG);
			}
		}
    }else if((nType1 == ICE_TAG || nType1 == HERO_TAG) && (nType2 > HERO_BULLET_START_TAG && nType2 < HERO_BULLET_END_TAG)){
		if ( !(nType1 == HERO_TAG && ((Bullet*)dataB)->bFirstMove) )
		{

			if( nType1 == ICE_TAG && (nType2 == HERO_PISTOL_BULLET || nType2 == HERO_UZI_BULLET))
			{
				dataA->setTag(DELETED_TAG);
				dataB->setTag(DELETED_TAG);
			}
			if( nType1 == HERO_TAG && (nType2 == HERO_PISTOL_BULLET || nType2 == HERO_UZI_BULLET || nType2 == HERO_KNIFE_BULLET))            
			{
				dataA->setTag(DELETED_TAG);
				dataB->setTag(DELETED_TAG);
			}
		}
    }

    
    if((nType1 == HERO_PISTOL_BULLET || nType1 == HERO_UZI_BULLET || nType1 == HERO_KNIFE_BULLET || nType1 == HERO_HAND_GRENADE_BULLET) && (nType2  == OBSTACLE_TAG)){        
        dataA->setTag(DELETED_TAG);
    }else if((nType1 == OBSTACLE_TAG) && (nType2 == HERO_PISTOL_BULLET || nType2 == HERO_UZI_BULLET || nType2 == HERO_KNIFE_BULLET  || nType2 == HERO_HAND_GRENADE_BULLET)){
        dataB->setTag(DELETED_TAG);
    }
}

void MyListener::processBomb(CCPoint pos)
{
	
}

void MyListener::processSubLife() {
}

void MyListener::EndContact(b2Contact* contact) {
    
}

void MyListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}
////////////////////////////////////////////////////////////////////
/// class DestructionListener
////////////////////////////////////////////////////////////////////

void MyDestructionListener::SayGoodbye(b2Joint* joint)
{
}
