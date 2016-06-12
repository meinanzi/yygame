#ifndef _GoldenFlowerGAME_PLAYER_
#define _GoldenFlowerGAME_PLAYER_

#include "cocos2d.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace ui;
using namespace std;
using namespace HN;

namespace goldenflower
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

		static PlayerUI* create(LLONG userId, BYTE seatNo);

		PlayerUI(LLONG userId, BYTE seatNo);
		virtual ~PlayerUI();

	public:
		virtual bool init();

		GameTableUI* _TableUI; 

		void setTableUI(GameTableUI* tableUI);

		void setName(const char* name);
		std::string getName() const;
		std::string getHeadImage() const;

		void setSex(bool men,BYTE logicSeatNo);

		void setChip(LLONG chip);

		void setStatus(const char* status,int value);

		void setHandCard(int index, BYTE value);
		void changeHandCardValue(Node* pNode,int index, BYTE value);

		void note(LLONG money);

		void profit(LLONG money);

		void playAction(Action action);

		void setHandCardPosition();

		// show handcard 
		void lookHandCard();

		// lose game 
		void lose();

		void hideHandCard(bool setVisible);

		void setHandCardBroken();

		void startWait(int time, int total);

		void stopWait();

		void playWin(int type);

		void stopPlayWin();

		void setCardPosition(Vec2 position);

		PokerCard* getHandCard(int index);

		Vec2 getCardPosition();

		HNSprite* getHeadSp();

		void setViewSeatNo(BYTE seatNo);

		LLONG getUserID();

		bool isPlaying();

		bool isQuit();

		void enterGame();

	protected:
		static const int HAND_CARD_COUNT = 3;
		bool onTouchBegan(Touch *touch, Event *unused_event);
		void onTouchEnded(Touch *touch, Event *unused_event);
		void resetName(cocos2d::Node* pNode);

	private:
		HNSprite*       _bg;
		HNSprite*		_chipBackground;
		HNSprite*       _headSp;
		Text*           _nameText;
		Text*           _chipText;
		Text*           _statusText;
		std::string     _name;
		std::string     _headImage;
		LLONG           _chip;
		LLONG           _userId;
		ProgressTimer*  _progressTimer;
		float           _leftTime;
		float           _totalTime;
		BYTE			_seatNo;
		bool            _isPlaying;
		bool			_isQuit;
		void onWaitTimer(float delta);
	public:
		PokerCard*      _handCard[HAND_CARD_COUNT];
	};
}




#endif // !_GAME_PLAYER_
