
#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"

class logo : public ScaleLayer
{
public:
	~logo();
	bool init();
	static CCScene* scene();
	CREATE_FUNC(logo);

private:
	void createBackground();
	void logoTimer(float dt);
};
