#ifndef __LandlordExitPrompt_h__
#define __LandlordExitPrompt_h__

#include "HNNetExport.h"
#include "cocos2d.h"

namespace landlord
{
	class GameTableUI;

	class ExitPromptLayer : public HN::HNLayer
	{
	public:
		CREATE_FUNC(ExitPromptLayer);

	public:
		virtual bool init() override;

	public:
		void setExitCallBack(const std::function<void ()>& onExit);

	public:
		void onExitCallBack(Ref* pSender, Widget::TouchEventType type);
		void onCancelCallBack(Ref* pSender, Widget::TouchEventType type);

	protected:
		ExitPromptLayer();
		~ExitPromptLayer();

	private:
		std::function<void ()> _onExit;
	};
}

#endif // __LandlordExitPrompt_h__
