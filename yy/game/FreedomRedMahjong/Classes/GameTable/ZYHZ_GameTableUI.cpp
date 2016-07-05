#include "ZYHZ_GameTableUI.h"


using namespace std;

namespace ZYHZ
{
	/**************************************************************************/

	GameTableUI::GameTableUI()
	{
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
    
    void GameTableUI::showMaimaOption(BYTE timer, tagMaiMa* data)
    {
        auto node = CSLoader::createNode(COCOS_PATH + "maima.csb");
        
        auto maimacall = [this]()
        {
            if( _maimaCount < _maimaData.byCount )
            {
                _maimaCount++;
            }
        };
        
    }
};
