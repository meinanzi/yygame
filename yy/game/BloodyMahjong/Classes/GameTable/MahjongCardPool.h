#ifndef _MAHJONGCARDPOOL_H_
#define _MAHJONGCARDPOOL_H_

#include "cardPool.h"
#include "MahjongCard.h"

namespace XZMJ
{
#define CREATE_COUNT(_className) \
	static _className * create(INT count) \
	{ \
		auto p = new _className; \
		if (p && p->init(count)) \
		{ \
			p->autorelease(); \
			return p; \
		} \
		delete p; \
		return nullptr; \
	} 

	class MahjongCardPool :
		public CardPool
	{
	public:
		MahjongCardPool(void);
		~MahjongCardPool(void);

		CREATE_FUNC(MahjongCardPool);
		virtual void sortCard() override;
		virtual bool init() override;

	protected:
		INT _sumCount;    // ×ÜÕÅÊý
	};

}

#endif