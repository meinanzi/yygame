#include "BJLResultDialog.h"
#include "cocostudio/CocoStudio.h"
#include "HNLobby/FontConfig.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

ResultDialog::ResultDialog() 
	: _dealerWin(0)
	, _myWin(0)
{

}

ResultDialog::~ResultDialog()
{

}

ResultDialog* ResultDialog::create(Node* parent, LLONG dealerWin, LLONG myWin)
{
	ResultDialog* ptr = new ResultDialog();
	if (nullptr !=ptr && ptr->init(parent,dealerWin,myWin))
	{
		ptr->ignoreAnchorPointForPosition(false);
		ptr->autorelease();
		return ptr;
	}
	CC_SAFE_DELETE(ptr);
	return nullptr;
}

bool ResultDialog::init(Node* parent, LLONG dealerWin, LLONG myWin)
{
	if (!HNDialogBase::init(parent))
	{
		return false;
	}

	_dealerWin = dealerWin;
	_myWin = myWin;

	auto distpatch = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ResultDialog::onTouchBegan,this);
	listener->setSwallowTouches(true);
	distpatch->addEventListenerWithSceneGraphPriority(listener,this);
	
	Size size = getContentSize();
	_widget = GUIReader::getInstance()->widgetFromJsonFile("BJL/game/settlement/settlement.json");
	_widget->setAnchorPoint(Vec2(0.5f, 0.5f));
	_widget->ignoreAnchorPointForPosition(false);
	_widget->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(_widget);
	initUI();
	return true;
}

void ResultDialog::initUI()
{
	auto bg = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_widget,"bg"));
	auto size = bg->getContentSize();
	char str[50]={0};
	
	// ×¯¼ÒÊäÓ®
	sprintf(str,"%lld",_dealerWin);
	auto dealerScore = Label::createWithBMFont("BJL/game/table/number.fnt", str);
	dealerScore->setAnchorPoint(Vec2(0.5f , 0.5f));
	dealerScore->setPosition(size.width * 0.5 , size.height * 0.5 - 80);
	dealerScore->setScale(0.7f);
	_widget->addChild(dealerScore);

	// Íæ¼ÒÊäÓ®
	sprintf(str,"%lld", _myWin);
	auto myScore = Label::createWithBMFont("BJL/game/table/number.fnt", str);
	myScore->setAnchorPoint(Vec2(0.5f, 0.5f));
	myScore->setPosition(size.width * 0.5 , size.height * 0.5 + 50);
	myScore->setScale(0.7f);
	_widget->addChild(myScore);

}

bool ResultDialog::onTouchBegan(Touch *touch, Event *unused_event)
{
	return false;
}