#ifndef _ZYHZ_CARDPOOLUSER_H_
#define _ZYHZ_CARDPOOLUSER_H_

#include "ZYHZ_MessageHead.h"
#include "ZYHZ_CardPool.h"
namespace ZYHZ
{
	class CardPoolUser :
		public cocos2d::Node
	{
	public:
		CardPoolUser(void);
		~CardPoolUser(void);

		CC_SYNTHESIZE_PASS_BY_REF(UserInfoStruct, _userInfo, UserInfo);

		void setPool(CardPool* pool);

		CardPool* getOwnPool() { return _cardPool; };

	private:
		CardPool* _cardPool;
	};

}

#endif // _CARDPOOLUSER_H_
