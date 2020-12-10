#ifndef __GAMESOUND_H__
#define __GAMESOUND_H__

#include "Game.h"
#include "DSUtil.h"
#include <unordered_map>

enum Sound
{
	soundWhip = 1,
	soundHit = 2,
	soundCollectItem = 3,
	soundCollectWeapon = 4,
	soundStopTimer = 5,
	soundDagger = 6,
	soundDisplayMonney = 7,
	soundHurting = 8,
	soundOpenDoor = 9,
	soundBrokenBrick = 10,
	soundHolyWater = 11,
	soundSplashwater = 12,
	soundFallingDownWaterSurface = 13,
	soundGetScoreTimer = 14,
	soundGetScoreHeart = 15,
	soundAxe = 16,
	soundStopWatch = 17,
	soundInvisibilityPotion_Begin = 18,
	soundInvisibilityPotion_End = 19,
	soundHolyCross = 20,
	soundBoomerang = 21,

	musicState1 = 51,

	music_PhantomBat = 53,
};

class GameSound
{
private:
	unordered_map<Sound, CSound*> _SoundMap;

public:
	static GameSound* __Instance; // Singleton
	static GameSound* GetInstance();

	CSoundManager* dsound;

	void Init();
	GameSound();

	void AddSound(Sound type, LPTSTR fileName);
	void Play(Sound type, bool isLoop = false, LONG lVolume = 0L);
	void Stop(Sound type);
	void LoadSoundResources();

	bool GetIsSoundPlaying(Sound type);

	void StopAll(); // Stop toàn bộ âm thanh, dùng cho khi replay game
};

#endif