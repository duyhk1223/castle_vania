#include "GameSound.h"
 
GameSound* GameSound::__Instance = NULL;

GameSound* GameSound::GetInstance()
{
	if (__Instance == NULL)
		__Instance = new GameSound();
	return __Instance;
}

GameSound::GameSound()
{
	Init();
	LoadSoundResources();
}

void GameSound::Init()
{
	dsound = new CSoundManager();
	dsound->Initialize(Game::GetInstance()->GetWindowHandle(), DSSCL_PRIORITY);
	dsound->SetPrimaryBufferFormat(2, 22050, 16);
}

void GameSound::AddSound(Sound type, LPTSTR fileName)
{
	CSound* wave;
	HRESULT result = dsound->Create(&wave, fileName); // Get file âm thanh và khởi tạo

	if (result != DS_OK)
	{
		DebugOut(L"[SOUND] Load Sound that bai. eSound = %d \n", (int)type);
		return;
	}
	_SoundMap[type] = wave; // Lưu file âm thanh vào map chứa
}

void GameSound::LoadSoundResources()
{
	AddSound(Sound::soundWhip, L"Resources/sound/usingwhip.wav");
	AddSound(Sound::soundHolyWater, L"Resources/sound/holywater.wav");
	AddSound(Sound::soundHit, L"Resources/sound/hit.wav");
	AddSound(Sound::soundCollectItem, L"Resources/sound/collectitem.wav");
	AddSound(Sound::soundCollectWeapon, L"Resources/sound/collectweapon.wav");
	AddSound(Sound::soundHolyCross, L"Resources/sound/holycross.wav");
	AddSound(Sound::soundBoomerang, L"Resources/sound/08-boomerang.wav");

	AddSound(Sound::soundStopTimer, L"Resources/sound/stoptimer.wav");
	AddSound(Sound::soundDagger, L"Resources/sound/usingdagger.wav");
	AddSound(Sound::soundDisplayMonney, L"Resources/sound/display_monney.wav");
	AddSound(Sound::soundAxe, L"Resources/sound/Axe.wav");
	AddSound(Sound::soundStopWatch, L"Resources/sound/StopWatch.wav");
	AddSound(Sound::soundInvisibilityPotion_Begin, L"Resources/sound/InvisibilityPotion_Begin.wav");
	AddSound(Sound::soundInvisibilityPotion_End, L"Resources/sound/InvisibilityPotion_End.wav");

	AddSound(Sound::soundHurting, L"Resources/sound/hurting.wav");
	AddSound(Sound::soundOpenDoor, L"Resources/sound/opendoor.wav");
	AddSound(Sound::soundBrokenBrick, L"Resources/sound/broken_brick.wav");
	AddSound(Sound::soundSplashwater, L"Resources/sound/splashwater.wav");
	AddSound(Sound::soundFallingDownWaterSurface, L"Resources/sound/fallingdownwatersurface.wav");
	AddSound(Sound::soundGetScoreTimer, L"Resources/sound/getscoretime.wav");
	AddSound(Sound::soundGetScoreHeart, L"Resources/sound/getscoreheart-[AudioTrimmer.com].wav");

	AddSound(Sound::musicState1, L"Resources/music/Stage_01_Vampire_Killer.wav");

	AddSound(Sound::music_PhantomBat, L"Resources/music/Boss_Battle_Poison_Mind.wav");
}

bool GameSound::GetIsSoundPlaying(Sound type)
{
	return _SoundMap[type]->IsSoundPlaying();
}

void GameSound::Play(Sound type, bool isLoop, LONG Volume)
{
	_SoundMap[type]->Play(0, isLoop, Volume);
	//DSBPLAY_LOOPING             0x00000001 -> int = 1
}

void GameSound::Stop(Sound type)
{
	_SoundMap[type]->Stop();
}

void GameSound::StopAll()
{
	for (auto& x : _SoundMap)
	{
		if (x.second->IsSoundPlaying())
			x.second->Stop();
	}
}