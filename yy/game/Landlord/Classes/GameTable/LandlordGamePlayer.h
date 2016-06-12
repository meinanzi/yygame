#ifndef __Landlord_GAME_PLAYER_H__
#define __Landlord_GAME_PLAYER_H__

#include "cocos2d.h"
#include "HNNetExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace ui;
using namespace std;

namespace landlord
{
	class PokerCard;

	class PlayerUI: public HNLayer
	{
	public:
		static PlayerUI* create(LLONG userId);
		virtual bool init(LLONG userId);
	
	public:
		void setUserId(LLONG userId);
		void setUserName(const std::string& name);
		void setUserMoney(const std::string& money);
		void setUserCardCount(int count);
		void setAutoHead();
		void setHead(bool isBoy);
		void setGameHead(bool isBoy, bool isLord);
		void setEmptyHead();
		void showMoney(bool visible);
		void showCard(bool visible);
		void setTableUI(cocos2d::Node* tableUI);
		void showProfit(LLONG money);

	protected:
		PlayerUI();
		virtual ~PlayerUI();
	
	protected:
		bool onTouchBegan(Touch *touch, Event *unused_event);
		void onTouchEnded(Touch *touch, Event *unused_event);	
		void loadComponet(ui::Widget* widget);

	private:
		LLONG		_userId;
		LLONG		_money;
		INT			_cardCount;
		std::string _name;

	private:
		ui::ImageView*	_ivHead;
		ui::Text*		_textName;
		ui::Text*		_textMoney;
		ui::Text*		_textCardCount;
		cocos2d::Node*	_tableUI;
	};
}



#endif // !_GAME_PLAYER_
