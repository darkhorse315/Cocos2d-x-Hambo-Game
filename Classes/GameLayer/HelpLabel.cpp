#include "HelpLabel.h"
#include "ccframework/ResourceManager.h"

void HelpLabel::setLabel( const char* helpStr, int fontSize )
{
	_setZOrder(10);
	CCSprite *sp = ResourceManager::sharedResourceManager()->getSpriteWithName("game/background/helplabel");
	CCLabelTTF* label = CCLabelTTF::create( helpStr, "Arial", fontSize );
	sp->setOpacity(100);
	sp->setScaleX( (label->boundingBox().size.width + 30) / sp->boundingBox().size.width );
	sp->setScaleY( (label->boundingBox().size.height + 30) / sp->boundingBox().size.height );
	addChild(sp);
	addChild(label);
}
