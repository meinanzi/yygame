#ifndef __TRACTORGameResult_h__
#define __TRACTORGameResult_h__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "TRACTORGameClientMessage.h"

namespace TRACTOR
{
	class GameResulte : public HNLayer
	{
	public:
		static GameResulte* create(const std::vector<TGameResult>& results , bool isWin_me);
		virtual bool init() override;
		virtual void onExit() override;
		void setGameResulteData(const std::vector<TGameResult>& results);
		GameResulte(const std::vector<TGameResult>& results , bool isWin_me);
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);
		~GameResulte();

	private:
		void selectedItemEventCALLBACK(cocos2d::Ref *pSender, ui::ListView::EventType type);
		void updateTimer(float dt);
		ui::Widget* _widget;
		ui::ListView* _listView;
		ui::ImageView* _viewItem;
		std::vector<TGameResult> _results;
		bool _isWin_ME;
		INT _timeNumber;
		ui::TextAtlas* _timer;
	};
}

#endif // __TRACTORGameResult_h__