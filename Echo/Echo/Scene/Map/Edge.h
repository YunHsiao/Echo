#pragma once

struct SEdge {

	D3DXVECTOR3 pa, pb, normal; // pointing away from origin
	FLOAT distance;
	BOOL inside; // is normal pointing to the inside of polygon
	SEdge(const D3DXVECTOR3& a, const D3DXVECTOR3& b) 
		:pa(a)
		,pb(b)
		,inside(FALSE)
		{
			D3DXVec3Normalize(&normal, &(pb - pa));
			normal *= D3DXVec3Dot(&normal, &(-pa));
			normal -= (-pa);
			distance = D3DXVec3Length(&normal);
			normal /= distance;
		}

	FLOAT GetDistance(const D3DXVECTOR3& point) const {
		return D3DXVec3Dot(&point, &normal) - distance;
	}

	D3DXVECTOR3 GetProjectPoint(const D3DXVECTOR3& point) const {
		return point - normal * GetDistance(point);
	}

	BOOL AimingInside(const D3DXVECTOR3& dir) const {
		return D3DXVec3Dot(&normal, &dir) > 0.f == inside;
	}

	D3DXVECTOR3 GetReflectDirection(const D3DXVECTOR3& dir) const {
		if (AimingInside(dir)) return dir;
		return dir - normal * D3DXVec3Dot(&normal, &dir) * 2;
	}

	D3DXVECTOR3 GetRestrainedDirection(const D3DXVECTOR3& dir) const {
		if (AimingInside(dir)) return dir;
		D3DXVECTOR3 line(normal.y, -normal.x, 0.f);
		return line * D3DXVec3Dot(&line, &dir);
	}

	FLOAT GetXByY(const FLOAT y) const {
		if ((INT)pb.y == (INT)pa.y) return max(pa.x, pb.x);
		return (y-pa.y)*(pb.x-pa.x)/(pb.y-pa.y)+pa.x;
	}

	BOOL IsXaSmaller() {
		return pa.x < pb.x;
	}

	BOOL ValidateX(const FLOAT x, const FLOAT x1, const FLOAT x2) const {
		if (x2 < x1) {
			if (x1 < x || x < x2) return FALSE;
		} else if (x1 < x2) {
			if (x < x1 || x2 < x) return FALSE;
		} else {
			if (fabsf(x - x1) > 1e-3f) return FALSE;
		}
		return TRUE;
	}

	BOOL ValidateY(const FLOAT y, const FLOAT y1, const FLOAT y2) const {
		if (y2 < y1) {
			if (y1 < y || y < y2) return FALSE;
		} else if (y1 < y2) {
			if (y < y1 || y2 < y) return FALSE;
		} else {
			if (fabsf(y - y1) > 1e-3f) return FALSE;
		}
		return TRUE;
	}

	BOOL Validate(const D3DXVECTOR3& point) const {
		return ValidateX(point.x, pa.x, pb.x) && ValidateY(point.y, pa.y, pb.y);
	}

	BOOL Approching(const D3DXVECTOR3& pos) const {
		return fabsf(GetDistance(pos)) < 4.f && Validate(GetProjectPoint(pos));
	}

	BOOL Collided(const D3DXVECTOR3& pos) const {
		FLOAT dis = GetDistance(pos);
		// when get close to current edge
		if (Approching(pos)) {
			// and outside the polygon
			if (dis < 0.f == inside) return TRUE;
		}
		return FALSE;
	}

	BOOL Intersected(const D3DXVECTOR3& pr, const D3DXVECTOR3& po) const {
		FLOAT k1((pa.y-pb.y)/(pa.x-pb.x)), k2((pr.y-po.y)/(pr.x-po.x));
		if (k1 - k2 < 1e-5f) return FALSE;
		FLOAT a1(pa.x), b1(pa.y), a2(pb.x), b2(pb.y), c1(pr.x), d1(pr.y), c2(po.x), d2(po.y);
		D3DXVECTOR3 p(0.f, 0.f, 0.f);
		p.x=((a2-a1)*(c2-c1)*(d2-b2)+(b2-b1)*(c2-c1)*a2-(d2-d1)*(a2-a1)*c2)/((b2-b1)*(c2-c1)-(d2-d1)*(a2-a1)); 
		p.y=(b2-b1)/(a2-a1)*(p.x-a2)+b2;
		//p.x = ((pb.x-pa.x)*(po.x-pr.x)*(po.y-pb.y)+(pb.y-pa.y)*(po.x-pr.x)*pb.x-(po.y-pr.y)*(pb.x-pa.x)*po.x)/((pb.y-pa.y)*(po.x-pr.x)-(po.y-pr.y)*(pb.x-pa.x));
		//p.y = (pb.y-pa.y)/(pb.x-pa.x)*(p.x-pb.x)+pb.y;
		/*D3DXVECTOR3 p((k1*pa.x-k2*pr.x+pr.y-pa.y)/(k1-k2), 0, 0);
		p.y = pa.y+(p.x-pa.x)*k1;*/
		if (Validate(p) && ValidateX(p.x, pr.x, po.x) && ValidateY(p.y, pr.y, po.y))
			return TRUE;
		return FALSE;
	}
};