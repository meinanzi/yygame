#ifndef __Landlord_TOOLBAR_H__
#define __Landlord_TOOLBAR_H__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

namespace landlord
{
	class Toolbar : public HN::HNLayer
	{
	public:
		static Toolbar* create(const std::function<void(cocos2d::Node*)>& func);
		virtual bool init(const std::function<void(cocos2d::Node*)>& func);

		typedef std::function<void()> TalkMenuCallBack;
		TalkMenuCallBack	onSpeakeCallBack;

	public:
		bool isUp() const { return _isUp;}
		// 设置退出按钮回调函数
		void setExitCallBack(const std::function<void()>& onExitFunc);
		// 设置按钮回调函数
		void setSettingCallBack(const std::function<void()>& onSettingFunc);
		// 设置托管按钮回调函数
		void setAutoCallBack(const std::function<void()>& onAutoFunc);
	
	protected:
		void moveUp();
		void moveDown();

	protected:
		Toolbar();
		virtual ~Toolbar();

	public:
		void menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);

	private:
		bool _isUp;
		ui::ImageView* _iDropUp;
		ui::ImageView* _iDropDown;

		std::function<void(cocos2d::Node*)> _function;

		std::function<void()> _onExitFunc;
		std::function<void()> _onSettingFunc;
		std::function<void()> _onAutoFunc;
	};
}



#endif