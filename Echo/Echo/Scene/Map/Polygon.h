#pragma once
#include "Edge.h"

typedef std::vector<SEdge> Edges;
typedef std::vector<SEdge>::iterator EdgesIt;
typedef std::vector<SEdge>::const_iterator EdgesCIt;
typedef std::vector<SEdge>::difference_type EdgesOffset;

struct SPolygon {

	enum COLLISION_STATUS {
		CS_COLLIDE = 0,
		CS_VALID = -1,
		CS_THROUGH = -2
	};

	Edges edges;
	D3DXVECTOR3 center;

	inline const D3DXVECTOR3& GetCenterPoint() const { return center; }

	BOOL Validate(const D3DXVECTOR3& point) const {
		BOOL valid(FALSE);
		for (EdgesCIt it(edges.begin()); it != edges.end(); ++it) {
			if (!(it->ValidateY(point.y, it->pa.y, it->pb.y))) continue;
			if (it->GetXByY(point.y) < point.x) valid = !valid;
		}
		return valid;
	}

	EdgesOffset DetectCollision(const D3DXVECTOR3& pos) const {
		EdgesCIt it(edges.begin());
		if (Validate(pos)) return CS_VALID;
		for (; it != edges.end(); ++it) 
			if (it->Collided(pos)) return it - edges.begin();
		return CS_THROUGH;
	}

	BOOL ReflectCollision(D3DXVECTOR3& pos, D3DXVECTOR3& dir, BOOL ready) const {
		EdgesOffset offset = DetectCollision(pos);
		if (offset == CS_VALID) return TRUE;
		if (offset == CS_THROUGH) { if (ready) dir = -dir; return FALSE; }
		EdgesCIt it = edges.begin() + offset;
		BOOL inside = it->AimingInside(dir);
		if (inside && !ready) { dir = -dir; inside = !inside; }
		if (!inside) {
			// rollback
			FLOAT dis(fabsf(it->GetDistance(pos)));
			pos -= dir * dis;
			// reflect
			 dir = it->GetReflectDirection(dir);
		}
		return TRUE;
	}

	BOOL ReflectCollision(D3DXVECTOR3& pos, D3DXVECTOR3& dir) const {
		EdgesOffset offset = DetectCollision(pos);
		if (offset == CS_VALID) return TRUE;
		if (offset == CS_THROUGH) return FALSE;
		EdgesCIt it = edges.begin() + offset;
		if (!it->AimingInside(dir)) {
			// rollback
			FLOAT dis(fabsf(it->GetDistance(pos)));
			pos -= dir * dis;
			// reflect
			 dir = it->GetReflectDirection(dir);
		}
		return TRUE;
	}
	
	// return zero if can move, negative if went outside, offset if collided.
	FLOAT WallCollision(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir) const {
		EdgesOffset offset = DetectCollision(pos);
		if (offset == CS_THROUGH) return CS_THROUGH;
		else if (offset == CS_VALID) return 0.f; // 1e-10f
		EdgesCIt it = edges.begin() + offset;
		if (it->AimingInside(dir)) return 0.f;
		return fabsf(it->GetDistance(pos));
	}

	SPolygon(const Points& points) 
	{
		center = D3DXVECTOR3(0.f, 0.f, 0.f);
		for (PointsCIt i(points.begin()), j(points.end()-1); i != points.end(); j = i++) {
			edges.push_back(SEdge(*i, *j));
			center += *i;
		}
		center /= (FLOAT) edges.size();
		// None-extreme points process.
		for (EdgesIt i(edges.begin()), j(edges.end()-1); i != edges.end(); j = i++) {
			INT ta(static_cast<INT>(i->pa.y - i->pb.y)), 
				tb(static_cast<INT>(j->pb.y - j->pa.y));
			if (ta * tb <= 0) {
				if (!tb) {
					i->pb.y += (ta > 0.f ? .01f : -.01f);
					j->pa.x += (j->IsXaSmaller() ? -1.f : 1.f);
				} else {
					j->pa.y += (tb > 0.f ? .01f : -.01f);
					if (!ta) i->pb.x += (i->IsXaSmaller() ? -1.f : 1.f);
					else i->pb.x = j->GetXByY(j->pa.y);
				} 
			}
		}
		// Calculate BOOL member: is normal pointing to the inside of polygon?
		for (EdgesIt i(edges.begin()); i != edges.end(); ++i) {
			// mid = middle point of each edge with a offset of it's normal.
			D3DXVECTOR3 mid((i->pa.x + i->pb.x) / 2.f, (i->pa.y + i->pb.y) / 2.f, 0.f);
			mid += i->normal;
			i->inside = Validate(mid);
		}
	}
};