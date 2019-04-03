//
//  ResourceManager.m
//  glideGame
//
//  Created by KCU on 4/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ResourceManager.h"
#include "Constants.h"

ResourceManager* ResourceManager::_sharedResource = NULL;

ResourceManager* ResourceManager::sharedResourceManager()
{
	if (!_sharedResource) 
	{
		_sharedResource = new ResourceManager();
	}
	
	return _sharedResource;
}

ResourceManager::ResourceManager()
{
	_sharedResource = this;
}

ResourceManager::~ResourceManager()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
	
	for (int i = 0; i < TEXTURE_COUNT; i ++)
	{
		delete textures[i];
	}	
}

void ResourceManager::loadLoadingData()
{	
}

/*void ResourceManager:: print_memory_size { 
	mach_port_t host_port; 
	mach_msg_type_number_t host_size; 
	vm_size_t pagesize; 
    
	host_port = mach_host_self(); 
	host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t); 
	host_page_size(host_port, &pagesize);         
	
	vm_statistics_data_t vm_stat; 
	
	if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) 
		NSLog(@"Failed to fetch vm statistics"); 
	
	natural_t mem_used = (vm_stat.active_count + 
						  vm_stat.inactive_count + 
						  vm_stat.wire_count) * pagesize; 
	natural_t mem_free = vm_stat.free_count * pagesize; 
	natural_t mem_total = mem_used + mem_free; 
	NSLog(@"used: %d free: %d total: %d", mem_used, mem_free, mem_total); 
} */

const char* ResourceManager::makeFileName(const char*name,const char* ext)
{
	if ( CCApplication::sharedApplication()->getTargetPlatform() != kTargetIphone )
		return CCString::createWithFormat("%s%s%s.%s",g_resPath.getCString(), name,"-ipad", ext)->getCString();
	
	return CCString::createWithFormat("%s%s.%s", g_resPath.getCString(),name, ext)->getCString();
}

void ResourceManager:: loadData(const char* strName)
{	
	CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	const char*	strFile = makeFileName(strName,"plist");
	frameCache->addSpriteFramesWithFile(strFile);

//	NSLog(@"Resource Manager completed loading");
//	print_memory_size];
}

void ResourceManager:: unloadData(const char* strName)
{
	CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	const char*	strFile = makeFileName(strName,"plist");
	frameCache->removeSpriteFramesFromFile(strFile);
	
//	CCLOG(@"Resource Manager completed unloading");
//	print_memory_size];
}

CCSprite* ResourceManager::getTextureWithName(const char* textureName)
{
	const char*	strFile = makeFileName(textureName,"png");
	CCSprite*	sprite = CCSprite::createWithSpriteFrameName(strFile);

	return sprite;
}

CCSpriteFrame* ResourceManager::getSpriteFrameWithName(const char* frameName)
{
	CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	
	const char*	strFile = makeFileName(frameName,"png");
	CCSpriteFrame*	frame = frameCache->spriteFrameByName(strFile);

	return frame;
}

CCSprite* ResourceManager::getSpriteWithName(const char* strSpriteName)
{
	const char*	strFile;
	if ( strcmp(g_resPath.getCString(),"ipad_res") )
		strFile = CCString::createWithFormat("%s%s%s.png",g_resPath.getCString(),strSpriteName,"-ipad")->getCString();
	else
		strFile = CCString::createWithFormat("%s%s.png",g_resPath.getCString(),strSpriteName)->getCString();

	CCSprite*	sprite = CCSprite::create(strFile);

	return sprite;
}

CCSprite* ResourceManager::getTextureWithId(int textureId)
{
	return textures[textureId];
}
