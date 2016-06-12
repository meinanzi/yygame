#ifndef __HN_WaterMargin__GameRule__
#define __HN_WaterMargin__GameRule__

#include "cocos2d.h"
#include "HNUIExport.h"
#include <string>

using namespace std;
using namespace HN;

USING_NS_CC;

namespace WaterMargin
{

	class GameRule
	{
	public:
		struct RULE
		{
			int		_line;			//第几线
			bool	_value;			//是否中奖
			int		_firstImage;	//中奖第一个位置
			int		_lastImage;		//中奖最后一个位置
			int		_data[5];

			RULE()
			{
				reset();
			}

			void reset()
			{
				_line = -1;
				_value = false;
				_firstImage = -1;
				_lastImage = -1;
				memset(_data, 0, sizeof(_data));
			}

			void WinRule()
			{
				int num = 0;
				for (; num < 5; num++)
				{
					if (_data[num] != 1)	break;
				}

				if (num == 5)
				{
					_value = true;
					_firstImage = 0;
					_lastImage = 4;
					return;
				}

				int value = _data[num];
				int i = num+1;
				for (; i < 5; i++)
				{
					if (_data[i] != value && _data[i] != 1)		break;					
				}
				if (i >= 3)
				{
					_value = true;
					_firstImage = 0;					
				}
				if (i == 5)
				{					
					_lastImage = 4;
					return;
				}
				else if (i == 4)
				{
					_lastImage = 3;
					return;
				}
				else if (i == 3)
				{
					_lastImage = 2;
					return;
				}
				
				//-------------右---------------
				num = 4;
				for (; num >= 0; num--)
				{
					if (_data[num] != 1)	break;
				}
				value = _data[num];
				i = num-1;
				for (; i >= 0; i--)
				{
					if (_data[i] != value && _data[i] != 1)		break;
				}
				if (i == 0)
				{
					_value = true;
					_firstImage = 1;
					_lastImage = 4;
					return;
				}
				else if (i == 1)
				{
					_value = true;
					_firstImage = 2;
					_lastImage = 4;
					return;
				}
			}
		}_userObject;
	};
}

#endif