//
//  Collision.cpp
//

#include <cassert>

#include "ObjLibrary/Vector3.h"

#include "Collision.h"

using namespace ObjLibrary;



bool Collision :: pointVsCircle (const ObjLibrary::Vector3& point,
                                 const ObjLibrary::Vector3& center,
                                 float radius)
{
	assert(radius >= 0.0f);

	return point.isDistanceXZLessThan(center, radius);
}

bool Collision :: circleVsCircle (const ObjLibrary::Vector3& center1,
                                  float radius1,
                                  const ObjLibrary::Vector3& center2,
                                  float radius2)
{
	assert(radius1 >= 0.0f);
	assert(radius2 >= 0.0f);

	return pointVsCircle(center1, center2, radius1 + radius2);
}

bool Collision :: pointVsCylinder (const ObjLibrary::Vector3& point,
                                   const ObjLibrary::Vector3& center,
                                   float radius,
                                   float half_height)
{
	assert(radius >= 0.0f);
	assert(half_height >= 0.0f);

	if(fabs(point.y - center.y) > half_height)
		return false;
	if(point.isDistanceXZGreaterThan(center, radius))
		return false;
	return true;
}

bool Collision :: cylinderVsCylinder (const ObjLibrary::Vector3& center1,
                                      float radius1,
                                      float half_height1,
                                      const ObjLibrary::Vector3& center2,
                                      float radius2,
                                      float half_height2)
{
	assert(radius1 >= 0.0f);
	assert(half_height1 >= 0.0f);
	assert(radius2 >= 0.0f);
	assert(half_height2 >= 0.0f);

	return pointVsCylinder(center1, center2, radius1 + radius2, half_height1 + half_height2);
}

