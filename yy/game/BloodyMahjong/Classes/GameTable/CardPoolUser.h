#ifndef _CARDPOOLUSER_H_
#define _CARDPOOLUSER_H_

#include "XZMJ_MessageHead.h"
#include "CardPool.h"
namespace XZMJ
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
