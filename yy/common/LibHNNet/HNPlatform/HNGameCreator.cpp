#include "HNGameCreator.h"
#include <assert.h>

static HNGameCreator* sGameCreate = nullptr;

HNGameCreator* HNGameCreator::getInstance()
{
	if (nullptr == sGameCreate)
	{
		sGameCreate = new (std::nothrow) HNGameCreator();
	}
	return sGameCreate;
}

void HNGameCreator::destroyInstance()
{
	CC_SAFE_DELETE(sGameCreate);
}

int HNGameCreator::getRegistGameCount()
{
	return _creators.size();
}

void HNGameCreator::addGame(UINT uNameId, GAMETYPE type, GAME_CREATE_SELECTOR createGameSelector, ROOM_CREATE_SELECTOR createRoomSelector)
{
	ItemCreator creator;
	creator.type = type;
	creator.uNameId = uNameId;
	creator.gameSelector = createGameSelector;
	creator.roomSelector = createRoomSelector;
	creator.priority = ++_basePriority;

	assert(creator.validGame());
	auto value = std::make_pair(uNameId, creator);
	_creators.insert(value);
	setCurrentGame(creator.uNameId);
}

void HNGameCreator::setGameKindId(UINT uNameId, UINT uKindId)
{
	auto iter = _creators.find(uNameId);
	if (iter != _creators.end())
	{
		ItemCreator* creator = &(*iter).second;
		creator->uKindId = uKindId;
	}
}

bool HNGameCreator::startGameClient(UINT uNameId, BYTE bDeskIndex, bool bAutoCreate)
{
	auto iter = _creators.find(uNameId);
	if (iter != _creators.end())
	{
		ItemCreator* creator = &(*iter).second;
		if (creator->valid())
		{
			HNGameUIBase* game = creator->gameSelector(bDeskIndex, bAutoCreate);

			if (nullptr != game)
			{
				auto scene = Scene::create();
				scene->addChild(game);
				Director::getInstance()->replaceScene(scene);
				return true;
			}
		}
	}
	return false;
}

bool HNGameCreator::startGameRoom(UINT uNameId, Node* parent)
{
	auto iter = _creators.find(uNameId);
	if (iter != _creators.end())
	{
		HNLayer* room = (*iter).second.roomSelector(parent);

		if (nullptr != room)
		{
			return true;
		}
	}
	return false;
}

void HNGameCreator::setCurrentGame(UINT uNameId)
{
	auto iter = _creators.find(uNameId);
	if (iter != _creators.end())
	{
		_currentCreator = &(*iter).second;
	}
	else
	{
		_currentCreator = nullptr;
	}
}

UINT HNGameCreator::getCurrentGameNameID() const
{
	assert(nullptr != _currentCreator);

	return (nullptr != _currentCreator) ? _currentCreator->uNameId : 0;
}

UINT HNGameCreator::getCurrentGameKindID() const
{
	assert(nullptr != _currentCreator);
	return (nullptr != _currentCreator) ? _currentCreator->uKindId : -1;
}

BYTE HNGameCreator::validGame(UINT uNameId)
{
	auto iter = _creators.find(uNameId);
	if (iter != _creators.end())
	{
		return iter->second.priority;
	}
	return INVALID_PRIORITY;
}

// 获取有效的游戏
bool HNGameCreator::getValidGames(std::vector<ComNameInfo*>* validGames)
{
	{
		std::vector<ComNameInfo*>().swap(*validGames);
	}

	*validGames = getValidGames();

	return !validGames->empty();
}

std::vector<ComNameInfo*> HNGameCreator::getValidGames()
{
	//std::vector<ComNameInfo*> validGames(255);
	//INT validCount = 0;
	_validGames.clear();
	_validGames.resize(255);
	_validCount = 0;

	{
		GamesInfoModule()->transform([&](ComNameInfo* nameInfo, INT index)
		{
			BYTE level = validGame(nameInfo->uNameID);
			if (INVALID_PRIORITY != level)
			{
				_validGames[level] = nameInfo;
				++_validCount;
			}
		});
	}

	for (int i = 0; i < _validCount; ++i)
	{
		if (nullptr == _validGames[i])
		{
			for (int j = i + 1; j != _validGames.size(); ++j)
			{
				if (nullptr != _validGames[j])
				{
					_validGames[i] = _validGames[j];
					_validGames[j] = nullptr;
					break;
				}
			}
		}
	}

	_validGames.resize(_validCount);

	return _validGames;
}

BYTE HNGameCreator::getGamePriority(UINT uNameId)
{
	auto iter = _creators.find(uNameId);
	if (iter != _creators.end())
	{
		return iter->second.priority;
	}
	return INVALID_PRIORITY;
}

HNGameCreator::GAMETYPE HNGameCreator::getCurrentGameType() const
{
	assert(nullptr != _currentCreator);
	return (nullptr != _currentCreator) ? _currentCreator->type : UNKNOWN;
}
 
HNGameCreator::HNGameCreator() 
	: _currentCreator(nullptr)
	, _basePriority(INVALID_PRIORITY)
	, _validCount(0)
{

}

HNGameCreator::~HNGameCreator()
{
}        