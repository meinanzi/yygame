#ifndef __TRACTORGamePlayer_h__
#define __TRACTORGamePlayer_h__


#include "cocos2d.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "TRACTORGameClientMessage.h"

using namespace ui;
using namespace std;
using namespace HN;

namespace TRACTOR
{
	class PokerCard;
	class GameTableUI;

	class PlayerUI: public HNLayer
	{
	public:
		enum Action
		{
			eLookCard,
			eGiverUp,
			eNote,
			eCompareCard,
			eFollow
		};

		static PlayerUI* create(LLONG userId);

		PlayerUI(LLONG userId);
		virtual ~PlayerUI();

	public:
		virtual bool init();

		GameTableUI* _TableUI;

		void setTableUI(GameTableUI* tableUI);
		
		

	public:
		void setName(const char* name);
		std::string getName() const;

		void setChip(LLONG chip);

		void setStatus(const char* status,int value);

		void setHandCard(int index, BYTE value);

		void setSex(bool men);

		void note(LLONG money);

		void profit(LLONG money);

		void playAction(Action action);

		// show handcard 
		void lookHandCard();

		// lose game 
		void lose();

		void hideHandCard(bool setVisible);

		void startWait(int time, int total);

		void stopWait();

		void playWin(int type);

		void stopPlayWin();

	protected:
		static const int HAND_CARD_COUNT = 3;
		bool onTouchBegan(Touch *touch, Event *unused_event);
		void onTouchEnded(Touch *touch, Event *unused_event);
		void resetName(cocos2d::Node* pNode);

	private:
		HNSprite*       _bg;
		HNSprite*       _headSp;
		Text*           _nameText;
		Text*           _chipText;
		std::string     _name;
		LLONG           _chip;
		PokerCard*      _handCard[HAND_CARD_COUNT];
		LLONG           _userId;
		ProgressTimer*  _progressTimer;
		float           _leftTime;
		float           _totalTime;
		void onWaitTimer(float delta);
	};
}

#endif // __TRACTORGamePlayer_h__