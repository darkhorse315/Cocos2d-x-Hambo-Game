
#include "cocos2d.h"
#include "BaseLayer/ScaleLayer.h"

class HelpLabel : public ScaleLayer
{
public:
	CREATE_FUNC(HelpLabel);

	void setLabel( const char* helpStr, int fontSize );
};