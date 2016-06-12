#ifndef __HN_GoldenToad_GAME_AUDIO_H__
#define __HN_GoldenToad_GAME_AUDIO_H__

#include <string>
#include "GoldenToadMessageHead.h"

namespace GoldenToad
{

	class GoldenToadGameAudio
	{
	public:
		static void playBackgroundMusic();
		static void stopBackgroundMusic();
		static void pauseBackgroundMusic();
		static void resumeBackgroundMusic();

		static void playSmallFishDiead(FishKind kind);
		static void playFireEffect();
		static void playNetEffect();
		static void playIonFireEffect();
		static void playBingoEffect();
		static void playGoldEffect();
		static void playWaveEffect();
		static void playCatchEffect();
		static void playBombEffect();
		static void playAddEffect();

	};
}


#endif