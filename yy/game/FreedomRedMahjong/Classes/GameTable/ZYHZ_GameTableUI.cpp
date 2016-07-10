#include "ZYHZ_GameTableUI.h"


using namespace std;

namespace ZYHZ
{
	/**************************************************************************/

    GameTableUI::GameTableUI()
    : _maimaNode(nullptr)
    , _timer(0)
    , _maimaCount(0)
	{
        memset(&_maimaData, 0, sizeof(_maimaData));
	}

	GameTableUI::~GameTableUI()
	{
		delete _tableLogic;
		_tableLogic = nullptr;
	}

	GameTableUI* GameTableUI::create(INT deskNo, bool autoCreate)
	{
		auto ui = new GameTableUI;
		if (ui && ui->init(deskNo, autoCreate))
		{
			ui->autorelease();
			return ui;
		}
		CC_SAFE_DELETE(ui);
		return nullptr;
	}

	bool GameTableUI::init(INT deskNo, bool autoCreate)
	{
		if (!HNGameUIBase::init())
		{
			return false;
		}

		//loadResource();
		_mahjongManager = GameManager::create();
		this->addChild(_mahjongManager);

		// logic
		_tableLogic = new GameTableLogic(this, deskNo, autoCreate);
		_tableLogic->sendGameInfo();

		_mahjongManager->restartGame();

        {
//            // test
//            tagMaiMa testdata;
//            testdata.byCount=3;
//            testdata.byCard[0] = 8;
//            testdata.byCard[1] = 8;
//            testdata.byCard[2] = 8;
//            testdata.byCard[3] = 8;
//            
//            showMaimaOption(10, &testdata);
        }

		return true;
	}
	

	/******************************************************************/

	/******************************************************************/

	

	void GameTableUI::onGameDisconnect()
	{
		dealLeaveDesk();
	}


	void GameTableUI::dealLeaveDesk()
	{
		if (PlatformLogic()->isConnect())
		{
			GamePlatform::returnPlatform(LayerType::DESKLIST);
		}
		else
		{
            GamePlatform::returnPlatform(LayerType::ROOMLIST);
		}
	}

	void GameTableUI::agreeGame(const sitDir& dir)
	{
		_mahjongManager->userAgree(dir);
	}

	void GameTableUI::startGame(const bool& autoBegin)
	{
		_mahjongManager->startGame();
	}

	/*****************************************************************************************/

	void GameTableUI::outCard(const sitDir& dir, const INT& number)
	{
		_mahjongManager->outCard(dir, number);
	}

	void GameTableUI::catchCard(const sitDir& dir, const INT& number, const bool& head)
	{
		_mahjongManager->catchCard(dir, number, head);
	}

	void GameTableUI::setPlayGame(bool bs)
	{
		_mahjongManager->setPlayGame(bs);
	}

	void GameTableUI::reconnected(
			std::vector<std::vector<INT>>& vvHandCard, 
			std::vector<std::vector<INT>>& vvOutCard, 
			std::vector<std::vector<std::vector<INT>>>& vvvActionCard,
			std::vector<std::vector<INT>>& vvMenPai,
			std::vector<bool>& vTrusteeshep,
			std::vector<INT>& vQueMen,
			sitDir& ntDir)
	{
		_mahjongManager->reconnected(vvHandCard, vvOutCard, vvvActionCard, vvMenPai, vTrusteeshep, vQueMen, ntDir);
	}


	/*****************************************************************************************/
	void GameTableUI::addUser(const sitDir& dir, const UserInfoStruct& user)
	{
		_mahjongManager->addUser(dir, user);
	}

	void GameTableUI::removeUser(const sitDir& dir)
	{
		_mahjongManager->userLeave(dir);
	}

	/******************************************************************/
    
    void GameTableUI::hideMaimaOption()
    {
        if(_maimaNode!=nullptr)
        {
            _maimaNode->removeFromParent();
        }
        _maimaNode = nullptr;
    }
    
    void GameTableUI::showMaimaOption(BYTE timer, tagMaiMa* data)
    {
        hideMaimaOption();
        _maimaCount = 0;
        _timer = timer;
        memcpy(&_maimaData, data, sizeof(_maimaData));
        
        if(data!=nullptr)
        {
            _maimaNode = CSLoader::createNode(COCOS_PATH + "maima.csb");
            auto winsize = Director::getInstance()->getWinSize();
            _maimaNode->setContentSize(winsize);
            addChild(_maimaNode);
            Helper::doLayout(_maimaNode);
            
            // 禁止触摸向下传递
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [](Touch* touch, Event* unused_event){ return true;};
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _maimaNode);
            
            // Title提示
            auto title = (Text*)Helper::seekWidgetByName((Widget*)_maimaNode, "Text_title");
            char buff[64] = {0};
            sprintf(buff, "您还有%d次买码机会", data->byCount);
            title->setString(GBKToUtf8(buff));
            
            // 买码
            auto bg = dynamic_cast<ImageView*>(Helper::seekWidgetByName((Widget*)_maimaNode, "Image_bg"));
            for(auto imgnode: bg->getChildren())
            {
                auto img = (ImageView*)imgnode;
                img->addTouchEventListener([this, img, title](Ref* sender, Widget::TouchEventType touch)
                {
                    if(touch == Widget::TouchEventType::ENDED)
                    {
                        if(_maimaCount<_maimaData.byCount)
                        {
                            auto filename = getMjSpriteframeName(mahjongCreateType::DI_ZHENG_SOUTH, sitDir::MID_DIR, _maimaData.byCard[_maimaCount]);
                            if(!filename.empty())
                            {
                                img->loadTexture(filename, Widget::TextureResType::PLIST);
                            }
                            _maimaCount++;
                            
                            char buff[64] = {0};
                            sprintf(buff, "您还有%d次买码机会", _maimaData.byCount - _maimaCount);
                            title->setString(GBKToUtf8(buff));
                        }
                    }
                });
                img->setTouchEnabled(true);
            }
            
            // 计时器
            auto Text_timer = (Text*)Helper::seekWidgetByName((Widget*)_maimaNode, "Text_timer");
            sprintf(buff, "%d", timer);
            Text_timer->setString(buff);
            Text_timer->runAction(RepeatForever::create(Sequence::create(
                DelayTime::create(1.0f),
                CallFunc::create([this, Text_timer]()
                {
                    if(--_timer<0)
                    {
                        Text_timer->stopAllActions();
                        hideMaimaOption();
                    }
                    char buff[16];
                    sprintf(buff, "%d", _timer);
                    Text_timer->setString(buff);
                }),
                nullptr)));

        }
    }
    
    std::string GameTableUI::getMjSpriteframeName(mahjongCreateType mjtype, sitDir dir, int value)
    {
        auto mj = MahjongCard::create(mjtype, dir, value);
        auto sfname = mj->getSpriteframename();
        sfname+=".png";
        auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(sfname);
        if(sf==nullptr)
        {
            sfname="";
        }
        return sfname;
    }
};
