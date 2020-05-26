//
//  Game.h
//
//  A module to represent the state of the game.
//

#ifndef GAME_H
#define GAME_H

#include <cassert>
#include <string>
#include <vector>
#include <deque>

#include "ObjLibrary/Vector3.h"

#include "Heightmap.h"
#include "Disk.h"
#include "World.h"
#include "Rod.h"
#include "Ring.h"
#include "Player.h"
#include "Nodes.h"



//
//  Game
//
//  A class to represent the state of the game.
//
//  Class Invariant:
//    <1> !m_world.isInitialized() ||
//        m_world.getDiskCount() == mv_rods.size()
//    <2> !m_world.isInitialized() ||
//        m_world.getDiskCount() == mv_rings.size()
//
class Game
{
public:
	//
	//  Class Function: isModelsLoaded
	//
	//  Purpose: To determine if the models for the Game class
	//           have been loaded.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the Game class has been initialized.
	//  Side Effect: N/A
	//
	static bool isModelsLoaded ();

	//
	//  Class Function: loadModels
	//
	//  Purpose: To load the models for the Game class.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> !isModelsLoaded()
	//  Returns: The Game is initialized.  This includes
	//           initializing the classes it is composed from.
	//  Side Effect: N/A
	//
	static void loadModels ();

public:
	//
	//  Default Constructor
	//
	//  Purpose: To create a new Game without initializing it.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new Game is created but not initialized.
	//
	Game ();

	//
	//  Constructor
	//
	//  Purpose: To create a new Game and initialize it with the
	//           world in the specified file.
	//  Parameter(s):
	//    <1> filename: The name of the world data file
	//  Precondition(s):
	//    <1> filename != ""
	//  Returns: N/A
	//  Side Effect: A new Game is initialized from file
	//               filename.
	//
	Game (const std::string& filename);

	Game (const Game& original) = default;
	~Game () = default;
	Game& operator= (const Game& original) = default;

	//
	//  isInitialized
	//
	//  Purpose: To determine if this Game is initialized.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether this Game is initialized.
	//  Side Effect: N/A
	//
	bool isInitialized () const;

	//
	//  isGameOver
	//
	//  Purpose: To determine whether the game is over.  This
	//           happens when the player falls off the world.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: Whether the game is over.
	//  Side Effect: N/A
	//
	bool isGameOver () const;

	//
	//  isPlayerJumping
	//
	//  Purpose: To determine if the player is currently
	//           jumping.  Jumping includes falling.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: Whether the player is currently jumping.
	//  Side Effect: N/A
	//
	bool isPlayerJumping () const;

	//
	//  getScore
	//
	//  Purpose: To determine the current score.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: The player score.
	//  Side Effect: N/A
	//
	int getScore () const;

	//
	//  getCameraPosition
	//
	//  Purpose: To determine the position of the camera for
	//           this Game.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: The position of the camera.
	//  Side Effect: N/A
	//
	ObjLibrary::Vector3 getCameraPosition () const;

	//
	//  setupCamera
	//
	//  Purpose: To set up the camera for this Game.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: N/A
	//  Side Effect: The camera is set up for this Game.
	//
	void setupCamera () const;

	//
	//  drawSkybox
	//
	//  Purpose: To display the skybox for this Game.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isModelsLoaded()
	//    <2> isInitialized()
	//  Returns: N/A
	//  Side Effect: The skybox for this Game is displayed.
	//
	void drawSkybox () const;

	//
	//  draw
	//
	//  Purpose: To display this Game.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isModelsLoaded()
	//    <2> isInitialized()
	//  Returns: N/A
	//  Side Effect: The state of this Game is displayed.
	//
	void draw ();

	//
	//  init
	//
	//  Purpose: To initialize this Game based on the specified
	//           data file.
	//  Parameter(s):
	//    <1> filename: The name of the world data file
	//  Precondition(s):
	//    <1> filename != ""
	//  Returns: N/A
	//  Side Effect: This Game is initialized from file
	//               filename.  The current state of this Game,
	//               if any, is lost.
	//
	void init (const std::string& filename);

	//
	//  update
	//
	//  Purpose: To update this Game for 1 frame.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: N/A
	//  Side Effect: This Game is updated for 1 frame.
	//
	void update ();

	//
	//  playerAccelerateForward
	//  playerAccelerateBackward
	//  playerAccelerateLeft
	//  playerAccelerateRight
	//
	//  Purpose: To accelerate the player
	//           forward/backward/left/right for 1 frame.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//    <2> !isPlayerJumping()
	//  Returns: N/A
	//  Side Effect: The player is accelerated.
	//
	void playerAccelerateForward ();
	void playerAccelerateBackward ();
	void playerAccelerateLeft ();
	void playerAccelerateRight ();

	//
	//  playerTurnLeft
	//  playerTurnRight
	//
	//  Purpose: To turn the player left/right for 1 frame.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: N/A
	//  Side Effect: The player is rotated.
	//
	void playerTurnLeft ();
	void playerTurnRight ();

	//
	//  playerJump
	//
	//  Purpose: To make the player jump forward and upward.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//    <2> !isPlayerJumping()
	//  Returns: N/A
	//  Side Effect: If the player is on the ground, it jumps
	//               upwards.  Otherwise, there is no effect.
	//
	void playerJump ();

private:
	//
	//  Helper Function: initRods
	//
	//  Purpose: To initialize the rods.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> m_world.isInitialized()
	//    <2> mv_rods.empty()
	//  Returns: N/A
	//  Side Effect: The rods are added to the world.
	//
	void initRods ();

	//
	//  Helper Function: initRings
	//
	//  Purpose: To initialize the rings.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> m_world.isInitialized()
	//    <2> mv_rings.empty()
	//  Returns: N/A
	//  Side Effect: The rings are added to the world.
	//
	void initRings ();

	//
	//  Helper Function: handleCollisions
	//
	//  Purpose: To detect and resolve collisions for 1 frame.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isInitialized()
	//  Returns: N/A
	//  Side Effect: Collisions are handled for 1 frame.
	//
	void handleCollisions ();

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
	World m_world;
	std::vector<Rod>  mv_rods;
	std::vector<Ring> mv_rings;
	Player m_player;
	int m_score;

public:
	std::deque<int> path;
	std::vector<Nodes> game_nodes;
	void initSearchdata();
	void bestSearchData();
	void Dijkstras_path(Vector3 source_pos, int source_node_id, Vector3 dest_pos, int dest_node_id);
	void dijkstra_path(Vector3 source_pos, int source_node_id, Vector3 dest_pos, int dest_node_id);

	bool over;
};



#endif
