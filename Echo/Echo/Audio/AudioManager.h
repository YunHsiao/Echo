#pragma once
#include "bass.h"

class CAudioManager
{
private:
	struct Sound
	{
		std::string name;
		HSTREAM hSound;
	};

public:
	static CAudioManager* GetInstance() { return &s_SoundManager; }

public:
	CAudioManager();
	~CAudioManager();

	typedef std::vector<Sound> SoundPool;
	typedef std::vector<Sound>::iterator SoundPoolIt;
	typedef std::vector<Sound>::size_type SoundIndex;
	HRESULT Initialize(HWND hWnd, std::ifstream& fileList);
	void Clear();

	INT LoadSound(std::string& sName, DWORD flags = BASS_SAMPLE_MONO);
	BOOL DeleteSound(UINT index);

	BOOL ChannelPlay(UINT index, UINT rand = 0, FLOAT volume = MAX_VOLUME, BOOL restart = TRUE);
	BOOL ChannelStop(UINT index);
	BOOL ChannelPause(UINT index);
	BOOL SetVolume(UINT index, FLOAT volume);

private:
	BOOL LoadSounds(std::ifstream& fileList);
	SoundPool m_SoundPool;
	BOOL m_bInitialized;

private:
	static CAudioManager s_SoundManager;
};