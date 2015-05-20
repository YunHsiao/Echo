#include "stdafx.h"
#include "Player.h"
#include "ElementManager.h"
#include "AudioManager.h"
#include "Controller.h"

extern D3DXVECTOR3 g_vScreenCenter;

CPlayer::CPlayer(D3DXVECTOR3& pos, D3DXVECTOR3& dir, PLAYER_PARAMETER param, const SMap *map)
:m_vPos(pos)
,m_vDir(dir)
,m_fElapsedTime(0)
,m_bLeft(TRUE)
,m_param(param)
,m_pMap(map)
,m_cSound(AL_FOOTSTEP_STONE_A)
,m_pSound(AL_FOOTSTEP_STONE_A)
,m_status(PS_STANDING)
{
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

CPlayer::~CPlayer() 
{
}

void CPlayer::InitFootprints() {
	m_cSound = m_pMap->GetSoundEffect(m_vPos);
	CreateFootprint();
	CreateFootprint();
}

void CPlayer::Tick(DWORD fElapsedTime) 
{
	m_fElapsedTime += fElapsedTime;
	BOOL bDisplay(FALSE);
	
	if (m_status > PS_STANDING) {
		// Collision against the wall
		FLOAT result = m_pMap->GetArea(MT_WALL)->WallCollision
			(m_vPos + m_vDir * m_param.fFootSize, m_vDir);
		if (result < 0.f) {
			m_status = PS_STANDING;
		} else if (result > 0) m_vPos -= m_vDir * result;
		// Handle Player Event
		HandleSceneEvent();
	}

	switch (m_status) {
	case PS_DEAD:
		if (IsFinished() && m_fElapsedTime > 2000.f) {
			CElementManager::GetInstance()->Clear();
			CElementManager::GetInstance()->CreateElement(RL_WHITE, m_pMap, 3000.f, FALSE, 0xFF1010, TRUE);
			m_fElapsedTime = -10000.f;
		}
		break;
	case PS_ALIVE:
		if (IsFinished() && m_fElapsedTime > 2500.f) {
			CElementManager::GetInstance()->Clear();
			CElementManager::GetInstance()->CreateElement(RL_WHITE, m_pMap, 2500.f, FALSE, 0xFFFFFF, TRUE);
			m_fElapsedTime = -10000.f;
		}
		break;
	case PS_STANDING:
		if (m_fElapsedTime > m_param.nSneakInterval) bDisplay = TRUE;
		break;
	case PS_SNEAKING:
		m_vPos += m_vDir * (fElapsedTime * m_param.fSneakSpeed * 
			(IsWater(m_cSound)?m_param.fSwimmingDelay:1.f));
		if (m_fElapsedTime > m_param.nSneakInterval) bDisplay = TRUE;
		break;
	case PS_WALKING:
		m_vPos += m_vDir * (fElapsedTime * m_param.fWalkSpeed * 
			IsWater(m_cSound)?m_param.fSwimmingDelay:1.f);
		if (m_fElapsedTime > m_param.nWalkInterval) bDisplay = TRUE;
		break;
	case PS_RUNNING:
		m_vPos += m_vDir * (fElapsedTime * m_param.fRunSpeed * 
			IsWater(m_cSound)?m_param.fSwimmingDelay:1.f);
		if (m_fElapsedTime > m_param.nRunInterval) bDisplay = TRUE;
		break;
	}
	if (bDisplay) CreateFootprint();
}

void CPlayer::HandleSceneEvent() {
	if (m_pMap->ValidateArea(MT_EXIT, m_vPos)) {
		m_status = PS_ALIVE;
		m_fElapsedTime = 0;
		CAudioManager::GetInstance()->ChannelPlay(AL_DOOROPENCLOSE);
		CAudioManager::GetInstance()->ChannelStop(AL_MONSTER);
		CElementManager::GetInstance()->CreateCircle(m_vPos, 1.f, m_pMap, 5000.f,
			500.f, 72, 0xFFFFFF, TRUE);
		CController::GetInstance()->BindPlayer(NULL);
	} else if (m_pMap->ValidateArea(MT_TRAP, m_vPos)) {
		m_status = PS_DEAD;
		m_fElapsedTime = 0;
		CAudioManager::GetInstance()->ChannelPlay(AL_TRAP_HIT);
		CAudioManager::GetInstance()->ChannelPlay(AL_DEATH, 2);
		CAudioManager::GetInstance()->ChannelStop(AL_MONSTER);
		CElementManager::GetInstance()->CreateCircle(m_vPos, 1.f, m_pMap);
		CController::GetInstance()->BindPlayer(NULL);
	} else m_cSound = m_pMap->GetSoundEffect(m_vPos);
}

void CPlayer::CreateFootprint() 
{
	D3DXVECTOR3 nor(m_vDir.y, -m_vDir.x, 0.f), 
		pos(m_vPos+nor*m_param.fShoulderWidth*(m_bLeft?1.f:-1.f));
	BOOL c(IsWater(m_cSound));
	if (c) m_fElapsedTime = -700.f; // Slow down to fit in the swimming rhythm
	else m_fElapsedTime = 0.f;
	CElementManager::GetInstance()->CreateFootprint(pos, m_vDir, m_bLeft, m_pMap,
		c?0x1010FF:0xFFFFFF, m_param.fFootScale); // Create footprint
	m_bLeft = !m_bLeft;
	if (m_status == PS_STANDING) return;

	CElementManager::GetInstance()->CreateCircle(pos, 
		m_status == PS_SNEAKING ? SNEAKING_ECHO : WALKING_ECHO, m_pMap,
		5000.f,	500.f, 72, 0xFFFFFF, TRUE); // Create echo circle
	if (m_status == PS_SNEAKING && !c) return; 

	BOOL p(IsWater(m_pSound));
	if (p ^ c) m_cSound = c ? AL_WATER_SPLASH_ENTER : AL_WATER_SPLASH_EXIT;
	CAudioManager::GetInstance()->ChannelPlay(m_cSound, p ^ c ? 0 : 4, 
		MAX_VOLUME / (m_status == PS_SNEAKING ? 4 : 1)); // Play corresponding sound
	m_pSound = m_cSound;
}

void CPlayer::Stamp(FLOAT intensity) 
{
	if (intensity > 1.f) intensity = 1.f;
	CAudioManager::GetInstance()->ChannelPlay(AL_CLAP_QUIET - 
		static_cast<INT>(intensity * 2 + .5f), 0, intensity);
	CElementManager::GetInstance()->CreateCircle(m_vPos, intensity, m_pMap, 5000.f,
		500.f, 72, 0xFFFFFF, TRUE);
}

// For debug usage
void CPlayer::CreateLine(D3DXVECTOR3& pos, D3DXVECTOR3& dir) {
	CElementManager::GetInstance()->CreateLine(m_vPos - g_vScreenCenter + pos, dir, m_pMap);
}

void CPlayer::Die(BOOL IsTrap) {
	m_status = PS_DEAD;
	m_fElapsedTime = 0;
	CAudioManager::GetInstance()->ChannelPlay(IsTrap?AL_TRAP_HIT:AL_MONSTER_EAT);
	CAudioManager::GetInstance()->ChannelPlay(AL_DEATH, 2);
	CAudioManager::GetInstance()->ChannelStop(AL_MONSTER);
	CElementManager::GetInstance()->CreateCircle(m_vPos, 1.f, m_pMap, 5000.f, 500.f, 72, 0xFF0000);
	CController::GetInstance()->BindPlayer(NULL);
}