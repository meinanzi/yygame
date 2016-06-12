#include "MahjongCardPool.h"

namespace XZMJ
{

	MahjongCardPool::MahjongCardPool(void)
	{
	}


	MahjongCardPool::~MahjongCardPool(void)
	{
	}

	void MahjongCardPool::sortCard()
	{
		_isInSort = true;
	}

	bool MahjongCardPool::init()
	{
		if (! CardPool::init())
		{
			return false;
		}
		return true;
	}


}