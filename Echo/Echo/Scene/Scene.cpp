#include "stdafx.h"
#include "Scene.h"
#include "Controller.h"
#include "ElementManager.h"
#include "AudioManager.h"

extern D3DXVECTOR3 g_vScreenCenter;

void CScene::InitScene() {
	tinyxml2::XMLDocument doc; CHAR path[256];
	sprintf_s(path, 256, ".\\Resources\\Scene\\SC_%u.xml", m_level);
	if (doc.LoadFile(path)) {
		m_level = 1;
		sprintf_s(path, 256, ".\\Resources\\Scene\\SC_%u.xml", m_level);
		if (doc.LoadFile(path)) {
			m_level = 0;
			if (doc.LoadFile("SC_Example.xml")) {
				HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_SCENE), RT_RCDATA);
				DWORD len = SizeofResource(NULL, hRsrc);
				BYTE* lpRsrc = (BYTE*)LoadResource(NULL, hRsrc);
				FILE *file = NULL;
				fopen_s(&file, "SC_Example.xml", "w");
				fwrite(lpRsrc, len, 1, file);
				fclose(file);
				if (doc.LoadFile("SC_Example.xml")) return;
				FreeResource(lpRsrc);
			}
		} else return;
	}
	tinyxml2::XMLElement *node(NULL), *child(NULL);
	D3DXVECTOR3 pos(0,0,0), dir(0,0,0); UINT id(0);
	InitMap(doc, node, child, "Wall", MT_WALL);
	InitMap(doc, node, child, "Exit", MT_EXIT);
	InitMap(doc, node, child, "Trap", MT_TRAP);
	InitMap(doc, node, child, "Water", MT_WATER);
	InitMap(doc, node, child, "Trigger", MT_TRIGGER);
	InitMap(doc, node, child, "Dirt", MT_DIRT);
	InitMap(doc, node, child, "Gritty", MT_GRITTY);
	InitMap(doc, node, child, "Stone", MT_STONE);
	InitMap(doc, node, child, "StoneAlt", MT_STONE_ALT);
	InitMap(doc, node, child, "Wet", MT_WET);
	InitMap(doc, node, child, "Wood", MT_WOOD);
	node = doc.FirstChildElement("Tips");
	if (node) {
		child = node->FirstChildElement();
		while (child) {
			id = atoi(child->Attribute("rid"));
			pos.x = (FLOAT) atoi(child->Attribute("x"));
			pos.y = (FLOAT) atoi(child->Attribute("y"));
			m_tips.insert(std::make_pair(id, pos));
			child = child->NextSiblingElement();
		}
	}
	node = doc.FirstChildElement("Decorations");
	if (node) {
		child = node->FirstChildElement();
		while (child) {
			id = atoi(child->Attribute("sid"));
			pos.x = (FLOAT) atoi(child->Attribute("x"));
			pos.y = (FLOAT) atoi(child->Attribute("y"));
			m_decorations.insert(std::make_pair(id, pos));
			child = child->NextSiblingElement();
		}
	}
	node = doc.FirstChildElement("Player");
	if (node) {
		PLAYER_PARAMETER pp;
		pos.x = (FLOAT) atoi(node->FirstChildElement("Position")->Attribute("x"));
		pos.y = (FLOAT) atoi(node->FirstChildElement("Position")->Attribute("y"));
		dir.x = (FLOAT) atoi(node->FirstChildElement("Direction")->Attribute("x"));
		dir.y = (FLOAT) atoi(node->FirstChildElement("Direction")->Attribute("y"));
		m_players.push_back(new CPlayer(pos, dir, pp, GetMap()));
		CController::GetInstance()->BindPlayer(m_players[0]);
		m_status = m_players[0]->GetStatus();
		m_vPos = m_players[0]->GetScreenCoord();
	}
	node = doc.FirstChildElement("Monster");
	if (node) {
		MONSTER_PARAMETER mp;
		child = node->FirstChildElement();
		while (child) {
			pos.x = (FLOAT) atoi(child->Attribute("x"));
			pos.y = (FLOAT) atoi(child->Attribute("y"));
			m_monsters.push_back(new CMonster(pos, mp, GetMap(), m_status, m_vPos));
			m_map.AddMonster(&m_monsters.back()->GetCoord());
			child = child->NextSiblingElement();
		}
	}
	node = doc.FirstChildElement("BGM");
	if (node) {
		m_uiBGM = atoi(node->FirstChild()->Value());
		CAudioManager::GetInstance()->ChannelPlay(AL_AMBIENT_A + m_uiBGM);
	}
}

CScene::CScene(UINT level)
:m_status(NULL)
,m_vPos(NULL)
,m_lElapsedTime(3000)
,m_lRemaining(30000)
,m_level(level)
,m_uiBGM(0)
{
	CElementManager::GetInstance()->CreateElement(RL_QUALITY, GetMap(), 0.f, FALSE, 0xFFFFFF, TRUE);
	InitScene();
}

CScene::~CScene()
{
	CController::GetInstance()->BindPlayer(NULL);
	for (PlayersIt it(m_players.begin()); it != m_players.end(); ++it)
		SAFE_DELETE(*it)
	m_players.clear();
	for (MonstersIt it(m_monsters.begin()); it != m_monsters.end(); ++it)
		SAFE_DELETE(*it)
	m_monsters.clear();
	CAudioManager::GetInstance()->ChannelStop(AL_AMBIENT_A + m_uiBGM);
}

void CScene::InitMap(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *node, 
					 tinyxml2::XMLElement *child, const CHAR *name, UINT type) 
{
	D3DXVECTOR3 pos(0, 0, 0);
	node = doc.FirstChildElement(name);
	if (node) {
		child = node->FirstChildElement();
		while (child) {
			pos.x = (FLOAT) atoi(child->Attribute("x"));
			pos.y = (FLOAT) atoi(child->Attribute("y"));
			m_points.push_back(pos);
			child = child->NextSiblingElement();
		}		
		m_map.insert(type, SPolygon(m_points));
		m_points.clear();
	}
}

BOOL CScene::Tick(DWORD fElapsedTime) {
	m_lElapsedTime += fElapsedTime;
	if (m_lRemaining > 0) m_lRemaining -= fElapsedTime;
	for (PlayersRIt it(m_players.rbegin()); it != m_players.rend(); ++it)
		(*it)->Tick(fElapsedTime);
	if (*m_status >= PS_STANDING) {
		if (m_lRemaining < 1000) m_players[0]->Die(TRUE);
		for (MonstersIt it(m_monsters.begin()); it != m_monsters.end(); ++it)
			if ((*it)->Tick(fElapsedTime, 
				CElementManager::GetInstance()->IsEchoInside((*it)->GetCoord())) && *m_status) 
			m_players[0]->Die(FALSE);
	}
	CElementManager::GetInstance()->Tick(fElapsedTime);
	if (m_lElapsedTime > 3000 && *m_status >= PS_STANDING) {
		for (ItemsIt it = m_tips.begin(); it != m_tips.end(); ++it)
			CElementManager::GetInstance()->CreateElement(it->first, GetMap(), 5000.f,
			FALSE, 0xFFFFFF, FALSE, it->second);
		for (ItemsIt it = m_decorations.begin(); it != m_decorations.end(); ++it) {
			CElementManager::GetInstance()->CreateCircle(it->second, SNEAKING_ECHO, GetMap());
			CAudioManager::GetInstance()->ChannelPlay(it->first);
		}
		m_lElapsedTime = 0;
	}
	return *m_status < PS_STANDING && CElementManager::GetInstance()->IsGone();
}

void CScene::Render(BOOL highQuality) {
	sprintf_s(m_hint, 16, "Ê£ÓàÊ±¼ä£º%ld", m_lRemaining/1000);
	CDirect3D9::GetInstance()->DrawText(m_hint);
	CElementManager::GetInstance()->Render(*m_vPos - g_vScreenCenter, highQuality);
}