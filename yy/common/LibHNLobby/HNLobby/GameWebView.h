#ifndef GameWebView_h__
#define GameWebView_h__

#include "HNUIExport.h"
#include "ui/UIWebView.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace ui;
using namespace cocostudio;

class GameWebViewLayer : public HNLayer
{
private:
	LayerColor*															_layerColor;
	Text*																			_titleText;

public:
	CREATE_FUNC(GameWebViewLayer);

	GameWebViewLayer();
	virtual ~GameWebViewLayer();

public:
	virtual bool init() override;  
	void showWebView(const std::string& URL);
	void setTitle(const std::string& title);
	void setColor(Color3B color);
	void closeWebView();
private:
	void closeEventCallBack(Ref* pSender, Widget::TouchEventType type);
	void setURL(const std::string& URL);
};

#endif // Game_title_h__