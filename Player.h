//
//  Player.h
//
//  A module to represent the player.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "ObjLibrary/Vector3.h"

class World;



//
//  Player
//
//  A class to represent the player.
//
//  Class Invariant:
//    <1> m_forward.y == 0.0
//    <2> m_forward.isNormal()
//
class Player
{
public:
	//
	//  RADIUS
	//
	//  The radius of the player.
	//
	static const float RADIUS;

	//
	//  HALF_HEIGHT
	//
	//  Half of the height of the player.
	//
	static const float HALF_HEIGHT;

	//
	//  DEAD_Y
	//
	//  The Y-cooridnate at which the player is marked as dead.
	//
	static const float DEAD_Y;

	//
	//  Class Function: isModelsLoaded
	//
	//  Purpose: To determine if the models for the Player class
	//           have been loaded.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the Player class has been initialized.
	//  Side Effect: N/A
	//
	static bool isModelsLoaded ();

	//
	//  Class Function: loadModels
	//
	//  Purpose: To load the models for the Player class.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> !isModelsLoaded()
	//  Returns: The Player is initialized.
	//  Side Effect: N/A
	//
	static void loadModels ();

public:
	//
	//  Default Constructor
	//
	//  Purpose: To create a new Player with default values.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new Player is created at the origin and
	//               marked as stationary and on the ground.
	//
	Player ();

	//
	//  Constructor
	//
	//  Purpose: To create a new Player with the specified
	//           position.
	//  Parameter(s):
	//    <1> position: The player position
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new Player is created at position
	//               position and marked as stationary and on
	//               the ground.
	//
	Player (const ObjLibrary::Vector3& position);

	Player (const Player& original) = default;
	~Player () = default;
	Player& operator= (const Player& original) = default;

	//
	//  getPosition
	//
	//  Purpose: To determine the current position of this
	//           Player.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The current position.
	//  Side Effect: N/A
	//
	const ObjLibrary::Vector3& getPosition () const;

	//
	//  getVelocity
	//
	//  Purpose: To determine the current velocity of this
	//           Player.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The current velocity.
	//  Side Effect: N/A
	//
	const ObjLibrary::Vector3& getVelocity () const;

	//
	//  getForward
	//
	//  Purpose: To determine the current forward vector of this
	//           Player.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The current forward vector.
	//  Side Effect: N/A
	//
	const ObjLibrary::Vector3& getForward () const;

	//
	//  isJumping
	//
	//  Purpose: To determine if this Player is currently
	//           jumping.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether this Player is jumping.
	//  Side Effect: N/A
	//
	bool isJumping () const;

	//
	//  isDead
	//
	//  Purpose: To determine if this Player is dead.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether this Player has fallen far below the
	//           world.
	//  Side Effect: N/A
	//
	bool isDead () const;

	//
	//  draw
	//
	//  Purpose: To display this Player.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isModelsLoaded()
	//  Returns: N/A
	//  Side Effect: This Player is displayed.
	//
	void draw () const;

	//
	//  init
	//
	//  Purpose: To initialize this Player at the specified
	//           position.
	//  Parameter(s):
	//    <1> position: The player position
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Player is set to be at position
	//               position, stationary, and on the ground.
	//
	void init (const ObjLibrary::Vector3& position);

	//
	//  update
	//
	//  Purpose: To update this Player for 1 frame.
	//  Parameter(s):
	//    <1> world: The World this Player is in
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Player is updated for 1 frame.
	//
	void update (const World& world);

	//
	//  accelerateForward
	//
	//  Purpose: To accelerate this Player forward at the
	//           specified rate.
	//  Parameter(s):
	//    <1> acceleration: The acceleration rate for the player
	//  Precondition(s):
	//    <1> !isJumping()
	//  Returns: N/A
	//  Side Effect: This Player is accelerated forward at a
	//               rate of acceleration m/2^2.  If
	//               acceleration is negative, this Player is
	//               accelerated backwards.
	//
	void accelerateForward (float acceleration);

	//
	//  accelerateLeft
	//
	//  Purpose: To accelerate this Player to the left at the
	//           specified rate.
	//  Parameter(s):
	//    <1> acceleration: The acceleration rate for the player
	//  Precondition(s):
	//    <1> !isJumping()
	//  Returns: N/A
	//  Side Effect: This Player is accelerated to the left at a
	//               rate of acceleration per second.  If
	//               acceleration is negative, this Player is
	//               accelerated to the rught.
	//
	void accelerateLeft (float acceleration);

	//
	//  turnLeft
	//
	//  Purpose: To rotate this Player left by the specified
	//           angle.
	//  Parameter(s):
	//    <1> radians_per_second: The rotation rate for the
	//                            player
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Player is rotated to the left at a
	//               rate of radians_per_second for 1 frame.  If
	//               radians_per_second is negative, this Player
	//               is rotated to the right.
	//
	void turnLeft (float radians_per_second);

	//
	//  doJump
	//
	//  Purpose: To make this Player jump.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> !isJumping()
	//  Returns: N/A
	//  Side Effect: This Player jumps.
	//
	void doJump ();

	//
	//  markRunning
	//
	//  Purpose: To change this Player to be running for this
	//           frame.
	//  Parameter(s):
	//    <1> is_forward: Whether this player is running forward
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: If is_forward == true, this Player is
	//               marked as running forward.  Otherwise, it
	//               is marked as running backwards.  In either
	//               case, the run animation is updated for 1
	//               frame.
	//
	void markRunning (bool is_forward);

private:
	//
	//  Helper Function: moveAccordingToVelocity
	//
	//  Purpose: To move this Player according to its velocity
	//           for 1 frame.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Player is moved according to its
	//               velocity.
	//
	void moveAccordingToVelocity ();

	//
	//  Helper Function: decreaseRunCounter
	//
	//  Purpose: To decrease the run counter for this Player.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: The run counter is decreased for one frame.
	//               When it reaches 0, the stand animation will
	//               be used instead of the run animation.
	//
	void decreaseRunCounter ();

	//
	//  Helper Function: applyFriction
	//
	//  Purpose: To apply friction of the specified strength to
	//           this Player.
	//  Parameter(s):
	//    <1> friction_per_second: The strength of the friction
	//  Precondition(s):
	//    <1> friction_per_second >  0.0
	//    <2> friction_per_second <= 1.0
	//  Returns: N/A
	//  Side Effect: The velocity of this Player is decreased
	//               due to friction of strength friction.
	//
	void applyFriction (float friction_per_second);

	//
	//  Helper Function: applyGravity
	//
	//  Purpose: To make this Player accelerate downward
	//           according to gravity for 1 frame.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: This Player accelerates due to gravity.
	//
	void applyGravity ();

	//
	//  Helper Function: slide
	//
	//  Purpose: To make the player slide downhill for 1 frame.
	//  Parameter(s):
	//    <1> world: The World this Ring is in
	//  Precondition(s):
	//    <1> !isJumping()
	//  Returns: N/A
	//  Side Effect: The player is accelerated downhill.
	//
	void slide (const World& world);

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
	ObjLibrary::Vector3 m_velocity;
	ObjLibrary::Vector3 m_forward;
	bool m_is_jumping;
	unsigned int m_run_counter;
	float m_run_frame;
};



#endif
