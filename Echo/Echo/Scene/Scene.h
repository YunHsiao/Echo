#pragma once
#include "Player.h"
#include "Monster.h"
#include "tinyxml2.h"
#include "Map.h"

class CScene {

public:
	CScene(UINT level);
	~CScene();
	BOOL Tick(DWORD fElapsedTime);
	void Render(BOOL highQuality);
	BOOL IsAlive() { return *m_status; }
	typedef std::vector<CPlayer*> Players;
	typedef std::vector<CPlayer*>::iterator PlayersIt;
	typedef std::vector<CPlayer*>::reverse_iterator PlayersRIt;
	typedef std::vector<CMonster*> Monsters;
	typedef std::vector<CMonster*>::iterator MonstersIt;
	typedef std::multimap<UINT, D3DXVECTOR3> Items;
	typedef std::multimap<UINT, D3DXVECTOR3>::iterator ItemsIt;
	inline const SMap *GetMap() { return &m_map; }
	inline UINT GetLevel() { return m_level; }

protected:
	void InitScene();
	void InitMap(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *node, 
		tinyxml2::XMLElement *child, const CHAR *name, UINT type);
	const PLAYER_STATUS* m_status;
	const D3DXVECTOR3 *m_vPos;
	Players m_players;
	Monsters m_monsters;
	Points m_points;
	Items m_tips, m_decorations;
	SMap m_map;
	DWORD m_lElapsedTime;
	INT m_lRemaining;
	UINT m_uiBGM;
	SMap m_Map;
	UINT m_level;
	CHAR m_hint[16];
};