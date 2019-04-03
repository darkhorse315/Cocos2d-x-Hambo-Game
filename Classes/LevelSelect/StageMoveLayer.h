//
//  StageMoveLayer.h
//  JarHead
//
//  Created by admin on 6/26/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"

class StageMoveLayer : public ScaleLayer {    
    StageSelectLayer *parentLayer;
public:
	~StageMoveLayer();
	bool init();
	static CCScene* scene();
	void onStage(CCObject* sender);

	void onBack( CCObject* sender );
	bool ccTouchBegan(CCTouch* touch, CCEvent *event);
	void ccTouchMoved(CCTouch *touch,CCEvent *event);

	CREATE_FUNC(StageMoveLayer);
};