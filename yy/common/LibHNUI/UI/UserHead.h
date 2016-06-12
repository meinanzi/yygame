#ifndef __UserHead_h__
#define __UserHead_h__

#include "HNUIExport.h"
#include "cocos2d.h"

namespace HN
{

	class UserHead : public HNSprite
	{
	private:
		Sprite* _userHead;

	public:
		static UserHead* create(const std::string& frame);

		void setUserHead(const std::string& head);

	protected:
		bool initWithImage(const std::string& frame);

		bool onTouchBegan(Touch* touch, Event* event);

		void onTouchEnd(Touch* touch, Event* event);

	public:
		std::function<void(UserHead* sender, Vec2 touchVec)> onHeadClickEvent;

	private:
		UserHead();
		virtual ~UserHead();
	};
}


#endif // __UserHead_h__