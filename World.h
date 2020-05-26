//
//  World.h
//
//  A module to manage all the disks in the world.
//

#ifndef WORLD_H
#define WORLD_H

#include <cassert>
#include <string>
#include <vector>

#include "ObjLibrary/Vector3.h"

#include "Heightmap.h"
#include "Disk.h"
#include "Nodes.h"
#include "MovementGraph.h"
#include "Ring.h"

//#include "Main.h"


//
//  World
//
//  A class to manage all the disks in the world.
//
//  Class Invariant:
//    <1> m_radius >= 0.0f
//

class World
{
public:
	//
	//  Default Constructor
	//
	//  Purpose: To create a new World with no disks.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new World is created but not initialized.
	//
	World ();

	//
	//  Constructor
	//
	//  Purpose: To create a new World and initialize it with
	//           the world in the specified file.
	//  Parameter(s):
	//    <1> filename: The name of the world data file
	//  Precondition(s):
	//    <1> filename != ""
	//  Returns: N/A
	//  Side Effect: A new World is initialized from file
	//               filename.
	//
	World (const std::string& filename);

	World (const World& original) = default;
	~World () = default;
	World& operator= (const World& original) = default;

	//
	//  isInitialized
	//
	//  Purpose: To determine if this World is initialized.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether this World is initialized.
	//  Side Effect: N/A
	//
	bool isInitialized () const;

	//
	//  getRadius
	//
	//  Purpose: To determine the World radius.  This is the
	//           distance from the world center to the outside
	//           of the outmost disk.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: The number of disks.
	//  Side Effect: N/A
	//
	float getRadius () const;

	//
	//  getDiskCount
	//
	//  Purpose: To determine the number of disks in this World.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: The number of disks.
	//  Side Effect: N/A
	//
	unsigned int getDiskCount () const;

	//
	//  getDisk
	//
	//  Purpose: To retreive the specified disk in this World.
	//  Parameter(s):
	//    <1> disk: Which disk
	//  Precondition(s):
	//    <1> isInitialized()
	//    <2> disk < getDiskCount()
	//  Returns: Disk disk.
	//  Side Effect: N/A
	//
	const Disk& getDisk (unsigned int disk) const;

	//
	//  isCylinderOnAnyDisk
	//
	//  Purpose: To determine if a cylinder with the specified
	//           center position and radius in on at least one
	//           disk.
	//  Parameter(s): N/A
	//    <1> position: The center of the cylinder
	//    <2> radius: The radius of the cylinder
	//  Precondition(s):
	//    <1> isInitialized()
	//    <2> radius >= 0.0f
	//  Returns: Whether the cylinder is on, above, or below any
	//           disk.  This function does not take the
	//           Z-coordinate into account.
	//  Side Effect: N/A
	//
	bool isCylinderOnAnyDisk (
	                        const ObjLibrary::Vector3& position,
	                        float radius) const;

	//
	//  getHeightOnDisk
	//
	//  Purpose: To determine if the height of the specified
	//           position, assuming that it is on a disk.
	//  Parameter(s): N/A
	//    <1> position: The position to test
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: The disk height at position position.  If
	//           position position is not on a disk,
	//           Disk::EDGE_HEIGHT is returned.
	//  Side Effect: N/A
	//
	float getHeightOnDisk (
	                 const ObjLibrary::Vector3& position) const;

	//
	//  getClosestDiskIndex
	//
	//  Purpose: To determine the index of the
	//           horizontally-closest disk to the specified
	//           position.
	//  Parameter(s):
	//    <1> position: The position to test
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: The index of the disk closest to position
	//           position.
	//  Side Effect: N/A
	//
	unsigned int getClosestDiskIndex (
	                 const ObjLibrary::Vector3& position) const;

	//
	//  getClosestDisk
	//
	//  Purpose: To retrieve the horizontally-closest disk to
	//           the specified position.
	//  Parameter(s):
	//    <1> position: The position to test
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: The disk closest to position position.
	//  Side Effect: N/A
	//
	const Disk& getClosestDisk (
	                 const ObjLibrary::Vector3& position) const;

	//
	//  draw
	//
	//  Purpose: To display this World.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: N/A
	//  Side Effect: The disks in this World are displayed.
	//
	void draw ();

	//
	//  init
	//
	//  Purpose: To initialize this World based on the specified
	//           data file.
	//  Parameter(s):
	//    <1> filename: The name of the world data file
	//  Precondition(s):
	//    <1> filename != ""
	//  Returns: N/A
	//  Side Effect: The disks in file filename are loaded into
	//               this World.
	//
	void init (const std::string& filename);

	

private:
	//
	//  Helper Function: loadDisks
	//
	//  Purpose: To load the disks from the specified data file.
	//  Parameter(s):
	//    <1> filename: The name of the world data file
	//  Precondition(s):
	//    <1> filename != ""
	//    <2> mv_disks.empty()
	//  Returns: N/A
	//  Side Effect: The disks in file filename are loaded.
	//
	void loadDisks (const std::string& filename);

	//
	//  Helper Function: invariant
	//
	//  Purpose: To determine if the class invariant is true.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the class invariant is true.
	//  Side Effect: N/A
	//
	bool invariant () const;

private:
	float m_radius;
	std::vector<Disk> mv_disks;
	
	
	double weight_ij, weight_ik, weight_jk;
	
	void initMovementGraph();
	bool isTouching(Disk disk_a, Disk disk_b);
	Vector3 node_position_i, node_position_j;
	int node_i, node_j, node_k;
	

	int addNode(Vector3 node_pos, Disk nodeondisk);
	void addLink(int i, int j, double weight);
	double calculateweight(int i, int j);
	double calculateweightring(int i, int j);
public:
	void Display_movementGraph();
	std::vector<Nodes> all_nodes;
	std::vector<MovementGraph> m_graph;

	std::vector<Nodes> getNodes();

	
	//void updateRings();
};



#endif
