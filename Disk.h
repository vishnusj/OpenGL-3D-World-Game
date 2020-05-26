//
//  Disk.h
//
//  A module to represent one disk in the world.
//

#ifndef DISK_H
#define DISK_H

#include "ObjLibrary/Vector3.h"

#include "Heightmap.h"



//
//  Disk
//
//  A class to represent one disk in the world.
//
//  Class Invariant:
//    <1> m_position.y == 0
//    <2> m_radius >= RADIUS_MIN
//    <3> m_radius <= RADIUS_MAX
//    <4> m_disk_type == calculateDiskType(m_radius)
//    <5> m_rotation_radians <= TWO_PI
//
class Disk
{
public:
	//
	//  RADIUS_MIN
	//
	//  The minimum legal radius for a disk.
	//
	static const float RADIUS_MIN;

	//
	//  RADIUS_MAX
	//
	//  The maximum legal radius for a disk.
	//
	static const float RADIUS_MAX;

	//
	//  Class Function: calculateDiskType
	//
	//  Purpose: To determine the correct type for a disk of the
	//           specified radius.
	//  Parameter(s):
	//    <1> radius: The disk radius
	//  Precondition(s):
	//    <1> radius >= RADIUS_MIN
	//    <2> radius <= RADIUS_MAX
	//  Returns: The disk type for the disk.
	//  Side Effect: N/A
	//
	static unsigned int calculateDiskType (float radius);

	//
	//  Class Function: isModelsLoaded
	//
	//  Purpose: To determine if the models for the Disk class
	//           have been loaded.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the Disk class has been initialized.
	//  Side Effect: N/A
	//
	static bool isModelsLoaded ();

	//
	//  Class Function: loadModels
	//
	//  Purpose: To load the models for the Disk class.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> !isModelsLoaded()
	//  Returns: The Disk is initialized.
	//  Side Effect: N/A
	//
	static void loadModels ();

public:
	//
	//  Default Constructor
	//
	//  Purpose: To create a new Disk with default values.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new Disk is created at the origin with
	//               radius RADIUS_MIN.
	//
	Disk ();

	//
	//  Constructor
	//
	//  Purpose: To create a new Disk with the specified
	//           position and radius.
	//  Parameter(s):
	//    <1> position: The center position
	//    <2> radius: The disk radius
	//  Precondition(s):
	//    <1> position.y == 0
	//    <2> radius >= RADIUS_MIN
	//    <3> radius <= RADIUS_MAX
	//  Returns: N/A
	//  Side Effect: A new Disk is created at position position
	//               radius radius and the appriopriate disk
	//               type.  The disk rotation is determined
	//               randomly.
	//
	Disk (const ObjLibrary::Vector3& position,
	      float radius);

	Disk (const Disk& original) = default;
	~Disk () = default;
	Disk& operator= (const Disk& original) = default;

	//
	//  getPosition
	//
	//  Purpose: To determine the center position of this Disk.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The center position.
	//  Side Effect: N/A
	//
	const ObjLibrary::Vector3& getPosition () const;

	//
	//  getRadius
	//
	//  Purpose: To determine the radius of this Disk.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The disk radius.
	//  Side Effect: N/A
	//
	float getRadius () const;

	//
	//  getDiskType
	//
	//  Purpose: To determine the disk type for this Disk.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The disk type.
	//  Side Effect: N/A
	//
	unsigned int getDiskType () const;

	//
	//  getHeight
	//
	//  Purpose: To determine the height of this Disk at the
	//           specified position.
	//  Parameter(s):
	//    <1> position: The position to test
	//  Precondition(s): N/A
	//  Returns: The height of this Disk at position position.
	//           If position position is not on top of this
	//           Disk, 0.0 is returned.  This is the same value
	//           returned when position position is on this Disk
	//           but not on the heightmap.
	//  Side Effect: N/A
	//
	float getHeight (const ObjLibrary::Vector3& position) const;

	//
	//  draw
	//
	//  Purpose: To display this Disk.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isModelsLoaded()
	//  Returns: N/A
	//  Side Effect: This Disk is displayed.
	//
	void draw () const;

private:
	//
	//  Helper Function: invariant
	//
	//  Purpose: To determine if the class invariant is true.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: Whether the class invariant is true.
	//
	bool invariant () const;

private:
	ObjLibrary::Vector3 m_position;
	float m_radius;
	unsigned int m_disk_type;
	Heightmap m_heightmap;
	float m_rotation_radians;
public:
	std::vector<int> disk_node_list;
};



#endif
