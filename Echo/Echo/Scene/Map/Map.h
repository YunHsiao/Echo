#pragma once
#include "Polygon.h"

enum MAP_TYPE {
	// For Render Effect
	MT_WALL,
	MT_EXIT,
	MT_TRAP,
	MT_WATER,
	MT_TRIGGER,
	// For Sound Effect
	MT_DIRT,
	MT_GRITTY,
	MT_STONE,
	MT_STONE_ALT,
	MT_WET,
	MT_WOOD,
	MT_COUNT
};

typedef std::multimap<UINT, SPolygon> MapData;
typedef std::multimap<UINT, SPolygon>::iterator MapDataIt;
typedef std::multimap<UINT, SPolygon>::const_iterator MapDataCIt;

struct SMap {
	inline void AddMonster(D3DXVECTOR3* pos) { mp.push_back(pos); }
	inline void clear() { map.clear(); }
	inline UINT count(UINT type) const { return map.count(type); }
	inline MapDataCIt insert(UINT type, SPolygon& area) 
	{ return map.insert(std::make_pair(type, area)); }
	inline const SPolygon *GetArea(UINT type, UINT index = 0) const {
		MapDataCIt it = begin(type);
		UINT i = 0;
		while (it != end(type) && i++ < index) ++it;
		return &it->second;
	}
	UINT GetSoundEffect(D3DXVECTOR3& pos) const {
		if (ValidateArea(MT_WATER, pos)) return AL_WATER_SPLASH_1;
		for (UINT t(MT_DIRT); t < MT_COUNT; ++t) 
			if (ValidateArea(t, pos)) 
				return AL_FOOTSTEP_DIRT_A+(t-MT_DIRT)*4;
		return AL_FOOTSTEP_STONE_A;
	}
	UINT GetRenderEffect(D3DXVECTOR3& pos) const {
		std::pair<MapDataCIt, MapDataCIt> p;
		for (UINT t(MT_EXIT); t < MT_DIRT; ++t)
			if (ValidateArea(t, pos)) return t;
		for (MonsterPosCIt it(mp.begin()); it != mp.end(); ++it)
			if (D3DXVec3LengthSq(&(pos - **it)) < 900.f) return MT_TRAP;
		return MT_COUNT;
	}

	BOOL ValidateArea(UINT type, D3DXVECTOR3& pos) const {
		std::pair<MapDataCIt, MapDataCIt> p(range(type));
		MapDataCIt it(p.first);
		for (; it != p.second; ++it) 
			if (it->second.Validate(pos)) return TRUE;
		return FALSE;
	}

	const SPolygon *GetArea(UINT type, D3DXVECTOR3& pos) const {
		std::pair<MapDataCIt, MapDataCIt> p(range(type));
		MapDataCIt it(p.first);
		for (; it != p.second; ++it) 
			if (it->second.Validate(pos)) return &it->second;
		return NULL;
	}

	typedef std::vector<D3DXVECTOR3*> MonsterPos;
	typedef std::vector<D3DXVECTOR3*>::iterator MonsterPosIt;
	typedef std::vector<D3DXVECTOR3*>::const_iterator MonsterPosCIt;

private:
	MonsterPos mp;
	MapData map;
	inline MapDataCIt begin(UINT type) const { return map.lower_bound(type); }
	inline MapDataCIt end(UINT type) const { return map.upper_bound(type); }
	inline std::pair<MapDataCIt, MapDataCIt> range(UINT type) const 
	{ return map.equal_range(type); }

};
