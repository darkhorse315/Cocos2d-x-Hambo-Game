#include "MyTutorial.h"
#include "LevelHelper/LevelHelperLoader.h"
#include "Constants.h"


MyTutorial::MyTutorial( )
{
	m_tutArray = new CCArray();
	m_tutArray_Body = new CCArray();
}

MyTutorial::~MyTutorial( )
{
	m_tutArray_Body->removeAllObjects();
	m_tutArray->removeAllObjects();
	delete m_tutArray;
	delete m_tutArray_Body;
}

void MyTutorial::readFromFile( const char* filePath )
{
	const char *fPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(filePath);


	unsigned long nSize = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fPath, "rb", &nSize);

	char *chs = (char*)pBuffer + 4;

	CCString *totalStr = CCString::create(chs);
	CCArray* tutArr = componentsSeparatedByString(totalStr,"|");

	m_tutArray->removeAllObjects();
	m_tutArray_Body->removeAllObjects();
	for ( unsigned int i = 0; i < tutArr->count(); i++ )
	{
		CCArray* arr = componentsSeparatedByString( ((CCString*)tutArr->objectAtIndex(i)),"," );
		
		TUTORIAL *tut = new TUTORIAL;
		if ( arr->count() > 0 ) tut->m_Time = ((CCString*)arr->objectAtIndex(0))->floatValue();
		if ( arr->count() > 1 ) tut->m_TutorialType = (TUTORIAL_TYPE)((CCString*)arr->objectAtIndex(1))->intValue();
		if ( tut->m_TutorialType == BODYPOSSET )
		{
			if ( arr->count() > 2 ) 
			{
				tut->m_BodyPosDict = new CCDictionary;
				CCString* str; CCObject* obj;
				CCArray* strArr = componentsSeparatedByString((CCString*)arr->objectAtIndex(2),"*");
				CCARRAY_FOREACH(strArr,obj)
				{
					str = (CCString*)obj;
					CCArray* chArr = componentsSeparatedByString(str,";");
					if ( chArr->count() > 3 )
					{
						int ind = ((CCString*)chArr->objectAtIndex(0))->intValue();
						float x = ((CCString*)chArr->objectAtIndex(1))->floatValue();
						float y = ((CCString*)chArr->objectAtIndex(2))->floatValue();
						float angle = ((CCString*)chArr->objectAtIndex(3))->floatValue();
						tut->m_BodyPosDict->setObject( CCString::createWithFormat("{%f,%f}@%f",x,y,angle),ind );
					}
				}
			}
			//ËË·ç ËØ³õ ¼³¸ó²÷ °û´Ýµá °¡¼ê³Þ.
			if ( m_tutArray_Body->count() > 0 )
			{
				TUTORIAL *oldtut = (TUTORIAL*)m_tutArray_Body->lastObject();
				CCArray *keys = oldtut->m_BodyPosDict->allKeys();

				CCObject* obj;
				CCARRAY_FOREACH(keys,obj)
				{
					CCInteger* ii = (CCInteger*)obj;
					const CCString *ss = tut->m_BodyPosDict->valueForKey(ii->getValue());
					if ( ss->m_sString == "" )
						tut->m_BodyPosDict->setObject( (CCObject*)oldtut->m_BodyPosDict->valueForKey(ii->getValue()),ii->getValue() );
				}
			}
			m_tutArray_Body->addObject(tut);
		}
		else
		{
			if ( arr->count() > 2 ) tut->m_PrevTouchPos.x = ((CCString*)arr->objectAtIndex(2))->floatValue() * VIRT_WIDTH / 768;
			if ( arr->count() > 3 ) tut->m_PrevTouchPos.y = ((CCString*)arr->objectAtIndex(3))->floatValue() * VIRT_HEIGHT / 576;
			if ( arr->count() > 4 ) tut->m_TouchPos.x = ((CCString*)arr->objectAtIndex(4))->floatValue() * VIRT_WIDTH / 768;
			if ( arr->count() > 5 ) tut->m_TouchPos.y = ((CCString*)arr->objectAtIndex(5))->floatValue() * VIRT_HEIGHT / 576;
			if ( arr->count() > 6 ) tut->m_WeaponType = ((CCString*)arr->objectAtIndex(6))->intValue();
			m_tutArray->addObject(tut);
		}
	}

	CC_SAFE_DELETE_ARRAY(pBuffer);
}

void MyTutorial::saveToFile( const char* filePath )
{
	if ( !g_SaveTutorial || g_bTutorial ) return;

	CCObject *obj;
	CCString *totalStr = CCString::create("");
	CCARRAY_FOREACH(m_tutArray,obj)
	{
		TUTORIAL* tut = (TUTORIAL*)obj;
		if ( tut->m_TutorialType == BODYDELETE )
		{
		}
		CCString *memStr;
		if ( tut->m_TutorialType != BODYPOSSET )
		{
			memStr = CCString::createWithFormat("%f,%d,%.2f,%.2f,%.2f,%.2f,%d|",
				tut->m_Time,tut->m_TutorialType,tut->m_PrevTouchPos.x,tut->m_PrevTouchPos.y,tut->m_TouchPos.x,tut->m_TouchPos.y,tut->m_WeaponType);
		}
		else
		{
			CCArray* memArr = tut->m_BodyPosDict->allKeys();
			CCObject* obj;
			memStr = CCString::createWithFormat("%f,%d,",tut->m_Time,tut->m_TutorialType);
			CCARRAY_FOREACH(memArr,obj)
			{
				CCInteger* sprInd = (CCInteger*)obj;
				const CCString* ptStr = tut->m_BodyPosDict->valueForKey(sprInd->getValue());
				CCArray* paramArr = componentsSeparatedByString((CCString*)ptStr,"@");
				if ( paramArr->count() > 1 )
				{
					CCPoint pt = LHPointFromString( ((CCString*)paramArr->objectAtIndex(0))->getCString() );
					float angle = ((CCString*)paramArr->objectAtIndex(1))->floatValue();
					memStr->m_sString += CCString::createWithFormat("%d;%.2f;%.2f;%.2f*",sprInd->getValue(),pt.x,pt.y,angle)->getCString();
				}
			}
			memStr->m_sString += "|";
		}
		
		totalStr->m_sString += memStr->getCString();
	}

	CCString *fPath = CCString::create(CCFileUtils::sharedFileUtils()->getWriteablePath());
	fPath->m_sString += filePath;
	FILE *fp = fopen(fPath->getCString(),"wt");
	int len = totalStr->length();
	fwrite(&len,sizeof(int),1,fp);
	fwrite(totalStr->getCString(),totalStr->length(),1,fp);
	fclose(fp);
}

void MyTutorial::addTutorial(float time,	TUTORIAL_TYPE tutType,	CCPoint	prevtouchPos,	CCPoint	touchPos,int weapontype,CCDictionary* bodyPosDict)
{
	if ( g_SaveTutorial && !g_bTutorial )
	{
		TUTORIAL *tut = new TUTORIAL;
		tut->m_Time = time;
		tut->m_TutorialType = tutType;
		tut->m_TouchPos = touchPos;
		tut->m_PrevTouchPos = prevtouchPos;
		tut->m_WeaponType = weapontype;
		tut->m_BodyPosDict = bodyPosDict;

		int i;
		for ( i = m_tutArray->count() - 1; i >= 0; i-- )
		{
			TUTORIAL *t1 = (TUTORIAL*)m_tutArray->objectAtIndex(i);
			if ( tut->m_Time > t1->m_Time )
				break;
		}
		m_tutArray->insertObject( tut,i+1 );
	}
}