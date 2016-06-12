#ifndef GameLists_h__
#define GameLists_h__


#include "HNNetExport.h"
#include "HNUIExport.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameLists : public HNLayer
{
public:
	typedef std::function<void ()> EnterGameCallBack;
	EnterGameCallBack onEnterGameCallBack;
	EnterGameCallBack onRunActionCallBack;

private:
	PageView*					_pageViewGames;			// 游戏列表
	Button*						_currentSelectedGame;	// 列表按钮

	std::vector<Sprite*>		_pagesPrompt;
	INT							_currentPageIdx;
	bool						_isTouch;

public:
	GameLists();
	virtual ~GameLists();

	CREATE_FUNC(GameLists);

public:
	virtual bool init() override;

	virtual void onEnter() override;

	virtual void onExit() override;

	// 界面回调接口
public:
	//pageView拖动回调函数
	void pageViewMoveCallBack(Ref* pSender, PageView::EventType type);
	//列表按钮点击回调函数
	void enterRoomEventCallBack(Ref* pSender, Widget::TouchEventType type);

	// 功能接口
private:
	// 创建游戏列表
	void createGameList();
	// 创建游戏
	void createGamePage(std::vector<ComNameInfo*> games);
	// 创建游戏元素
	Node* createGameItem(ComNameInfo* game);

public:
	//更新游戏人数
	void updateGamePeopleCount(UINT Id, UINT count);
};
#endif // GameLists_h__
