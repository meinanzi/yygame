#include "HNLobby/ChildLayer/NoticeList.h"
#include "HNNetExport.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"

USING_NS_CC_EXT;
using namespace network;

// 对话框布局文件
static const char* NOTICE_LIST_CSB = "platform/NoticeList/NoticeList.csb";

// 列表项布局文件
static const char* NOTICE_LIST_ITEM = "platform/NoticeList/NoticeItem.csb";

class NoticeItem: public TableViewCell
{
public:
	// 创建
	CREATE_FUNC(NoticeItem);

	// 初始化
	bool init()
	{
		if(!TableViewCell::init())
		{
			return false;
		}

		// 布局
		auto csLoader = CSLoader::createNode(NOTICE_LIST_ITEM);
		CCAssert(csLoader != nullptr, "null");
		this->addChild(csLoader);
		csLoader->setPosition(Vec2(310.f, 60.f));

		// 标题
		_title = dynamic_cast<ui::Text*>(csLoader->getChildByName("Text_title"));
		CCAssert(_title != nullptr, "null");

	    // 内容
		_content = dynamic_cast<ui::Text*>(csLoader->getChildByName("Text_content"));
		CCAssert(_content != nullptr, "null");

		// 时间
		_time = dynamic_cast<ui::Text*>(csLoader->getChildByName("Text_time"));
		CCAssert(_time != nullptr, "null");

		return true;
	}

	// 修改标题
	void setTitle(const std::string& title)
	{
		_title->setString(title);
	}

	// 修改内容
	void setContent(const std::string& content)
	{
		_content->setString(content);
	}

	// 修改时间
	void setTime(const std::string& time)
	{
		_time->setString(time);
	}

	NoticeItem(): _title(nullptr), _content(nullptr), _time(nullptr){}

private:
	ui::Text* _title;
	ui::Text* _content;
	ui::Text* _time;
};

bool NoticeList::init()
{
	if(!HNLayer::init())
	{
		return false;
	}

	ignoreAnchorPointForPosition(false);

	// 添加屏蔽层
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

	auto winSize = Director::getInstance()->getWinSize();

	// 加载布局
	auto csLoader = CSLoader::createNode(NOTICE_LIST_CSB);
	CCAssert(csLoader != nullptr, "null");

	this->addChild(csLoader);
	csLoader->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	// 列表背景
	ImageView_bg = dynamic_cast<ui::ImageView*>(csLoader->getChildByName("Image_bg"));

	ImageView_frame = dynamic_cast<ui::ImageView*>(ImageView_bg->getChildByName("Image_frame"));
	CCAssert(ImageView_frame != nullptr, "null");

	// 关闭按钮
	auto Button_close = dynamic_cast<ui::Button*>(ImageView_bg->getChildByName("Button_close"));
	CCAssert(Button_close != nullptr, "null");
	Button_close->addTouchEventListener(CC_CALLBACK_2(NoticeList::onCloseClickCallback, this));

	

	// 添加列表
	_tableView = TableView::create(this, Size(ImageView_frame->getContentSize().width - 10, ImageView_frame->getContentSize().height - 10));
	_tableView->setDirection(extension::ScrollView::Direction::VERTICAL);
	_tableView->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
	ImageView_frame->addChild(_tableView);

	Vec2 anchor = ImageView_frame->getAnchorPoint();
	Vec2 pos    = ImageView_frame->getPosition();

	_tableView->setPosition(Vec2(5.f, 5.f));
	//_tableView->setPosition(Vec2(pos.x - pos.x * anchor.x + 5, pos.y - pos.y * anchor.y + 5));

	return true;
}

Size NoticeList::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
	return Size(620, 115);
}

extension::TableViewCell* NoticeList::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
	TableViewCell* cell = table->dequeueCell();

	if(cell == nullptr)
	{
		cell = NoticeItem::create();
	}

	auto item = (NoticeItem*)cell;
	item->setTitle(_datas[idx].title);
	item->setContent(_datas[idx].content);
	item->setTime(_datas[idx].time);

	return cell;
}

// 列表项数量
ssize_t NoticeList::numberOfCellsInTableView(extension::TableView *table)
{
	return _datas.size();
}

// 列表项被点击
void NoticeList::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{

}

void NoticeList::queryNoticeList()
{
	std::string url = PlatformConfig::getInstance()->getNoticeUrl();
	url.append("Type=GetSystemMsg");
	url.append("&pageSize=-1");
	url.append("&pageindex=1");

	HNHttpRequest::getInstance()->addObserver(this);
	HNHttpRequest::getInstance()->request("notice", HttpRequest::Type::GET, url);
}


void NoticeList::onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData)
{
	if (requestName.compare("notice") != 0)
	{
		return;
	}

	if (!isSucceed)
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("数据查询失败"));
		return;
	}

	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(responseData.c_str());

	if (doc.HasParseError() || !doc.IsObject())
	{
		return;
	}

	if (doc.HasMember("list") && doc["list"].IsArray() && doc["list"].Capacity() > 0)
	{
		_datas.clear();
		for (rapidjson::SizeType i = 0; i < doc["list"].Capacity(); i++)
		{
			rapidjson::Value& value = doc["list"][i];
			DataNode node;
			node.title = value["MTitle"].GetString();
			node.content = value["MContent"].GetString();
			node.time = value["AddTime"].GetString();

			_datas.push_back(node);
		}

		_tableView->reloadData();
	}
}

void NoticeList::onEnter()
{
	HNLayer::onEnter();

	queryNoticeList();
}

void NoticeList::onCloseClickCallback(Ref* pSender, Widget::TouchEventType type)
{
	if(type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	ImageView_bg->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&](){
			this->removeFromParent();
	}), nullptr));
}

NoticeList::NoticeList()
	: _tableView(nullptr)
{

}

NoticeList::~NoticeList()
{
	HNHttpRequest::getInstance()->removeObserver(this);
}