#ifndef __GoldenFlowerGame_Dashboard_H__
#define __GoldenFlowerGame_Dashboard_H__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "GoldenFlowerMessageHead.h"

namespace goldenflower
{
	class GameTableUI;

	class Dashboard: public HNLayer
	{
	public:
		static Dashboard* create(GameTableUI* parent);
		virtual bool init(GameTableUI* parent);
		void setLookVisible(bool visible);
		void setFollowVisible(bool visible);
		void setAddVisible(bool visible);
		void setAddBetVisible(bool visible);
		void setOpenVisible(bool visible);
		void setGiveUpVisible(bool visible);
		void setAddRange(LLONG min, LLONG max);
		LLONG getAddValue();
		void show();
		void hide();
	protected:
		Dashboard();
		~Dashboard();
		void menClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchtype);
		void sliderChangedCallback(cocos2d::Ref* pSender,cocos2d::ui::Slider::EventType type);
		void setAddValue(LLONG addValue);
		void setButtonEnabled(ui::Button* pButton, bool enabled);
	private:
		GameTableUI* _tableUI;
		Node* _dashboard_node;
		ui::Button* _lookButton;
		ui::Button* _compareButton;
		ui::Button* _followButton;
		ui::Button* _giveUpButton;
		ui::Button* _addButton;
		ui::Button* _betButton1;
		ui::Button* _betButton2;
		ui::Button* _betButton3;
		ui::Button* _betButton4;
		ui::Slider* _slider;
		ui::TextAtlas* _noteLabel;
		ui::ImageView* _sliderBg;
		LLONG _addValue;
		LLONG _min;
		LLONG _max;
		int _visibleWidth, _visibleHeight;
	public:
		bool _CanAddOfAll[E_CHOUMA_COUNT];
		Vec2 _BetBegPosition;
		ui::ImageView* _ButtonBeg;
	};
}



#endif