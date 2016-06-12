#include "HNGamesInfoModule.h"
#include "HNCommon/HNCommonMarco.h"

namespace HN
{
	HNGamesInfoModule* HNGamesInfoModule::getInstance()
	{
		static HNGamesInfoModule gamesInfoModule;
		return &gamesInfoModule;
	}

	HNGamesInfoModule::HNGamesInfoModule(void)
		: _gameKinds(new GAME_KINDS())
		, _gameNames(new GAME_NAMES())
		, _selectedGame(nullptr)
	{
	}

	HNGamesInfoModule::~HNGamesInfoModule(void)
	{
		clear();
		HN_SAFE_DELETE(_gameKinds);
		HN_SAFE_DELETE(_gameNames);
	}

	void HNGamesInfoModule::clear()
	{
		for (auto iter = _gameNames->begin(); iter != _gameNames->end(); ++iter)
		{
			HN_SAFE_DELETE(*iter);
		}
		_gameNames->clear();

		for (auto iter = _gameKinds->begin(); iter != _gameKinds->end(); ++iter)
		{
			HN_SAFE_DELETE(*iter);
		}
		_gameKinds->clear();
	}

	int HNGamesInfoModule::getGameNameCount() const
	{
		return _gameNames->size();
	}

	ComNameInfo* HNGamesInfoModule::getGameNameByKindID(INT kindID)
	{
		ComNameInfo* game = nullptr;
		for (auto iter =_gameNames->begin(); iter != _gameNames->end(); ++iter)
		{
			if ((*iter)->uKindID == kindID) 
			{
				game = *iter; 
				break;
			}
		}
		return game;
	}

	ComNameInfo* HNGamesInfoModule::getGameNameByPos(INT pos)
	{
		if (pos >= _gameNames->size())
		{
			return nullptr;
		}

		if (!_gameNames->empty())
		{
			return _gameNames->at(pos);
		}

		return nullptr;
	}

	void HNGamesInfoModule::addGameKind(ComKindInfo* kindInfo)
	{
		assert(kindInfo != nullptr);
		if (nullptr != kindInfo)
		{
			ComKindInfo* game = findGameKind(kindInfo->uKindID);
			if (nullptr != game)
			{
				*game = *kindInfo;
			}
			else
			{
				ComKindInfo* game = new ComKindInfo();
				*game = *kindInfo;
				_gameKinds->push_back(game);
			}
		}
	}

	void HNGamesInfoModule::addGameName(ComNameInfo* nameInfo)
	{
		assert(nameInfo != nullptr);
		if (nullptr != nameInfo)
		{
			ComNameInfo* game = findGameName(nameInfo->uNameID);
			if (nullptr != game)
			{
				*game = *nameInfo;
			}
			else
			{
				ComNameInfo* game = new ComNameInfo();
				*game = *nameInfo;
				_gameNames->push_back(game);
			}
		}
	}

	ComKindInfo* HNGamesInfoModule::findGameKind(int kindID)
	{
		ComKindInfo* game = nullptr;
		for (auto iter = _gameKinds->begin(); iter != _gameKinds->end(); ++iter)
		{
			if ((*iter)->uKindID == kindID) 
			{
				game = *iter;
				break;
			}
		}
		return game;
	}

	/*
	* 查找游戏数据
	*/
	ComNameInfo* HNGamesInfoModule::findGameName(INT gameID)
	{
		ComNameInfo* game = nullptr;
		for (auto iter = _gameNames->begin(); iter != _gameNames->end(); ++iter)
		{
			if ((*iter)->uNameID == gameID) 
			{
				game = *iter;
				break;
			}
		}
		return game;
	}

	void HNGamesInfoModule::transform(const TransformNameInfo& fun)
	{
		INT index = 0;
		for (auto iter = _gameNames->begin(); iter != _gameNames->end(); ++iter, ++index)
		{
			fun(*iter, index);
		}
	}

// 	void HNGamesInfoModule::transform(const TransformKindInfo& fun)
// 	{
// 		INT index = 0;
// 		for (auto iter = _gameKinds->begin(); iter != _gameKinds->end(); ++iter, ++index)
// 		{
// 			fun(*iter, index);
// 		}
// 	}

}