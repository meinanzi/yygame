#ifndef __HN_HNDialogBase_H__
#define __HN_HNDialogBase_H__

#include "Base/HNLayer.h"
#include "cocos2d.h"

namespace HN
{
	class HNDialogBase: public HNLayer
	{
	public:
		virtual void show();
		virtual void hide();
		virtual void setShowPoint(const cocos2d::Vec2& showPoint){ _showPoint = showPoint;}
		virtual void setHidePoint(const cocos2d::Vec2& hidePoint){ _hidePoint = hidePoint;}
		virtual void locateMid(cocos2d::Node* pNode);
	public:
		HNDialogBase();
		virtual ~HNDialogBase();
		bool init(cocos2d::Node* parent);
	private:
		cocos2d::Node* _parent;
		cocos2d::Vec2 _showPoint;
		cocos2d::Vec2 _hidePoint;
	};
}

#endif