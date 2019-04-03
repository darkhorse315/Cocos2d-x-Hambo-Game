
#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"
#include "cocos-ext.h"

USING_NS_CC;

class OptionLayer : public ScaleLayer
{
public:
	~OptionLayer();
	bool init();
	static CCScene* scene();
	CREATE_FUNC(OptionLayer);

private:
	void createBackground();
	void createButtons();

	void onClose( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onQuitOK( CCObject* sender );
	void onBack( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onResetGame( CCObject* sender, cocos2d::extension::CCControlEvent event );
	void onEffect( CCObject* sender );
	void onBGM( CCObject* sender );
	void onResetOK( CCObject* sender );
};