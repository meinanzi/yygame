#ifndef __LayerManger_h__
#define __LayerManger_h__

#include "cocos2d.h"

USING_NS_CC;

class LayerManger
{
public:
	LayerManger();
	virtual ~LayerManger();

private:
	Vector<Node*> _childs;
};

#endif // __LayerManger_h__
