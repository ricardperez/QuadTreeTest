//
//  quad_tree.h
//  QuadTreeTest
//
//  Created by Ricard Pérez del Campo on 03/06/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#ifndef __QuadTreeTest__quad_tree__
#define __QuadTreeTest__quad_tree__

#include <iostream>
#include <set>
#include "xy.h"

class QuadTree
{
private:
	int _capacity;
	Boundary _boundary;
	std::set<XYObject *> _objects;
	
	QuadTree *_northWest;
	QuadTree *_northEast;
	QuadTree *_southWest;
	QuadTree *_southEast;
	
	void subdivide();
	void getObjectsCloseToBoundary(Boundary boundary, std::set<XYObject *> &objects);
	
public:
	QuadTree(float centerX, float centerY, float halfWidth, float halfHeight);
	~QuadTree();
	bool addObject(XYObject *object);
	std::set<XYObject *> getObjectsCloseToBoundary(float centerX, float centerY, float halfWidth, float halfHeight);
	void clear();
	bool containsObject(XYObject *object);
};

#endif /* defined(__QuadTreeTest__quad_tree__) */
