#ifndef __Landlord_GAME_AUDIO_H__
#define __Landlord_GAME_AUDIO_H__

#include <string>

namespace landlord
{

	class GameAudio
	{
	public:
		static void playBackground();
		static void stopBackground();

		static void playCallNT(bool isBoy);
		static void playNotCallNT(bool isBoy);
		static void playRobNT(bool isBoy);
		static void playNotRobNT(bool isBoy);
		static void playDouble(bool isBoy);
		static void playNotDouble(bool isBoy);
		static void playPublish(bool isBoy);
		static void playNotOut(bool isBoy);

		static void playSingle(const std::string &value, bool isBoy);
		static void playDouble(const std::string &value, bool isBoy);
		static void playThree(int value, bool isBoy);
		static void playStraight(bool isBoy);
		static void playDoubleSequence(bool isBoy);
		static void playPlan(bool isBoy);
		static void playFourTake(bool isDouble, bool isBoy);

		static void playRocket(bool isBoy);
		static void playBomb(bool isBoy);
		static void playCardLast(bool isBoy, int value);

		static void playPlanEffect();
		static void playBombEffect();
		static void playDoubleSequenceEffect();
		static void playRocketEffect();
		static void playStraightEffect();

		static void playWin();
		static void playLose();

		static void playDispathCard();
	};
}


#endif