//
//  HelloWorldLayer.h
//  towerGame
//
//  Created by KCU on 6/13/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//


// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "AppDelegate.h"
#include "BaseLayer/ScaleLayer.h"

USING_NS_CC;
// HelloWorld Layer
class LoadScene : ScaleLayer
{
private:
    WND_ID	m_nCurWnd;
    WND_ID  m_nNewWnd;
    int     m_nParam;
	long    m_tick;

public:
	static CCScene* scene(WND_ID wndCur,WND_ID wndNew,int param);

	LoadScene(WND_ID wndCur, WND_ID wndNew, int param);
	~LoadScene();
private:
	void changeWindow();
	void draw( float dt );	
};

// returns a Scene that contains the HelloWorld as the only child
