#ifndef __TRACTORGameDashboard_h__
#define __TRACTORGameDashboard_h__


#include "HNUIExport.h"
#include "HNNetExport.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

namespace TRACTOR
{

	class GameTableUI;

	class Dashboard: public HNLayer
	{
	public:
		static Dashboard* create(GameTableUI* parent);
		virtual bool init(GameTableUI* parent);
		void setActionOption(int options);
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
		ui::Button* _lookButton;
		ui::Button* _compareButton;
		ui::Button* _followButton;
		ui::Button* _giveUpButton;
		ui::Button* _addButton;
		ui::Slider* _slider;
		ui::TextAtlas* _noteLabel;
		ui::ImageView* _sliderBg;
		LLONG _addValue;
		LLONG _min;
		LLONG _max;
	};
}

#endif // __TRACTORGameDashboard_h__
