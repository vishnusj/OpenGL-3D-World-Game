//
//  Rod.h
//
//  A module to represent a rod pickup in the world.
//

#ifndef ROD_H
#define ROD_H

#include <cassert>

#include "ObjLibrary/Vector3.h"



//
//  Rod
//
//  A class to represent a rod pickup in the world.
//
//  Class Invariant:
//    <1> m_points >= POINTS_MIN
//    <2> m_points <= POINTS_MAX
//
class Rod
{
public:
	//
	//  RADIUS
	//
	//  The radius for a rod.
	//
	static const float RADIUS;

	//
	//  HALF_HEIGHT
	//
	//  The vertical distance from the center of a rod to its
	//    top or bottom.
	//
	static const float HALF_HEIGHT;

	//
	//  POINTS_MIN
	//
	//  The minimum points per rod collected.
	//
	static const unsigned int POINTS_MIN = 1;

	//
	//  POINTS_MAX
	//
	//  The maximum points per rod collected.
	//
	static const unsigned int POINTS_MAX = 5;

	//
	//  Class Function: isModelsLoaded
	//
	//  Purpose: To determine if the models for the Rod class
	//           have been loaded.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the Rod class has been initialized.
	//  Side Effect: N/A
	//
	static bool isModelsLoaded ();

	//
	//  Class Function: loadModels
	//
	//  Purpose: To load the models for the Rod class.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> !isModelsLoaded()
	//  Returns: The Rod is initialized.
	//  Side Effect: N/A
	//
	static void loadModels ();

public:
	//
	//  Default Constructor
	//
	//  Purpose: To create a new Rod with default values.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new Rod is created at the origin worth
	//               POINTS_MIN points.
	//
	Rod ();

	//
	//  Constructor
	//
	//  Purpose: To create a new Rod with the specified
	//           position and point value.
	//  Parameter(s):
	//    <1> position: The center position
	//    <2> points: The point value
	//  Precondition(s):
	//    <1> points >= POINTS_MIN
	//    <2> points <= POINTS_MAX
	//  Returns: N/A
	//  Side Effect: A new Rod is created at position position
	//               and points points.
	//
	Rod (const ObjLibrary::Vector3& position,
	     unsigned int points);

	Rod (const Rod& original) = default;
	~Rod () = default;
	Rod& operator= (const Rod& original) = default;

	//
	//  getPosition
	//
	//  Purpose: To determine the center position of this Rod.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The center position.
	//  Side Effect: N/A
	//
	const ObjLibrary::Vector3& getPosition () const;

	//
	//  getPoints
	//
	//  Purpose: To determine the points value for this Rod.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The points value.
	//  Side Effect: N/A
	//
	unsigned int getPoints () const;

	//
	//  isTaken
	//
	//  Purpose: To determine if this Rod has been taken.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the player has taken this Rod.
	//  Side Effect: N/A
	//
	bool isTaken () const;

	//
	//  draw
	//
	//  Purpose: To display this Rod.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isModelsLoaded()
	//  Returns: N/A
	//  Side Effect: This Rod is displayed.
	//
	void draw () const;

	//
	//  markTaken
	//
	//  Purpose: To mark this Rod has having been taken by the
	//           player.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Rod is marked as having been taken by
	//               the player.
	//
	void markTaken ();

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
	unsigned int m_points;
	bool m_is_taken;
};



#endif
