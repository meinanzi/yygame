#include "MahjongCardPoolUser.h"

namespace XZMJ
{

	MahjongCardPoolUser::MahjongCardPoolUser(void)
	{
	}


	MahjongCardPoolUser::~MahjongCardPoolUser(void)
	{
	}

	bool MahjongCardPoolUser::init()
	{
		if (!CardPoolUser::init())
		{
			return false;
		}
		return true;
	}

}