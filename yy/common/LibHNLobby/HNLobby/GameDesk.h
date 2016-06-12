#ifndef GameDesk_h__
#define GameDesk_h__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "HNLogicExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <vector>

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

//桌子信息
typedef struct tagDeskInfo
{
	INT		deskID;               // 桌号
	INT		multiple;             // 倍率
	INT		goldMin;              // 最小携带金钱
	INT		goldMax;              // 最大携带金钱
	INT		peopleValue;          // 桌子当前人数
	INT		peopleMax;            // 桌子最大人数
	bool    isLocked;             // 是否锁桌
}DeskInfo;

class GameDesk : public HNLayer, public IHNRoomDeskLogic
{
public:
	typedef std::function<void ()> CloseCallBack;
	CloseCallBack				onCloseCallBack;

private:
	ComRoomInfo*				_roomInfo;
	PageView*					_pageViewDesks;			// 房间列表
	Button*						_currentSelectedDesk;	// 列表按钮
	INT							_deskNO;				// 桌子号
	
	// 绘制页临时数据
	std::vector<DeskInfo*>		_deskinfos;

	// 桌子列表所有数据
	std::vector<DeskInfo*>      _allDeskInfo;
	
	bool						_pageEven;				//是否整数页
	INT							_pageLen;				//计算出来的总页数
	bool						_canCreate;				//是否还需要创建页面

	std::vector<Sprite*>		_pagesPrompt;
	INT							_currentPageIdx;
	bool						_isTouch;

public:
	static GameDesk* createDesk(ComRoomInfo* roomInfo);

public:
	GameDesk();
	virtual ~GameDesk();

	bool initData(ComRoomInfo* roomInfo);
	virtual void onExit() override;

public:
	void closeDeskList();

	// 输入密码回调
	void onEnterPasswordCallback(const std::string& password);

public:
	// 创建牌桌列表pageView
	void createDeskList(ComRoomInfo* roomInfo);
	// 分页创建牌桌列表page
	void createDeskPage();
	// 创建牌桌页面数据
	void createDeskPageInfo(ComRoomInfo* roomInfo);
	// 创建牌桌列表page
	void createDeskListPage(std::vector<DeskInfo*> deskinfos);
	// 创建牌桌列表button
	Node* addDesk(DeskInfo* deskInfo);

public:
	// pageView拖动回调函数
	void pageViewMoveCallBack(Ref* pSender, PageView::EventType type);

	// 桌子是否锁定更新
	void updateLockDesk();

	// 进入游戏桌点击回调
	void enterTableEventCallBack(Ref* pSender, Widget::TouchEventType type);

public:
	virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) override;
	virtual void onRoomDeskLock(BYTE deskNo) override;
	virtual void onRoomDeskUnLock(BYTE deskNo) override;
	virtual void onRoomDeskUserCountChanged() override;

	//更新游戏人数
	void updateDeskPeopleCount(BYTE deskNo, bool isLock);

protected:
	HNRoomDeskLogic* _deskLogic;
};

#endif // GameDesk_h__