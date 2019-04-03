
#include "cocos2d.h"

USING_NS_CC;

enum TUTORIAL_TYPE
{
	NONE,
	TOUCHBEGAN,
	TOUCHMOVE,
	TOUCHENDED,
	WEAPON,
	BODYPOSSET,
	BODYDELETE
};

class TUTORIAL:public CCObject
{
public:
	float			m_Time;
	TUTORIAL_TYPE	m_TutorialType;
	CCPoint			m_TouchPos;
	CCPoint			m_PrevTouchPos;
	int				m_WeaponType;
	CCDictionary*	m_BodyPosDict;

public:
	TUTORIAL()
	{
		m_Time = -1;
		m_TutorialType = NONE;
		m_PrevTouchPos = m_TouchPos = CCPoint(-1,-1);
		m_WeaponType = -1;
		m_BodyPosDict = NULL;
	};

	~TUTORIAL()
	{
		if ( m_BodyPosDict )
		{
			m_BodyPosDict->removeAllObjects();
			m_BodyPosDict->release();
		}
	}
};

class MyTutorial : public cocos2d::CCObject
{
public:
	MyTutorial();
	~MyTutorial();

	CCArray *m_tutArray;
	CCArray *m_tutArray_Body;

	void addTutorial(float time,	TUTORIAL_TYPE tutType,	CCPoint	prevtouchPos,	CCPoint	touchPos,int weapontype,CCDictionary* bodyPosDict = NULL);
	void readFromFile( const char* filePath );
	void saveToFile( const char* filePath );
};