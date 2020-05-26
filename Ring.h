//
//  Ring.h
//
//  A module to represent a wandering ring pickup.
//

#ifndef RING_H
#define RING_H

#include <cassert>
#include <vector>
#include <deque>
#include <algorithm>

#include "ObjLibrary/Vector3.h"
#include "Nodes.h"

class World;



//
//  Ring
//
//  A class to represent a ring pickup that wanders around the
//    world randomly.
//
class Ring
{
public:
	//
	//  RADIUS
	//
	//  The radius for a Ring.
	//
	static const float RADIUS;

	//
	//  HALF_HEIGHT
	//
	//  The vertical distance from the center of a Ring to its
	//    top or bottom.
	//
	static const float HALF_HEIGHT;

	//
	//  POINTS
	//
	//  The points per Ring collected.
	//
	static const unsigned int POINTS;

	//
	//  Class Function: isModelsLoaded
	//
	//  Purpose: To determine if the models for the Ring class
	//           have been loaded.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the Ring class has been initialized.
	//  Side Effect: N/A
	//
	static bool isModelsLoaded ();

	//
	//  Class Function: loadModels
	//
	//  Purpose: To load the models for the Ring class.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> !isModelsLoaded()
	//  Returns: The Ring is initialized.
	//  Side Effect: N/A
	//
	static void loadModels ();

public:
	//
	//  Default Constructor
	//
	//  Purpose: To create a new Ring with default values.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new Ring is created at the origin with no
	//               path.
	//
	Ring ();

	//
	//  Constructor
	//
	//  Purpose: To create a new Ring with the specified
	//           position.
	//  Parameter(s):
	//    <1> position: The center position
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new Ring is created at position with no
	//               path.
	//
	Ring (const ObjLibrary::Vector3& position, int i,std::vector<Nodes> ring_Nodes);

	Ring (const Ring& original) = default;
	~Ring () = default;
	Ring& operator= (const Ring& original) = default;

	

	//
	//  getPosition
	//
	//  Purpose: To determine the center position of this Ring.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The center position.
	//  Side Effect: N/A
	//
	const ObjLibrary::Vector3& getPosition () const;

	//
	//  isTaken
	//
	//  Purpose: To determine if this Ring has been taken.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the player has taken this Ring.
	//  Side Effect: N/A
	//
	bool isTaken () const;

	//
	//  draw
	//
	//  Purpose: To display this Ring.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isModelsLoaded()
	//  Returns: N/A
	//  Side Effect: This Ring is displayed.
	//
	void draw () const;

	//
	//  drawPath
	//
	//  Purpose: To display the path for this Ring.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isModelsLoaded()
	//  Returns: N/A
	//  Side Effect: The path for this Ring is displayed.
	//
	void drawPath ();

	//
	//  update
	//
	//  Purpose: To update this Ring for 1 frame.
	//  Parameter(s):
	//    <1> world: The World this Ring is in
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Ring is updated for 1 frame.
	//
	void update (World& world, std::vector<Nodes> target_node_find);

	//
	//  markTaken
	//
	//  Purpose: To mark this Ring has having been taken by the
	//           player.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Ring is marked as having been taken by
	//               the player.
	//
	void markTaken ();

private:
	//
	//  isTargetPosition
	//
	//  Purpose: To determine if this Ring has a target
	//           position.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether this Ring is moving towards a target
	//           position.
	//  Side Effect: N/A
	//
	bool isTargetPosition () const;

	//
	//  moveTowardsTarget
	//
	//  Purpose: To move this Ring towards its current target for 1
	//           frame.
	//  Parameter(s):
	//    <1> world: The World this Ring is in
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Ring is moved towards its current target.
	//
	void moveTowardsTarget (const World& world);

	//
	//  chooseTarget
	//
	//  Purpose: To choose a new target position for this Ring.
	//  Parameter(s):
	//    <1> world: The World this Ring is in
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new target position is chosen for this
	//               Ring.
	//
	void chooseTarget(int node_id, std::vector<Nodes> target_node_find);
	void MM(World& world);
	void getnextnode();
	static bool valuesort(const Nodes& a, const Nodes& b);
	bool check_similarity_closedList(Vector3 check);
	bool check_similarity_closedList2(Vector3 check);
	bool check_similarity_OpenList(Vector3 check);
	bool check_similarity_OpenList2(Vector3 check);
	bool similarClosedList();

public:
	void drawSpheres() const;

private:
	bool isNodePosition() const;
	int getNodeID(Vector3 pos);
	double calculateweight(Vector3 posa, Vector3 posb, World& world);
	//double calculateweightring(Vector3 posa, Vector3 posb, World& world);
private:
	ObjLibrary::Vector3 m_position;
	float m_rotation;
	bool m_is_taken;
	ObjLibrary::Vector3 m_target_position;
	int source_id, target_id;
	std::deque<Nodes> OpenList;
	std::deque<Nodes> ClosedList;

	std::deque<Nodes> OpenList2;
	std::deque<Nodes> ClosedList2;

	std::deque<Nodes> FinalClosedList;

	std::deque<Nodes> Neighbours;
	
	std::vector<Nodes> ring_nodes;

	std::deque<Nodes> drawList;
	std::deque<Nodes> drawListsphere;

	Vector3 nodepos;
	int temp, index_j, index_i;
	bool goal;
public:
	bool copy = 0;
	Nodes current_node, current_node2;
	Vector3 startpos;
};

struct neighbourvalue
{
	Vector3 Node_pos;
	double weight_value;
};

#endif
