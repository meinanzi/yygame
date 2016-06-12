#ifndef __Landlord_Action_Board_H__
#define __Landlord_Action_Board_H__

#include "LandlordPokerCard.h"

#include "HNNetExport.h"

namespace landlord
{
	class CardListBoard: public HN::HNLayer
	{
	public:
		static CardListBoard* create(bool isMySelf, BYTE seatNo);
		virtual bool init(bool isMySelf, BYTE seatNo);
		//add new.
		void addCardOneByOne(const std::vector<BYTE>& values);
		void addCardAllOnce(const std::vector<BYTE>& values, bool isUp);
		void removeCardAllOnce(const std::vector<BYTE>& values);

		//sort card value, record in card list.
		void setCallFunction(std::function<void(cocos2d::Node*)> callfunc);
		void getUpCards(std::vector<BYTE>* upCards);
		void getTouchedCards(std::vector<BYTE>* outCards);
		BYTE getSeatNo() const { return _seatNo;}
		void clear();
		void downCards();
		void upCards(const std::vector<BYTE>& cards);

	protected:
		CardListBoard();
		virtual ~CardListBoard();

		//add new.
		void addCard(BYTE value, bool isUp);
		void removeCard(BYTE value);
		void resizeCardList();
		bool touchCheck(const Vec2& pos, bool isAhead = true);
		void reorderCard();
		void removeTouchedTag();
		void upCard(PokerCard* card);
		void downCard(PokerCard* card, float delay);
		void changeUpDown();
		bool onTouchBegan(Touch* touch, Event *event);
		void onTouchMoved(Touch* touch, Event *event);
		void onTouchEnded(Touch* touch, Event *event);
		void scheduleRun(float delta);

	private:
		//未处理列表
		std::vector<BYTE> _undoList;
		std::vector<PokerCard*> _cardsPtr;
		bool _isMySelf;
		BYTE _seatNo;
		std::function<void (cocos2d::Node*)> _callfunc;
		Vec2	_startMovePoint;
		Vec2	_midMovePoint;
		Vec2	_currentMovePoint;
	};
}




#endif