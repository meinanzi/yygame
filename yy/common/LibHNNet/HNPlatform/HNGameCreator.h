#ifndef HNGame_Creator_h__
#define HNGame_Creator_h__

#include "HNBaseType.h"
#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include <unordered_map>

typedef std::function<HNGameUIBase* (UINT uDeskId, bool autoCreate)> GAME_CREATE_SELECTOR;

typedef std::function<HNLayer* (Node* parent)> ROOM_CREATE_SELECTOR;

class HNGameCreator
{
public:
	// 游戏类型
	enum GAMETYPE
	{
		UNKNOWN = 0, // 未知类型
		NORMAL,      // 常规游戏
		BR,          // 百人游戏
		SINGLE       // 单人游戏
	};
	static const BYTE INVALID_PRIORITY = 0xFF;

	struct ItemCreator
	{
		BYTE priority;
		GAMETYPE type;
		UINT uNameId;
		UINT uKindId;
		GAME_CREATE_SELECTOR	gameSelector;
		ROOM_CREATE_SELECTOR	roomSelector;

		ItemCreator() : uNameId(0), uKindId(0), type(UNKNOWN), gameSelector(nullptr), roomSelector(nullptr)
		{

		}

		bool validGame()
		{
			return (0 != uNameId) && (UNKNOWN != type) && (nullptr != gameSelector);
		}

		bool valid()
		{
			return (0 != uNameId) && (0 != uKindId) && (UNKNOWN != type) && (nullptr != gameSelector);
		}
	};

public:
	// 获取单例
	static HNGameCreator* getInstance();

	// 销毁单例
	static void destroyInstance();

	// 获取注册游戏数量
	int getRegistGameCount();

public:
	// 注册游戏
	void addGame(UINT uNameId, GAMETYPE type, GAME_CREATE_SELECTOR createGameSelector, ROOM_CREATE_SELECTOR createRoomSelector = nullptr);

	// 设置注册游戏的类型id
	void setGameKindId(UINT uNameId, UINT uKindId);
	
	// 获取有效的游戏列表
	bool getValidGames(std::vector<ComNameInfo*>* validGames);
	std::vector<ComNameInfo*> getValidGames();

	// 获取游戏优先级
	BYTE getGamePriority(UINT uNameId);
	
	// 设置当前选择的游戏ID
	void setCurrentGame(UINT uNameId);
	
	// 当前游戏ID
	UINT getCurrentGameNameID() const;
	
	// 当前游戏类型
	UINT getCurrentGameKindID() const;
	
	// 获取游戏类型
	GAMETYPE getCurrentGameType() const;

public:
	// 启动游戏客户端
	bool startGameClient(UINT uNameId, BYTE bDeskIndex, bool bAutoCreate);
	
	// 启动游戏房间
	bool startGameRoom(UINT uNameId, Node* parent);

protected:
	// 校验游戏
	BYTE validGame(UINT uNameId);

private:
	// 构造函数
	HNGameCreator();

	// 析构函数
	virtual ~HNGameCreator();

private:
	// 注册游戏列表
	std::unordered_map<UINT, ItemCreator> _creators;

	// 当前激活游戏
	ItemCreator* _currentCreator;

	// 优先级
	BYTE	_basePriority;

	// 游戏游戏数量
	int     _validCount;

	// 有效游戏列表
	std::vector<ComNameInfo*> _validGames;
};

#define GameCreator()	HNGameCreator::getInstance()

#endif // HNGame_Creator_h__
