//
//  main.cpp
//  QuadTreeTest
//
//  Created by Ricard Pérez del Campo on 03/06/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#include <iostream>
#include "quad_tree.h"
#include "segment.h"
#include <set>
#include <stdlib.h>
#include <time.h>
#include <cfloat>

XYObject *GetClosestSegmentToPoint(const std::set<XYObject *> &segments, XY point)
{
	float distance = FLT_MAX;
	XYObject *closestSegment = 0;
	
	for (std::set<XYObject *>::iterator it = segments.begin(); it != segments.end(); ++it)
	{
		XYObject *segment = *it;
		float currDistance = segment->distanceToPoint(point);
		if (currDistance < distance)
		{
			distance = currDistance;
			closestSegment = segment;
		}
	}
	
	return closestSegment;
}

int main(int argc, const char * argv[])
{
	srand((uint)time(0));

	float size = 1000.0f;
	int nSegments = (argc <= 1 ? 10000 : atoi(argv[1]));
	int nPoints = (argc <= 2 ? 10000 : atoi(argv[2]));
	
	std::cout << "Segments to create: " << nSegments << std::endl;
	std::cout << "Points to be searched: " << nPoints << std::endl;
	std::cout << std::endl;
	
	clock_t instantiateSetStart = clock();
	std::set<XYObject *> segmentsSet;
	for (int i=0; i<nSegments; ++i)
	{
		float ax = (((float)rand() / RAND_MAX) * size);
		float ay = (((float)rand() / RAND_MAX) * size);
		float bx = (((float)rand() / RAND_MAX) * size);
		float by = (((float)rand() / RAND_MAX) * size);
		
		Segment *s = new Segment(XY(ax,ay), XY(bx,by));
		segmentsSet.insert(s);
	}
	clock_t instantiateSetFinish = clock();
	
	clock_t searchSetStart = clock();
	for (int i=0; i<nPoints; ++i)
	{
		float x = (((float)rand() / RAND_MAX) * size);
		float y = (((float)rand() / RAND_MAX) * size);
		XY point(x,y);
		
		GetClosestSegmentToPoint(segmentsSet, point);
	}
	clock_t searchSetFinish = clock();
	
	
	clock_t instantiateQuadTreeStart = clock();
	QuadTree quadTree(size*0.5f, size*0.5f, size*0.5f, size*0.5f);
	for (int i=0; i<nSegments; ++i)
	{
		float ax = (((float)rand() / RAND_MAX) * size);
		float ay = (((float)rand() / RAND_MAX) * size);
		float bx = (((float)rand() / RAND_MAX) * size);
		float by = (((float)rand() / RAND_MAX) * size);
		
		Segment *s = new Segment(XY(ax,ay), XY(bx,by));
		quadTree.addObject(s);
	}
	clock_t instantiateQuadTreeFinish = clock();
	
	clock_t searchQuadTreeStart = clock();
	for (int i=0; i<nPoints; ++i)
	{
		float x = (((float)rand() / RAND_MAX) * size);
		float y = (((float)rand() / RAND_MAX) * size);
		XY point(x,y);
		
		std::set<XYObject *> closerSegments = quadTree.getObjectsCloseToBoundary(x, y, 0.0f, 0.0f);
		GetClosestSegmentToPoint(closerSegments, point);
	}
	clock_t searchQuadTreeFinish = clock();
	
	
	
	float instantiateSetSeconds = ((float)(instantiateSetFinish - instantiateSetStart) / CLOCKS_PER_SEC);
	float searchSetSeconds = ((float)(searchSetFinish - searchSetStart) / CLOCKS_PER_SEC);
	float instantiateQuadTreeSeconds = ((float)(instantiateQuadTreeFinish - instantiateQuadTreeStart) / CLOCKS_PER_SEC);
	float searchQuadTreeSeconds = ((float)(searchQuadTreeFinish - searchQuadTreeStart) / CLOCKS_PER_SEC);
	
	std::cout << "Instantiate set: " << instantiateSetSeconds << " seconds  -> In 1 second: " << (nSegments/instantiateSetSeconds) << " added segments" << std::endl;
	std::cout << "Search in set: " << searchSetSeconds << " seconds  -> In 1 second: " << (nPoints/searchSetSeconds) << " searches" << std::endl;
	std::cout << "Set total time: " << (instantiateSetSeconds + searchSetSeconds) << " seconds" << std::endl;
	std::cout << std::endl;
	std::cout << "Instantiate quadtree: " << instantiateQuadTreeSeconds << " seconds  -> In 1 second: " << (nSegments/instantiateQuadTreeSeconds) << " added segments" << std::endl;
	std::cout << "Search in quad tree: " << searchQuadTreeSeconds << " seconds  -> In 1 second: " << (nPoints/searchQuadTreeSeconds) << " searches" << std::endl;
	std::cout << "Quadtree total time: " << (instantiateSetSeconds + searchSetSeconds) << " seconds" << std::endl;
	
	
	for (std::set<XYObject *>::iterator it = segmentsSet.begin(); it != segmentsSet.end(); ++it)
	{
		delete *it;
	}
	segmentsSet.clear();
	
    return 0;
}

