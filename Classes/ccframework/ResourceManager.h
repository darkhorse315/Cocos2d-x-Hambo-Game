#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "cocos2d.h"

USING_NS_CC;

enum TEXTURE_ID
{
	TEXTURE_COUNT = 1
};

class ResourceManager : public CCObject
{
	static ResourceManager *_sharedResource;
public:
	ResourceManager();
	~ResourceManager();
	CCSprite* textures[TEXTURE_COUNT];
	static ResourceManager* sharedResourceManager();
	void releaseResourceManager();


	void loadLoadingData() ;

	void loadData(const char* strName);

	void unloadData(const char*strName) ;

	CCSprite* getTextureWithName(const char* textureName);
	CCSprite* getSpriteWithName(const char* strSpriteName);
	CCSpriteFrame* getSpriteFrameWithName(const char* frameName);

	CCSprite* getTextureWithId(int textureId);
	const char* makeFileName(const char* name,const char* ext);
};

#endif