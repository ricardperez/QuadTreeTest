//
//  segment.h
//  QuadTreeTest
//
//  Created by Ricard Pérez del Campo on 03/06/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#ifndef __QuadTreeTest__segment__
#define __QuadTreeTest__segment__

#include <iostream>
#include "xy.h"
class Segment : public XYObject
{
private:
	XY _a;
	XY _b;
	
	bool intersectsSegment(const Segment &other) const;
	
public:
	Segment(XY a, XY b) : _a(a), _b(b){};
	bool isContainedOrIntersectsBoundary(const Boundary &b) const;
	bool operator<(const XYObject &other) const;
	bool operator<(const Segment &other) const;
	float distanceToPoint(XY &point) const;
	void debug();
};


inline static bool IsOnSegment(double xi, double yi, double xj, double yj,
                        double xk, double yk) {
	return (xi <= xk || xj <= xk) && (xk <= xi || xk <= xj) &&
	(yi <= yk || yj <= yk) && (yk <= yi || yk <= yj);
}

inline static char ComputeDirection(double xi, double yi, double xj, double yj,
                             double xk, double yk) {
	double a = (xk - xi) * (yj - yi);
	double b = (xj - xi) * (yk - yi);
	return a < b ? -1 : a > b ? 1 : 0;
}

/** Do line segments (x1, y1)--(x2, y2) and (x3, y3)--(x4, y4) intersect? */
inline bool DoLineSegmentsIntersect(double x1, double y1, double x2, double y2,
                             double x3, double y3, double x4, double y4) {
	char d1 = ComputeDirection(x3, y3, x4, y4, x1, y1);
	char d2 = ComputeDirection(x3, y3, x4, y4, x2, y2);
	char d3 = ComputeDirection(x1, y1, x2, y2, x3, y3);
	char d4 = ComputeDirection(x1, y1, x2, y2, x4, y4);
	return (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
			((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) ||
	(d1 == 0 && IsOnSegment(x3, y3, x4, y4, x1, y1)) ||
	(d2 == 0 && IsOnSegment(x3, y3, x4, y4, x2, y2)) ||
	(d3 == 0 && IsOnSegment(x1, y1, x2, y2, x3, y3)) ||
	(d4 == 0 && IsOnSegment(x1, y1, x2, y2, x4, y4));
}

#endif /* defined(__QuadTreeTest__segment__) */
