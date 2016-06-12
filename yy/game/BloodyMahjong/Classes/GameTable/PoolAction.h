#ifndef _POOLACTION_H_
#define _POOLACTION_H_

#include "cocos2d.h"
#include "CardPool.h"

namespace XZMJ
{
	class PoolAction :
		public cocos2d::Node
	{
	public:
		PoolAction(void);
		~PoolAction(void);

		void setInfo(CardPool* pool, const INT& cardNumber);
		void uiAction(std::string name);
		virtual void run() = 0;

	private:
		Animation* getAni(std::string name);
		
	protected:
		CardPool * _pool;
		INT _cardNumber;
		Vector<SpriteFrame *> _vSpFrame;
		Sprite* _runSp;
	};
}

#endif