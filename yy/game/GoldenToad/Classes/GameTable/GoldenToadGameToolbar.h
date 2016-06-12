#ifndef __HN_GoldenToad_TOOLBAR_H__
#define __HN_GoldenToad_TOOLBAR_H__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

namespace GoldenToad
{
	class GoldenToadToolbar : public HN::HNLayer
	{
	public:
		static GoldenToadToolbar* create(Layout* toolBarBG);
		virtual bool init(Layout* toolBarBG);


	public:
		bool isOut() const { return _isOut; }

	protected:
		void toolBarMove();

	protected:
		GoldenToadToolbar();
		virtual ~GoldenToadToolbar();

	public:
		void menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);


	private:
		bool			_isOut;
		Layout*			_toolBarBG;

		typedef std::function<void()> ExitCallBack;
		typedef std::function<void()> ChatCallBack;
		typedef std::function<void()> HelpCallBack;
		typedef std::function<void()> SetCallBack;
		typedef std::function<void(bool bLock, bool bNormal)> LockFishCallBack;
		typedef std::function<void(Button* autoBtn)> AutoFireCallBack;

	public:
		ExitCallBack				_onExit;
		ChatCallBack				_onChat;
		HelpCallBack				_onHelp;
		SetCallBack					_onSet;
		AutoFireCallBack			_onAutoFire;
		LockFishCallBack			_onLock;

	};
}



#endif