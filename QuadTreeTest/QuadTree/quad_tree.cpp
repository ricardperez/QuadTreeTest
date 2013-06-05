//
//  quad_tree.cpp
//  QuadTreeTest
//
//  Created by Ricard Pérez del Campo on 03/06/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#include "quad_tree.h"

QuadTree::QuadTree(float centerX, float centerY, float halfWidth, float halfHeight, int capacity) :
_boundary(XY(centerX, centerY), XY(halfWidth, halfHeight)), _capacity(capacity)
{
	this->_northWest = 0;
	this->_northEast = 0;
	this->_southWest = 0;
	this->_southEast = 0;
}

QuadTree::~QuadTree()
{
	delete this->_northWest;
	delete this->_northEast;
	delete this->_southWest;
	delete this->_southEast;
}

void QuadTree::subdivide()
{
	float centerX = this->_boundary.center.x;
	float centerY = this->_boundary.center.y;
	float halfWidth = this->_boundary.halfDimension.x;
	float halfHeight = this->_boundary.halfDimension.y;
	
	float quarterWidth = halfWidth*0.5f;
	float quarterHeight = halfHeight*0.5f;
	
	this->_northWest = new QuadTree(centerX - quarterWidth, centerY - quarterHeight, quarterWidth, quarterHeight, this->_capacity);
	this->_northEast = new QuadTree(centerX + quarterWidth, centerY - quarterHeight, quarterWidth, quarterHeight, this->_capacity);
	this->_southWest = new QuadTree(centerX - quarterWidth, centerY + quarterHeight, quarterWidth, quarterHeight, this->_capacity);
	this->_southEast = new QuadTree(centerX + quarterWidth, centerY + quarterHeight, quarterWidth, quarterHeight, this->_capacity);
	
	/*
	 When subdiving, we have to relocate the objects to the correct child.
	 An object may be in more than one child depending on the problem, so we are
	 not going to exclude them.
	 */
	for (std::set<XYObject *>::iterator it=this->_objects.begin(); it != this->_objects.end(); ++it)
	{
		this->_northWest->addObject(*it);
		this->_northEast->addObject(*it);
		this->_southWest->addObject(*it);
		this->_southEast->addObject(*it);
	}
	this->_objects.clear();
}

bool QuadTree::addObject(XYObject *object)
{
	if (!object->isContainedOrIntersectsBoundary(this->_boundary))
	{
		return false;
	}
	
	/*
	 If it has no childs, we will try to add it to its container. But if limit
	 of elements is reached, then this leaf will be splitted to 4 new nodes and
	 its elements will be set to the correct one and this object will be added
	 to the correct child(s).
	 */
	if (this->_northWest == 0)
	{
		if (this->_objects.size() < this->_capacity)
		{
			this->_objects.insert(object);
			return true;
		} else
		{
			this->subdivide();
		}
	}
	
	
//	return (this->_northWest->addObject(object) || this->_northEast->addObject(object) || this->_southWest->addObject(object) || this->_southEast->addObject(object));
	
	bool inNorthWest = this->_northWest->addObject(object);
	bool inNorthEast = this->_northEast->addObject(object);
	bool inSouthWest = this->_southWest->addObject(object);
	bool inSouthEast = this->_southEast->addObject(object);
	
	return (inNorthWest || inNorthEast || inSouthWest || inSouthEast);
}

std::set<XYObject *> QuadTree::getObjectsCloseToBoundary(float centerX, float centerY, float halfWidth, float halfHeight)
{
	std::set<XYObject *> returnObjects;
	Boundary boundary(XY(centerX, centerY), XY(halfWidth, halfHeight));
	this->getObjectsCloseToBoundary(boundary, returnObjects);
	return returnObjects;
}

void QuadTree::getObjectsCloseToBoundary(Boundary boundary, std::set<XYObject *> &objects)
{
	if (this->_boundary.intersectsOrContainsBoundary(boundary))
	{
		if (this->_southWest == 0)
		{
			this->_northWest->getObjectsCloseToBoundary(boundary, objects);
			this->_northEast->getObjectsCloseToBoundary(boundary, objects);
			this->_southWest->getObjectsCloseToBoundary(boundary, objects);
			this->_southEast->getObjectsCloseToBoundary(boundary, objects);
		} else
		{
			objects.insert(this->_objects.begin(), this->_objects.end());
		}
	}
}