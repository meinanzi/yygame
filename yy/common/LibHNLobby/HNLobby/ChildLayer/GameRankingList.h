#ifndef __GAMERANKING_LAYER_H__
#define __GAMERANKING_LAYER_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

using namespace HN;
using namespace ui;
using namespace std;
using namespace extension;

class GameRankingList : public HNLayer, public TableViewDataSource, public TableViewDelegate
{

public:
	GameRankingList();
	virtual ~GameRankingList();

public:
	typedef std::function<void ()> CloseCallBack;
	CloseCallBack	onCloseCallBack;

public:
	virtual bool init() override;
	void closeRanking();
	void closeEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void openRank(Node* parent, Vec2 vec, int zorder, int tag);
	CREATE_FUNC(GameRankingList);

	/*****************************必须继承****************************/
	virtual void scrollViewDidScroll(extension::ScrollView* view) {};//滑动
	virtual void scrollViewDidZoom(extension::ScrollView* view) {};  //放大

	/***************************TableView**********************************/

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);//触摸事件
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);//Cell大小
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);//添加Cell
	virtual ssize_t numberOfCellsInTableView(TableView *table);//设置Cell个数

private:
	TableView *	_tableView;
	vector<MSG_GP_MoneyPaiHangBang_Item> _iList;
	int MyRankNum;
	ui::Scale9Sprite* rankingFrame;
	Text* rankingText;
private:
	//获取排行数据
	void getRankListData();
	//申请打开排行榜回调
	bool openRankListEventSelector(HNSocketMessage* socketMessage);
	//显示我的排名
	void showMyRank();
};


#endif // __GAMERANKING_LAYER_H__