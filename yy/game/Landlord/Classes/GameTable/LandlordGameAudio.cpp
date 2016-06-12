#include "LandlordGameAudio.h"
#include "HNUIExport.h"

namespace landlord
{

	#define GAME_AUDIO_ROOT "landlord/game/audio/effect/"

	void GameAudio::playBackground()
	{
		HNAudioEngine::getInstance()->playBackgroundMusic("landlord/game/audio/music/Audio_after_bomb.mp3", true);
	}

	void GameAudio::stopBackground()
	{
		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void GameAudio::playCallNT(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		isBoy ? filename.append("CallLandlord_JDZ_M.mp3") : filename.append("CallLandlord_JDZ_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playNotCallNT(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		isBoy ? filename.append("CallLandlord_BJDZ_M.mp3") : filename.append("CallLandlord_BJDZ_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playRobNT(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		isBoy ? filename.append("CallLandlord_QDZ_M.mp3") : filename.append("CallLandlord_QDZ_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playNotRobNT(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		isBoy ? filename.append("CallLandlord_BQDZ_M.mp3") : filename.append("CallLandlord_BQDZ_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playDouble(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		isBoy ? filename.append("CALL_DOUBLE_M.mp3") : filename.append("CALL_DOUBLE_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playNotDouble(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		isBoy ? filename.append("CALL_NO_DOUBLE_M.mp3") : filename.append("CALL_NO_DOUBLE_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playPublish(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		isBoy ? filename.append("CARDS_VISIBLE_M.mp3") : filename.append("CARDS_VISIBLE_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playNotOut(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		isBoy ? filename.append("Audio_Pass1_M.mp3") : filename.append("Audio_Pass1_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void GameAudio::playSingle(const std::string &value, bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_Single_");
		filename.append(value);
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameAudio::playDouble(const std::string &value, bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_Double_");
		filename.append(value);
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void GameAudio::playThree(int value, bool isBoy)
	{
		char str[10] = {0};
		sprintf(str, "%d", value);
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_Three_");
		filename.append(str);
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameAudio::playStraight(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_Straight");
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameAudio::playDoubleSequence(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_DoubleLine");
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameAudio::playPlan(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_Plane");
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameAudio::playFourTake(bool isDouble, bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		if(isDouble)
		{
			filename.append("Audio_Card_Four_Take_2Double");
		}
		else
		{
			filename.append("Audio_Card_Four_Take_2");
		}
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playRocket(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_Rocket");
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playBomb(bool isBoy)
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_Bomb");
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playCardLast(bool isBoy, int value)
	{
		char str[10] = {0};
		sprintf(str, "%d", value);
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Card_Last_");
		filename.append(str);
		isBoy ? filename.append("_M.mp3") : filename.append("_W.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameAudio::playPlanEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("CardType_Aircraft.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playBombEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("CardType_Bomb.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playDoubleSequenceEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("CardType_Double_Line.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playRocketEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("CardType_Missile.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playStraightEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("CardType_One_Line.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameAudio::playWin()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_You_Win.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playLose()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_You_Lose.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playDispathCard()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Special_Dispatch.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

}
