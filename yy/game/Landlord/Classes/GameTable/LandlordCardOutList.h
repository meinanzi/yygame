#ifndef __Landlord_CARD_OUT_LIST_H__
#define __Landlord_CARD_OUT_LIST_H__

#include "HNNetExport.h"
#include "LandlordMessageHead.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace landlord
{
	class GameTableUI;

	//////////////////////////////////////////////////////////////////////////
	class CardOutList: public HN::HNLayer
	{
	public:
		static CardOutList* create();
		virtual bool init();
		void outCard(const std::vector<BYTE>& cards);
	protected:
		CardOutList();
		virtual ~CardOutList();
	};

	//////////////////////////////////////////////////////////////////////////
	
	class CancelAuto: public HNLayer
	{
	public:
		static CancelAuto* create(GameTableUI* tableUI);
		virtual bool init(GameTableUI* tableUI);
		void menuClickCallback(cocos2d::Ref* pRef, cocos2d::ui::Widget::TouchEventType type);
	protected:
		CancelAuto();
		virtual ~CancelAuto();
		bool onTouchBegan(Touch *touch, Event *unused_event);
	private:
		cocos2d::EventListener* _touchListener;
		GameTableUI* _tableUI;
	};

}

#endif