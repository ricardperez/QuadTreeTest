//
//  xy.h
//  QuadTreeTest
//
//  Created by Ricard Pérez del Campo on 03/06/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#ifndef __QuadTreeTest__xy__
#define __QuadTreeTest__xy__

#include <iostream>
#include <cmath>

#define _ERR_ZERO_ 0.000001f

struct XY
{
	float x;
	float y;
	XY(float x_, float y_) : x(x_), y(y_){};
	XY operator+(const XY &other) const { return XY(this->x+other.x, this->y+other.y); };
	XY operator-(const XY &other) const { return XY(this->x-other.x, this->y-other.y); };
	XY operator*(const float val) const { return XY(this->x*val, this->y*val); };
	bool operator<(const XY &other) const { return ((this->x < other.x) || ((fabsf(this->x-other.y) < _ERR_ZERO_) && (this->y < other.y))); };
	bool operator==(const XY &other) const { return ((fabsf(this->x - other.x) < _ERR_ZERO_) && (fabsf(this->y - other.y) < _ERR_ZERO_)); };
	bool operator!=(const XY &other) const { return (!this->operator==(other)); };
	bool operator>(const XY &other) const { return (!this->operator<(other) && this->operator!=(other)); };
	bool operator<=(const XY &other) const { return (this->operator<(other) || this->operator==(other)); };
	bool operator>=(const XY &other) const { return (this->operator>(other) || this->operator==(other)); };
	float dot(XY other) { return (this->x*other.x + this->y*other.y); };
};

struct Boundary
{
	XY center;
	XY halfDimension;
	Boundary(XY center_, XY halfDimension_) : center(center_), halfDimension(halfDimension_){};
	bool containsPoint(XY point) const
	{
		return ((point.x > center.x-halfDimension.x) &&
				(point.x < center.x+halfDimension.x) &&
				(point.y > center.y-halfDimension.y) &&
				(point.y < center.y+halfDimension.y));
	};
	bool intersectsOrContainsBoundary(Boundary b) const
	{
		return (b.containsPoint(this->center) ||
				this->containsPoint(b.center) ||
				(this->containsPoint(XY(b.center.x-b.halfDimension.x, b.center.y-b.halfDimension.y)) ||
				 this->containsPoint(XY(b.center.x+b.halfDimension.x, b.center.y-b.halfDimension.y)) ||
				 this->containsPoint(XY(b.center.x+b.halfDimension.x, b.center.y+b.halfDimension.y)) ||
				 this->containsPoint(XY(b.center.x-b.halfDimension.x, b.center.y+b.halfDimension.y))));
	}
};

class XYObject
{

public:
	virtual bool isContainedOrIntersectsBoundary(const Boundary &b) const = 0;
	virtual bool operator<(const XYObject &other) const = 0;
	virtual float distanceToPoint(XY &point) const = 0;
	virtual void debug() = 0;
};

#endif /* defined(__QuadTreeTest__xy__) */
