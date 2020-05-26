//
//  Collision.h
//
//  A module to detect collisions.
//

#ifndef COLLISION_H
#define COLLISION_H

#include "ObjLibrary/Vector3.h"



//
//  Collision
//
//  A namespace to detect collisions.
//

namespace Collision
{
	//
	//  pointVsCircle
	//
	//  Purpose: To determine if the specified point lies within
	//           the specified circle on the XZ plane.
	//  Parameter(s):
	//    <1> point: The point to check
	//    <2> center: The center of the cylinder
	//    <3> radius: The radius of the cylinder
	//  Precondition(s):
	//    <1> radius >= 0.0f
	//  Returns: Whether point point lies within a circle on the
	//           XZ plane with center center and radius radius.
	//           The Y-coordinates are ignored.
	//  Side Effect: N/A
	//
	bool pointVsCircle (const ObjLibrary::Vector3& point,
	                    const ObjLibrary::Vector3& center,
	                    float radius);

	//
	//  circleVsCircle
	//
	//  Purpose: To determine if the specified circles on the XZ
	//           plane intersect.
	//  Parameter(s):
	//    <1> center1: The center of the first circle
	//    <2> radius1: The radius of the first circle
	//    <3> center2: The center of the second circle
	//    <4> radius2: The radius of the second circle
	//  Precondition(s):
	//    <1> radius1 >= 0.0f
	//    <2> radius2 >= 0.0f
	//  Returns: Whether the first and second circles intersect.
	//           The Y-coordinates are ignored.
	//  Side Effect: N/A
	//
	bool circleVsCircle (const ObjLibrary::Vector3& center1,
	                     float radius1,
	                     const ObjLibrary::Vector3& center2,
	                     float radius2);

	//
	//  pointVsCylinder
	//
	//  Purpose: To determine if the specified point lies within
	//           the specified Y-aligned cylinder.
	//  Parameter(s):
	//    <1> point: The point to check
	//    <2> center: The center of the cylinder
	//    <3> radius: The radius of the cylinder
	//    <4> half_height: Half the height of the cylinder
	//  Precondition(s):
	//    <1> radius >= 0.0f
	//    <2> half_height >= 0.0f
	//  Returns: Whether point point lies within a Y-aligned
	//           cylinder with center of center, a radius of
	//           radius, and a height of half_height above and
	//           below the center.
	//  Side Effect: N/A
	//
	bool pointVsCylinder (const ObjLibrary::Vector3& point,
	                      const ObjLibrary::Vector3& center,
	                      float radius,
	                      float half_height);

	//
	//  cylinderVsCylinder
	//
	//  Purpose: To determine if the specified Y-aligned
	//           cylinders intersect.
	//  Parameter(s):
	//    <1> center1: The center of the first cylinder
	//    <2> radius1: The radius of the first cylinder
	//    <3> half_height1: Half the height of the first
	//                      cylinder
	//    <4> center2: The center of the second cylinder
	//    <5> radius2: The radius of the second cylinder
	//    <6> half_height2: Half the height of the second
	//                      cylinder
	//  Precondition(s):
	//    <1> radius1 >= 0.0f
	//    <1> half_height1 >= 0.0f
	//    <3> radius2 >= 0.0f
	//    <4> half_height2 >= 0.0f
	//  Returns: Whether the first and second Y-aligned
	//           cylinders intersect.
	//  Side Effect: N/A
	//
	bool cylinderVsCylinder (const ObjLibrary::Vector3& center1,
	                         float radius1,
	                         float half_height1,
	                         const ObjLibrary::Vector3& center2,
	                         float radius2,
	                         float half_height2);

}   // end of namespace Collision


#endif
