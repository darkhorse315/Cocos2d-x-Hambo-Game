
#include "Constants.h"
#include "LevelHelper/Nodes/LHSprite.h"

const int STAGE_COUNT = 5;
const int LEVEL_COUNT = 200;
const int PER_LEVEL_COUNT = 40;
const int CHARACTER_TYPE_COUNT = 5;
const int INIT_PLAYER_LIFE = 10;
const int COINPERLEVEL = 50;

int      g_nCurMission = 0;			// 0 ~ STAGE_COUNT-1
int      g_nCurLevel = 0;			// 0 ~ LEVEL_COUNT-1
int      g_nAvailableLevel[5];		// 0 ~ LEVEL_COUNT-1
int		 g_nBestScore[200/*LEVEL_COUNT*/];

int		 g_nBodyIndex[100];

cocos2d::CCString g_resPath;

bool     g_bGameSuccess = false;
bool     g_bGameOver = false;
bool     g_bGamePause = false;
int      g_nLife = INIT_PLAYER_LIFE;
int      g_nHeroWeaponType = 3/*HERO_HAND_GRENADE_BULLET*/;
bool     g_bSubLife = false;
bool	 g_CanKillEnemy = false;
cocos2d::CCArray  *g_arrBombEffect = NULL;
//int      g_nSwitch = 0;

int		 g_bStoreFromGame = false;

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
int VIRT_WIDTH = 2048;//1500/*768*/;
int VIRT_HEIGHT = 1536;//1000/*576*/;

int      g_arrBulletsCount = 0;
int      g_nGameCoins = 0;

bool     g_bTutorial = false;
bool	g_SaveTutorial = true;
bool	g_TestTutorial = false;

int      g_nHeroSkinIndex = 0;
bool      g_nHeroUnlocked[5] = {true, false, false, false, false};

bool        g_nMissionsUnlocked[5] = {true, false, false, false, false};

int      g_nSolutionsNumber = 3;
int      g_nSkipsNumber = 3;


int      g_nBulletInfo[200/*LEVEL_COUNT*/][6] = {
	{0, 0,  4,  0,  0,  0},
	{0, 0,  5,  0,  0,  0},
	{0, 0,  5,  0,  0,  0},
	{0, 0,  8,  0,  0,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  5,  0,  0,  0},
	{0, 0,  9,  0,  0,  0},
	{0, 0,  6,  0,  0,  0},
	{0, 0,  11,  0,  0,  0},
	{0, 0,  5,  0,  0,  0}, 
	{0, 0,  6,  0,  0,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  6,  0,  0,  0},
	{0, 0,  6,  0,  0,  0},
	{0, 0,  20,  0,  0,  0},
	{0, 0,  8,  0,  0,  0},
	{0, 0,  6,  0,  0,  0},
	{0, 0,  0,  4,  0,  0},
	{0, 0,  0,  4,  0,  0}, 
	{0, 0,  0,  9,  0,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  1,  4,  0,  0},
	{0, 0,  1,  4,  0,  0},
	{0, 0,  4,  4,  0,  0},
	{0, 0,  4,  3,  0,  0},
	{0, 0,  4,  2,  0,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  9,  0,  0,  0},
	{0, 0,  4,  3,  0,  0}, 
	{0, 0,  9,  0,  0,  0},
	{0, 0,  4,  2,  0,  0},
	{0, 0,  5,  3,  0,  0},
	{0, 0,  8,  0,  0,  0},
	{0, 0,  5,  3,  0,  0},
	{0, 0,  4,  3,  0,  0},
	{0, 0,  8,  0,  0,  0},
	{0, 0,  4,  3,  0,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  9,  0,  0,  0}, 
	{0, 0, 12,  0,  0,  0},
	{0, 0,  9,  0,  0,  0},
	{0, 0,  5,  0,  0,  0},
	{0, 0,  8,  0,  0,  0},
	{0, 0,  6,  0,  0,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  6,  0,  0,  0},
	{0, 0,  0,  0,  5,  0},
	{0, 0,  0,  0,  8,  0},
	{0, 0,  0,  0,  9,  0}, 
	{0, 0,  4,  0,  3,  0},
	{0, 0,  0,  0,  8,  0},
	{0, 0,  2,  0,  4,  0},
	{0, 0,  4,  0,  4,  0},
	{0, 0,  0,  0,  5,  0},
	{0, 0,  3,  0,  4,  0},
	{0, 0,  1,  0,  4,  0},
	{0, 0,  5,  0,  4,  0},
	{0, 0,  0,  0,  5,  0},
	{0, 0,  7,  0,  0,  0}, 
	{0, 0,  3,  0,  3,  0},
	{0, 0,  5,  0,  0,  0},
	{0, 0,  0,  0,  5,  0},
	{0, 0,  0,  2,  4,  0},
	{0, 0,  4,  2,  0,  0},
	{0, 0,  8,  0,  0,  0},
	{0, 0,  3,  0,  4,  0},
	{0, 0,  0,  0,  6,  0},
	{0, 0,  2,  0,  2,  0},
	{0, 0,  6,  0,  0,  0}, 
	{0, 0,  4,  1,  0,  0},
	{0, 0,  4,  0,  5,  0},
	{0, 0,  3,  0,  3,  0},
	{0, 0,  4,  0,  4,  0},
	{0, 0,  3,  0,  5,  0},
	{0, 0,  6,  0,  5,  0},
	{0, 0,  4,  0,  4,  0},
	{0, 0,  4,  0,  6,  0},
	{0, 0,  2,  0,  4,  0},
	{0, 0,  5,  0,  4,  0},

	{0, 0,  2,  2,  4,  0},//Mission 3
	{0, 0,  4,  0,  3,  0},
	{0, 0,  4,  0,  2,  0},
	{0, 0,  3,  0,  4,  0},
	{3, 0,  0,  0,  0,  0},
	{5, 0,  0,  0,  0,  0},
	{6, 0,  0,  0,  0,  0},
	{5, 0,  5,  0,  0,  0},
	{7, 0,  0,  0,  0,  0},
	{3, 0,  2,  0,  0,  0},
	{4, 0,  0,  0,  0,  0}, 
	{0, 5,  0,  0,  0,  0},
	{4, 2,  3,  0,  0,  0},
	{0, 2,  0,  0,  4,  0},
	{0, 3,  2,  0,  0,  0},
	{0, 4,  3,  0,  0,  0},
	{2, 4,  0,  0,  0,  0},
	{0, 0,  0,  0,  0,  3},
	{0, 3,  0,  0,  0,  1},
	{0, 0,  3,  0,  0,  2},
	{0, 0,  2,  0,  0,  3}, 
	{0, 0,  0,  0,  0,  3},
	{0, 0,  1,  0,  0,  3},
	{0, 0,  6,  0,  0,  3},
	{0, 0,  4,  0,  0,  2},
	{0, 3,  0,  0,  4,  2},
	{0, 0,  0,  0,  4,  2},
	{0, 0,  3,  0,  0,  3},
	{0, 5,  0,  0,  3,  4},
	{0, 6,  0,  0,  0,  3},
	{0, 0,  6,  0,  2,  3}, 
	{0, 0,  7,  0,  0,  2},
	{3, 3,  0,  0,  0,  0},
	{0, 4,  0,  0,  0,  1},
	{0, 0,  5,  0,  0,  0},
	{1, 3,  0,  0,  0,  0},
	{0, 4,  4,  0,  0,  0},
	{0, 0,  5,  0,  0,  2},
	{0, 0,  6,  0,  0,  0},
	{0, 4,  0,  0,  0,  2},
	//1:Grenade,2:Knife,3:Pistol,4:Uzi,5:Arrow,6:Bomb
	{0, 5,  1,  0,  0,  0},  //Mission 4
	{4, 0,  0,  0,  0,  0},
	{0, 5,  1,  0,  0,  0},
	{3, 0,  4,  0,  0,  0},
	{3, 0,  3,  0,  0,  0},
	{2, 4,  2,  0,  0,  0},
	{0, 0,  4,  0,  3,  0},
	{5, 5,  0,  0,  0,  0},
	{3, 0,  3,  0,  0,  0},
	{0, 0,  3,  0,  5,  0}, 
	{0, 3,  3,  0,  5,  0},
	{3, 0,  0,  0,  2,  0},
	{0, 0,  0,  0,  7,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  6,  0,  0,  0},
	{0, 0,  5,  5,  0,  0},
	{0, 0,  4,  0,  6,  0},
	{3, 0,  3,  0,  3,  0},
	{6, 0,  4,  0,  0,  0},
	{0, 5,  5,  0,  0,  0}, 
	{0, 0,  0,  0,  0,  6},
	{0, 0,  8,  0,  0,  0},
	{3, 0,  5,  0,  0,  0},
	{3, 3,  3,  0,  0,  0},
	{0, 3,  3,  0,  2,  0},
	{0, 0,  5,  0,  5,  0},
	{9, 0,  0,  0,  0,  0},
	{0, 4,  4,  0,  0,  0},
	{0, 0,  4,  0,  3,  0},
	{0, 0,  4,  0,  0,  0}, 
	{0, 3,  6,  0,  0,  0},
	{4, 0,  0,  0,  2,  0},
	{4, 0,  7,  0,  0,  0},
	{0, 0,  7,  0,  3,  0},
	{3, 3,  0,  0,  0,  0},
	{1, 0,  4,  0,  0,  0},
	{0, 4,  6,  0,  0,  0},
	{0, 0,  4,  0,  3,  0},
	{0, 0,  6,  0,  3,  0},
	{3, 5,  0,  0,  0,  0},

	{0, 0,  6,  0,  0,  0},  //Mission 5
	{2, 5,  1,  0,  0,  0},
	{0, 7,  1,  0,  0,  0},
	{6, 0,  1,  0,  0,  0},
	{0, 4,  8,  0,  0,  0},
	{0, 7,  1,  0,  0,  0},
	{0, 11, 1,  0,  0,  0},
	{0, 0,  1,  5,  0,  0},
	{0, 0,  7,  0,  0,  0},
	{0, 0,  7,  0,  2,  0},
	{0, 4,  5,  0,  0,  0}, 
	{4, 5,  1,  0,  0,  0},
	{0, 0,  9,  0,  0,  0},
	{1, 0,  0,  0,  3,  0},
	{0, 0,  4,  0,  0,  4},
	{0, 0,  4,  2,  0,  0},
	{0, 3,  4,  0,  0,  3},
	{0, 0,  9,  0,  0,  0},
	{0, 0,  10, 0,  0,  0},
	{0, 6,  0,  0,  3,  0},
	{4, 0,  5,  0,  0,  0}, 
	{0, 0,  0,  0,  4,  0},
	{3, 0,  5,  0,  1,  0},
	{2, 0,  5,  0,  0,  0},
	{0, 0,  4,  0,  0,  1},
	{0, 0,  0,  4,  0,  2},
	{0, 0,  3,  0,  0,  3},
	{0, 5,  0,  0,  0,  2},
	{3, 0,  7,  0,  0,  0},
	{0, 4,  0,  0,  5,  0},
	{0, 6,  9,  0,  0,  0}, 
	{0, 2,  1,  0,  5,  0},
	{0, 3,  4,  0,  0,  2},
	{0, 0,  11, 0,  0,  0},
	{0, 4,  3,  0,  1,  0},
	{0, 3,  2,  0,  3,  0},
	{0, 0,  0,  0,  0,  5},
	{0, 3,  7,  0,  0,  0},
	{2, 3,  2,  0,  3,  0},
	{0, 3,  4,  0,  0,  2},
};

int UI_USER_INTERFACE_IDIOM()
{
	return UIUserInterfaceIdiomPad;
}

CCArray* componentsSeparatedByString( CCString* srcStr,const char*sepStr )
{
	CCArray* arr = CCArray::create();
	std::string src = srcStr->m_sString;
	std::string sep(sepStr);
	std::string subStr;
	int n1 = 0;
	int n2 = src.find(sep,n1);
	while ( n2 > 0 )
	{
		subStr = src.substr(n1,n2-n1);
		arr->addObject( CCString::create( subStr ) );
		n1 = n2+1;
		n2 = src.find(sep,n1);
	}
	n2 = src.length();
	subStr = src.substr(n1,n2-n1);
	arr->addObject( CCString::create( subStr ) );

	return arr;
}

void animateFocusNode(CCNode* menuItem,float m_fOrginalScale)
{
	CCScaleTo* movetozero = CCScaleTo::create(0.1f,1.2f * m_fOrginalScale);
	CCEaseBackOut* ease = CCEaseBackOut::create(movetozero);
	CCScaleTo *movetozero1 = CCScaleTo::create(0.1f,1.15f * m_fOrginalScale);
	CCEaseBackOut* ease1 = CCEaseBackOut::create(movetozero1);
	CCScaleTo* movetozero2 = CCScaleTo::create(0.1f,1.2f * m_fOrginalScale);
	CCEaseBackOut* ease2 = CCEaseBackOut::create(movetozero2);
	CCFiniteTimeAction* sequence = CCSequence::create(ease, ease1, ease2, NULL);
	menuItem->runAction(sequence);	
}

void animateFocusLoseNode(CCNode* menuItem,float m_fOrginalScale)
{
	CCActionInterval *movetozero = CCScaleTo::create(0.1f,1.0f * m_fOrginalScale);
	CCActionInterval *ease = CCEaseBackOut::create(movetozero);
	CCActionInterval *movetozero1 = CCScaleTo::create(0.1f,1.05 * m_fOrginalScale);
	CCActionInterval *ease1 = CCEaseBackOut::create(movetozero1);
	CCActionInterval *movetozero2 = CCScaleTo::create(0.1f,1.0 * m_fOrginalScale);
	CCActionInterval *ease2 = CCEaseBackOut::create(movetozero2);
	CCActionInterval *movetozero3 = CCScaleTo::create(0.1f,1.0f * m_fOrginalScale);
	CCActionInterval *ease3 = CCEaseBackOut::create(movetozero3);
	CCFiniteTimeAction *sequence = CCSequence::create(ease,ease1, ease2, ease3, NULL);
	menuItem->runAction(sequence);
}

bool isBarrel(LHSprite* spr)
{
	if ( spr->getTag() != 0 )
		return false;

	int fID = spr->getUniqueName()->m_sString.find("barrel");
	if ( fID >= 0 )
		return true;
	return false;
}

bool isHeroFriend(LHSprite* spr)
{
	int fID = spr->getUniqueName()->m_sString.find("hero_friend");
	if ( fID >= 0 )
		return true;
	return false;
}

float watchAngle(CCPoint ptStart, CCPoint ptEnd)
{
	//½ë¼°Ëºµá¹¢À¾ ´¸¸ÜÉã¼°Ë¾ ¸Æµ¹¸ó²÷ °¢
	CCPoint ppp = CCPoint(ptEnd.x - ptStart.x,ptEnd.y - ptStart.y);
	double angle = atan((double)ppp.y / (double)ppp.x) * 180 / 3.1415927;

	if (ppp.x < 0) angle += 180;
	if (ppp.x == 0 && ppp.y > 0) angle = 90;
	if (ppp.x == 0 && ppp.y < 0) angle = 270;
	if (ppp.x > 0 && ppp.y<0) angle +=360;
	if (ppp.x == 0 && ppp.y == 0) angle = 0;
	return angle * 3.1415927 / 180;
}

void controlScale(CCNode *node)
{
	node->setScaleX( 1024 / 768 );
	node->setScaleY( 768 / 576 );
}

float iDevPixelX(float x)
{
	return x * VIRT_WIDTH / 1024.0f;
}

float iDevPixelY(float y)
{
	return y * VIRT_HEIGHT / 768.0f;
}

float iDevSize(float s)
{
	float s1 = sqrt( float(VIRT_WIDTH * VIRT_WIDTH + VIRT_HEIGHT * VIRT_HEIGHT) );
	float s2 = sqrt( float(1024 * 1024 + 768 * 768) );
	return s1 / s2 * s;
}

float iDevChildPixelX(float x)
{
	return sqrt(iDevPixelX(x));
}
float iDevChildPixelY(float y)
{
	return sqrt(iDevPixelY(y));
}
float iDevChildSize(float s)
{
	return sqrt(iDevSize(s));
}

#define RAND_LIMIT 32767
float RandomFloat(float lo, float hi)
{
	float r = (float)(std::rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

