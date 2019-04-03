//
//  GameLayer.h
//  OutZone_iphone
//
//  Created by admin on 1/19/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "Box2D/Box2D.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "hero/HeroManager.h"
#include "enemy/EnemyManager.h"
#include "MyListener.h"
#include "gamelayer/bullet/BombEffectManager.h"
#include "cocos-ext.h"
#include "GUI/CCControlExtension/CCControlButton.h"
#include "MyTutorial.h"
#ifdef DEBUG_BOX2D
#include "GLES-Render.h"
#endif

class BULLETINFO: public CCObject
{
public:
	int m_Index;
	int m_Type;
	float m_Angle;
	float m_BulletForce;
};


class GameLayer : public ScaleLayer
{
private:
#ifdef DEBUG_BOX2D
    GLESDebugDraw *m_debugDraw;
#endif
	b2World* m_world;
    MyListener*		m_pContactListener;
    
    LevelHelperLoader *m_level;
    EnemyManager    *m_enemyManager;    
    
    CCMenuItem *m_btnBGM;
	CCMenuItem *m_btnMuteBGM;

	CCLabelTTF *m_tapStartLabel;
	CCLayerColor *m_tapColor;
    
    int         m_nGameState;
    
    CCLabelBMFont *m_lblLevel;
    CCLabelTTF *m_lblLife; 
    CCLabelBMFont *m_lblGameScore;//CCLabeFX *m_lblGameScore;
    bool        m_bShootBullet;
    

	MyTutorial  m_TutotialRead;
    
    float m_fDirectionAngle;
    
    float     m_nDirectionWidth;
    float     m_nDirectionSubWidth;
    int     m_nCode;

	int		  m_nBodyCount;
	CCDictionary*  m_MemBodyDict;
    
    CCMenuItem *m_btnGuns[7];
	CCMenu	   *m_menuGuns[7];
	cocos2d::extension::CCControlButton *menuPause;
    CCLabelTTF *m_lblBulletCount[6];
    
    int         m_nBulletInfo[6];
    
    
    CCSprite *m_sprCloud[5];
    
    float m_fCloudSpeed[5];
    
    CCSprite *m_sprGameResultTxt;
    
    int       m_nTutorialIndex;
	CCPoint		m_CurPoi;
	bool		bDown;
	CCArray		*m_BulletArray;
	int nGameScore;

public:
    HeroManager *m_heroManager;
    BombEffectManager *m_bmbManager;
	CCSprite *m_sprDirection1;
	CCSprite *m_sprDirection2;
	bool m_bHeroFriendDestroyed;

	static CCScene* scene();
	MyTutorial  m_TutotialWrite;
	float         m_nTick;

	bool init();
	void draw();

	void ccTouchesEnded(CCSet * touches,CCEvent * event);
	void ccTouchesBegan(CCSet * touches,CCEvent * event);
	void ccTouchesMoved(CCSet * touches,CCEvent * event);
	void displayGameResult(float t);

	void enableButtons(bool bEnable);

	b2World* getWorld() { return m_world; };
	~GameLayer();
	void onTime(float dt);
	void tick(float dt);

	void onPause(CCObject* sender, cocos2d::extension::CCControlEvent event);

	void onGun1(CCObject* sender);
	void onGun2(CCObject* sender);
	void onGun3(CCObject* sender);
	void onGun4(CCObject* sender);
	void onGun5(CCObject* sender);
	void onGun6(CCObject* sender);
	void onGun7(CCObject* sender);

	void afterStep();
	void step(float dt);
	void shootUzi(float dt);
	void hideTapStart(float dt);
	void createScore(CCPoint pos,int nScore);

	void initGameInfo();
	void initVariables() ;
	void initButtons();
	void initWorld();
	void initClouds();
	void createListener();

	void initHeroManager();
	void initEnemyManager();
	void initGunBar();
	void initHelpLabels();
	void initHelpLabels_1_1(int z);
	void initHelpLabels_1_2(int z);
	void initHelpLabels_1_5(int z);
	void initHelpLabels_1_19(int z);
	void initHelpLabels_1_20(int z);
	void initHelpLabels_3_3(int z);
	void initHelpLabels_3_8(int z);
	void initHelpLabels_3_9(int z);
	void initHelpLabels_3_12(int z);
	void initHelpLabels_4_7(int z);

	void hideDirections();

	void loadLevel();

	void createBullets(CCPoint ptTouchedPos,float fBulletForce);
	void createBullets(CCPoint ptTouchedPos);

	void drawCurLevelNumber();
	void drawLife();

	void processSubLife();
	void processDirection();
	void processTutorial();
	void processTutorial_Body();
	void processEnemyAfraid();

	void composeClouds();
	void flyClouds();

	void checkGameResult();
	void submitScore();

	void displayGameOver();
	void displayGameSuccess();
	void displayBulletCount();
	void displayGameScore();

	void flyGameOverText();

	bool isEmptyBullet();
	void processBomb(LHSprite* bomb);
	void replaceWeaponBoard(int newWeaponID);

	void spriteMoveOnPathEnded(/*LHSprite* spr,CCString* pathName*/);
	void spriteAnimHasEnded(/*LHSprite* spr, CCString* animName*/);

	void unselectAllGunMenus();
	void preloadImages();

	CREATE_FUNC(GameLayer);
};