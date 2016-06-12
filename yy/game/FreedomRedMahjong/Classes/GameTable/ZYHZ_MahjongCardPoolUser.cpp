#include "ZYHZ_MahjongCardPoolUser.h"

namespace ZYHZ
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