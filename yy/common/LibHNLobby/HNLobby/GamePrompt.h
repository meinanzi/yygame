#ifndef GamePrompt_h__
#define GamePrompt_h__

#include "HNUIExport.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace ui;
using namespace cocostudio;

class GamePromptLayer : public HNLayer
{
	Rect _userRect;

private:
	Text*			promptText;
	Button*         promptButtonSure1;
	Button*         promptButtonSure2;
	Button*         promptButtonCancel;
	Node*			promptLoader;

public:
	CREATE_FUNC(GamePromptLayer);

	GamePromptLayer();
	virtual ~GamePromptLayer();

public:
	virtual bool init() override;  

	void showPrompt(const std::string& prompt);

	void closePrompt();

	void setPromptCanSelect();

	void setCallBack(std::function<void ()> sure);

	void setCancelCallBack(std::function<void ()> cancel);

	void setVerticalScreenShow();

private:
	void menuEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void setPrompt(const std::string& prompt);

	void setPrompt(const std::string& prompt, const std::string& font, int size);

private:
	std::function<void ()> _sure;

	std::function<void ()> _cancel;

};

#endif // GamePrompt_h__
