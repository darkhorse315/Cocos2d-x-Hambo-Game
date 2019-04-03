//
//  BulletManager.m
//  OutZone_iphone
//
//  Created by admin on 1/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BulletManager.h"
#include "Constants.h"

BulletManager::BulletManager(LevelHelperLoader *hLevel){    	
    m_level = hLevel;
}

Bullet* BulletManager::createBullets(int nType,float fAngle,CCPoint ptStart,CCPoint ptTarget,float fBulletSpeed)
{
    int nBulletImgType;
    int nForce = 2;
    int nBulletTag = nType;
    int nBulletSizeX = 2, nBulletSizeY = 2;
    switch (nType) {
        case HERO_HAND_GRENADE_BULLET:
            nBulletImgType = 1;
            nBulletSizeX = 4;
            nBulletSizeY = 4;
            break;
        case HERO_KNIFE_BULLET:
            nBulletImgType = 2;
            nBulletSizeX = 15;
            nBulletSizeY = 4;
            break;
        case HERO_PISTOL_BULLET:
            nBulletImgType = 3;
            break;
        case HERO_UZI_BULLET:
            nBulletImgType = 3;
            break;
        case HERO_FIRE_ARROW_BULLET:
            nBulletImgType = 5;
            nForce = 0;
            nBulletSizeX = 4;
            nBulletSizeY = 8;
            break;
        case HERO_BOMB_BULLET:
            nBulletImgType = 6;
            nForce = 0;
            nBulletSizeX = 11;
            nBulletSizeY = 11;
            break;        
        default:
            break;
    }
    Bullet *b = new Bullet(nType,m_level,ptStart ,ptTarget ,fAngle ,nBulletImgType ,nForce ,nBulletTag ,ccp(iDevPixelX(nBulletSizeX), iDevPixelY(nBulletSizeY)) ,fBulletSpeed);
    m_level->cocosLayer->addChild(b,1);
	return b;
}