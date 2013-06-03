//
//  segment.cpp
//  QuadTreeTest
//
//  Created by Ricard Pérez del Campo on 03/06/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#include "segment.h"
#include <cmath>

inline bool Segment::intersectsSegment(const Segment &other) const
{
	return DoLineSegmentsIntersect(this->_a.x, this->_a.y, this->_b.x, this->_b.y, other._a.x, other._a.y, other._b.x, other._b.y);
	
}

bool Segment::isContainedOrIntersectsBoundary(const Boundary &b) const
{
	bool contained = (b.containsPoint(this->_a) || b.containsPoint(this->_b));
	bool intersects = false;
	if (!contained)
	{
		XY boundaryBottomLeft = XY(b.center.x - b.halfDimension.x, b.center.y + b.halfDimension.y);
		XY boundaryBottomRight = XY(b.center.x + b.halfDimension.x, b.center.y + b.halfDimension.y);
		XY boundaryTopLeft = XY(b.center.x - b.halfDimension.x, b.center.y - b.halfDimension.y);
		XY boundaryTopRight = XY(b.center.x + b.halfDimension.x, b.center.y - b.halfDimension.y);
		
		intersects = (this->intersectsSegment(Segment(boundaryBottomLeft, boundaryBottomRight)) ||
					  this->intersectsSegment(Segment(boundaryBottomRight, boundaryTopRight)) ||
					  this->intersectsSegment(Segment(boundaryTopRight, boundaryTopLeft)) ||
					  this->intersectsSegment(Segment(boundaryTopLeft, boundaryBottomLeft)));
	}
	
	return (contained || intersects);
};

bool Segment::operator<(const XYObject &other) const
{
	return this->operator<((const Segment &)other);
}

bool Segment::operator<(const Segment &other) const
{
	return ((this->_a < other._a) || (this->_a == other._a && this->_b < other._b));
}

float Segment::distanceToPoint(XY &point) const
{
	const float l2 = ((this->_b.x - this->_a.x)*(this->_b.x - this->_a.x) + (this->_b.y - this->_a.y)*(this->_b.y - this->_a.y));
	if (fabsf(l2) < _ERR_ZERO_)
	{
		return sqrtf((point.x - this->_a.x) * (point.x - this->_a.x) + (point.y - this->_a.y) * (point.y - this->_a.y));
	}
	
	// Consider the line extending the segment, parameterized as _a + t (_b - _a).
	// We find projection of point p onto the line.
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	const float t = (((point - this->_a).dot(this->_b - this->_a)) / l2);
	if (fabsf(t) < _ERR_ZERO_)
	{
		// Beyond the '_a' end of the segment
		return sqrtf((point.x - this->_a.x) * (point.x - this->_a.x) + (point.y - this->_a.y) * (point.y - this->_a.y));
	} else if (t > 1.0)
	{
		 // Beyond the '_b' end of the segment
		return sqrtf((point.x - this->_b.x) * (point.x - this->_b.x) + (point.y - this->_b.y) * (point.y - this->_b.y));
	}
	
	const XY projection = (this->_a + (this->_b - this->_a)*t);  // Projection falls on the segment
	return sqrtf((point.x - projection.x) * (point.x - projection.x) + (point.y - projection.y) * (point.y - projection.y));
}

void Segment::debug()
{
	std::cout << "A = (" << this->_a.x << ", " << this->_a.y << ")  -  B = (" << this->_b.x << ", " << this->_b.y << ")" << std::endl;
}