#ifndef __DriftingCarGameCell_H__
#define __DriftingCarGameCell_H__

/*
 *	ลÜยํตฦตฅิช
 */


#include "HNUIExport.h"

#include <array>

namespace DriftingCar
{
	class GameCell:public HNSprite
	{
	public:
		enum 
		{
			CAR_LOGO_COUNT = 32,
		};
		static GameCell* create();

		virtual bool init()override;

		void turnOn();

		void turnOff();
	private:

	protected:
		GameCell();
		virtual ~GameCell();

		Sprite* _spriteCar;
	};
}


#endif // __GuessCarGameCell_H__



