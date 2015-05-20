#include "stdafx.h"
#include "AudioManager.h"

CAudioManager CAudioManager::s_SoundManager;

CAudioManager::CAudioManager()
:m_bInitialized(FALSE)
{
}

CAudioManager::~CAudioManager()
{
	if (m_bInitialized) {
		Clear();
		::BASS_Free();
	}
}

void CAudioManager::Clear()
{
	for (SoundPoolIt it(m_SoundPool.begin()); it != m_SoundPool.end(); ++it)
		::BASS_StreamFree(it->hSound);
	m_SoundPool.clear();
}

INT CAudioManager::LoadSound(std::string& sName, DWORD flags)
{
	for (SoundPoolIt it(m_SoundPool.begin()); it != m_SoundPool.end(); ++it)
		if(sName == it->name) return it - m_SoundPool.begin();

	if (!m_bInitialized) return -1;
	HSTREAM hSound = ::BASS_StreamCreateFile(FALSE, sName.c_str(), 0, 0, flags); 
	if(hSound) {
		Sound musicinfo;
		musicinfo.name = sName;
		musicinfo.hSound = hSound;
		m_SoundPool.push_back(musicinfo);
		return m_SoundPool.size() - 1;
	}
	return -1;
}

BOOL CAudioManager::LoadSounds(std::ifstream& fileList) {
	INT cnt = -1;
	std::string fileName;
	while (getline(fileList, fileName)) {
		++cnt;
		if (LoadSound(fileName, cnt > AL_AMBIENT_D || cnt == AL_MONSTER ? 
				BASS_SAMPLE_MONO : BASS_SAMPLE_LOOP) < 0) {
			return FALSE;
		}
	}
	return TRUE;
}

HRESULT CAudioManager::Initialize(HWND hWnd, std::ifstream& fileList)
{
	HRESULT hr;
	if (FAILED(hr = ::BASS_Init(-1, 44100, BASS_DEVICE_3D, hWnd, NULL))) return hr;
	m_bInitialized = TRUE;
	LoadSounds(fileList);
	return hr;
}

BOOL CAudioManager::DeleteSound(UINT index)
{
	if (m_SoundPool.size() <= index || !m_bInitialized) return FALSE;
	BOOL b = ::BASS_StreamFree(m_SoundPool[index].hSound);
	m_SoundPool.erase(m_SoundPool.begin() + index);
	return b;
}

BOOL CAudioManager::ChannelPlay(UINT index, UINT rand, FLOAT volume, BOOL restart)
{
	UINT t(0);
	if (rand) { rand_s(&t); t %= rand; }
	if (m_SoundPool.size() <= index + t || !m_bInitialized) return FALSE;
	SetVolume(index + t, volume);
	if(NULL != m_SoundPool[index + t].hSound) 
		return ::BASS_ChannelPlay(m_SoundPool[index + t].hSound, restart);
	else
		return FALSE;
}

BOOL CAudioManager::ChannelStop(UINT index)
{
	if (m_SoundPool.size() <= index || !m_bInitialized) return FALSE;
	if(NULL != m_SoundPool[index].hSound) 
		return ::BASS_ChannelStop(m_SoundPool[index].hSound);
	else
		return FALSE;
}

BOOL CAudioManager::ChannelPause(UINT index)
{
	if (m_SoundPool.size() <= index || !m_bInitialized) return FALSE;
	if(NULL != m_SoundPool[index].hSound) 
		return ::BASS_ChannelPause(m_SoundPool[index].hSound);
	else
		return FALSE;
}

BOOL CAudioManager::SetVolume(UINT index, FLOAT volume)
{
	if (m_SoundPool.size() <= index || !m_bInitialized) return FALSE;
	return ::BASS_ChannelSetAttribute(m_SoundPool[index].hSound, 
		BASS_ATTRIB_VOL, volume);
}