#ifndef __DZPokerGamePlayer_h__
#define __DZPokerGamePlayer_h__

#include "cocos2d.h"
#include "HNNetExport.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "DZPokerPokerCard.h"
#include "HNUIExport.h"

using namespace ui;
using namespace std;

namespace DZPoker
{
	class PlayerUI: public HN::HNLayer
	{
	public:
		/*创建玩家*/
		static PlayerUI* create(LLONG userId);
		/*设置名称*/
		void setName(const std::string& name);
		/*设置性别*/
		void setSex(bool men);
		/*设置筹码*/
		void setChip(LLONG chip);
		/*设置玩家状态*/
		void setStatus(const char* status,int value);
		/*设置玩家名字*/
		std::string getName() const;
		/*设置玩家收牌*/
		void setHandCard(int index, BYTE value);
		/*隐藏手牌*/
		void hideHandCard();
		/*倒计时*/
		void startWait(int time, int total);
		/*停止倒计时*/
		void stopWait();
		/*显示赢牌效果*/
		void playWin(int type);
		/*停止显示赢牌效果*/
		void stopPlayWin();
		/*初始化布局*/
		virtual bool init(LLONG userId);
		PlayerUI();
		virtual ~PlayerUI();
	protected:
		bool onTouchBegan(Touch *touch, Event *unused_event);
		void onTouchEnded(Touch *touch, Event *unused_event);
		void resetName(cocos2d::Node* pNode);
	private:
		BYTE _deskPosition;							//玩家的本地视图中的椅子号
	private:
		HNSprite* _bg;
		HNSprite* _headSp;
		Text* _nameText;
		Text* _chipText;
		std::string _name;
		LLONG _chip;
		PokerCard* _handCard[2];
		LLONG _userId;
		ProgressTimer* _progressTimer;
		void onWaitTimer(float delta);
		float _leftTime;
		float _totalTime;
	};

}

#endif // __DZPokerGamePlayer_h__
