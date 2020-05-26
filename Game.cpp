//
//  Game.cpp
//

#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

#include "GetGlut.h"
#include "Pi.h"
#include "Random.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/DisplayList.h"

#include "PhysicsFrameLength.h"
#include "DiskType.h"
#include "Heightmap.h"
#include "Disk.h"
#include "Rod.h"
#include "Ring.h"
#include "Player.h"
#include "Collision.h"
#include "Game.h"
#include "World.h"
#include "Nodes.h"


using namespace std;
using namespace ObjLibrary;
namespace
{
	const Vector3 CAMERA_UP(0.0, 1.0, 0.0);
	const float   CAMERA_DISTANCE_HORIZONTAL = 2.0f;
	const float   CAMERA_DISTANCE_VERTICAL   = 0.4f;

	const float   PLAYER_FORWARD_ACCELERATION  = 25.0f;  // meters per second squared
	const float   PLAYER_BACKWARD_ACCELERATION = 10.0f;  // meters per second squared
	const float   PLAYER_STRAFE_ACCELERATION   = 10.0f;  // meters per second squared
	const float   PLAYER_TURN_RATE             =  2.0f;  // radians per frame

	DisplayList g_skybox_list;
}



bool Game :: isModelsLoaded ()
{
	assert(Disk  ::isModelsLoaded() == g_skybox_list.isReady());
	assert(Rod   ::isModelsLoaded() == g_skybox_list.isReady());
	assert(Ring  ::isModelsLoaded() == g_skybox_list.isReady());
	assert(Player::isModelsLoaded() == g_skybox_list.isReady());

	return g_skybox_list.isReady();
}

void Game :: loadModels ()
{
	assert(!isModelsLoaded());

	g_skybox_list = ObjModel("Models/Skybox.obj").getDisplayList();

	Disk  ::loadModels();
	Rod   ::loadModels();
	Ring  ::loadModels();
	Player::loadModels();

	assert(isModelsLoaded());
}



Game :: Game ()
		: m_world()
		, mv_rods()
		, mv_rings()
		, m_player()
		, m_score(0)
{
	assert(invariant());
}

Game :: Game (const std::string& filename)
		: m_world()
		, mv_rods()
		, mv_rings()
		, m_player()
		, m_score(0)
{
	assert(filename != "");

	assert(invariant());
	init(filename);

	assert(invariant());
}



bool Game :: isInitialized () const
{
	return m_world.isInitialized();
}

bool Game :: isGameOver () const
{
	return m_player.isDead();
}

bool Game :: isPlayerJumping () const
{
	return m_player.isJumping();
}

int Game :: getScore () const
{
	return m_score;
}

ObjLibrary::Vector3 Game :: getCameraPosition () const
{
	return m_player.getPosition() -
	       m_player.getForward() * CAMERA_DISTANCE_HORIZONTAL +
	       CAMERA_UP             * CAMERA_DISTANCE_VERTICAL;
}

void Game :: setupCamera () const
{
	assert(isInitialized());

	Vector3 player_position = m_player.getPosition();
	Vector3 camera_position = getCameraPosition();

	gluLookAt(camera_position.x, camera_position.y, camera_position.z,
	          player_position.x, player_position.y, player_position.z,
	                CAMERA_UP.x,       CAMERA_UP.y,       CAMERA_UP.z);
}

void Game :: drawSkybox () const
{
	assert(isModelsLoaded());
	assert(isInitialized());

	Vector3 camera_position = getCameraPosition();

	glDepthMask(GL_FALSE);
	glPushMatrix();
		glTranslated(camera_position.x, camera_position.y, camera_position.z);
		g_skybox_list.draw();
	glPopMatrix();
	glDepthMask(GL_TRUE);
}

void Game :: draw () 
{
	assert(isModelsLoaded());
	assert(isInitialized());

	m_world.draw();
	if(over)
	m_world.Display_movementGraph();
	for(unsigned int i = 0; i < mv_rods.size(); i++)
		mv_rods[i].draw();
	for (unsigned int i = 0; i < mv_rings.size(); i++)
	{
		mv_rings[i].draw();
		
		

		//if (copy == 1)
			
	}

	if (over)
	{
		mv_rings[0].drawSpheres();
		mv_rings[0].drawPath();
	}
	m_player.draw();
}



void Game :: init (const std::string& filename)
{
	assert(filename != "");

	m_world.init(filename);
	
	for (int i = 0; i < m_world.all_nodes.size(); i++)
	{
		game_nodes.push_back(m_world.all_nodes[i]);
	}
	initSearchdata();
//	bestSearchData();
	mv_rods.clear();
	
	initRods();

	mv_rings.clear();
	initRings();

	Vector3 player_position = m_world.getDisk(0).getPosition();
	player_position.y = m_world.getHeightOnDisk(player_position) + Player::HALF_HEIGHT;
	m_player.init(player_position);

	m_score = 0;

	assert(invariant());
}
/*
void Game :: update ()
{
	assert(isInitialized());
	int target = random1(6);
	m_player.update(m_world);
	for (int i = 0; i < game_nodes.size(); i++)
	{
		
	}
		

	for (unsigned int i = 0; i < mv_rings.size(); i++)
	{
		//
		//Dijkstras_path(game_nodes[i].pos_node, i, game_nodes[target].pos_node, target);
		//if(target<6)
		
		mv_rings[i].update(m_world, game_nodes, path);
		
	}
	

	handleCollisions();

	assert(invariant());
}
*/

void Game::update()
{
	assert(isInitialized());

	m_player.update(m_world);
	for (unsigned int i = 0; i < mv_rings.size(); i++)
	{
		
		mv_rings[i].update(m_world, game_nodes);
		
	}
		

	handleCollisions();

	assert(invariant());
}



void Game :: playerAccelerateForward ()
{
	assert(isInitialized());
	assert(!isPlayerJumping());

	const Disk& closest_disk = m_world.getClosestDisk(m_player.getPosition());
	float factor = DiskType::getAccelerationFactor(closest_disk.getDiskType());
	m_player.accelerateForward(PLAYER_FORWARD_ACCELERATION * factor);
	m_player.markRunning(true);

	assert(invariant());
}

void Game :: playerAccelerateBackward ()
{
	assert(isInitialized());
	assert(!isPlayerJumping());

	const Disk& closest_disk = m_world.getClosestDisk(m_player.getPosition());
	float factor = DiskType::getAccelerationFactor(closest_disk.getDiskType());
	m_player.accelerateForward(-PLAYER_BACKWARD_ACCELERATION * factor);
	m_player.markRunning(false);

	assert(invariant());
}

void Game :: playerAccelerateLeft ()
{
	assert(isInitialized());
	assert(!isPlayerJumping());

	const Disk& closest_disk = m_world.getClosestDisk(m_player.getPosition());
	float factor = DiskType::getAccelerationFactor(closest_disk.getDiskType());
	m_player.accelerateLeft(PLAYER_STRAFE_ACCELERATION * factor);

	assert(invariant());
}

void Game :: playerAccelerateRight ()
{
	assert(isInitialized());
	assert(!isPlayerJumping());

	const Disk& closest_disk = m_world.getClosestDisk(m_player.getPosition());
	float factor = DiskType::getAccelerationFactor(closest_disk.getDiskType());
	m_player.accelerateLeft(-PLAYER_STRAFE_ACCELERATION * factor);

	assert(invariant());
}

void Game :: playerTurnLeft ()
{
	assert(isInitialized());

	m_player.turnLeft(PLAYER_TURN_RATE);

	assert(invariant());
}

void Game :: playerTurnRight ()
{
	assert(isInitialized());

	m_player.turnLeft(-PLAYER_TURN_RATE);

	assert(invariant());
}

void Game :: playerJump ()
{
	assert(isInitialized());
	assert(!isPlayerJumping());

	m_player.doJump();

	assert(invariant());
}



void Game :: initRods ()
{
	assert(m_world.isInitialized());
	assert(mv_rods.empty());

	for(unsigned int i = 0; i < m_world.getDiskCount(); i++)
	{
		const Disk& disk = m_world.getDisk(i);
		Vector3 position = disk.getPosition();
		position.y = Rod::HALF_HEIGHT + disk.getHeight(position);
		unsigned int points = 1 + disk.getDiskType();

		mv_rods.push_back(Rod(position, points));
	}
}

void Game :: initRings ()
{
	assert(m_world.isInitialized());
	assert(mv_rings.empty());



	for( int i = 0; i < m_world.getDiskCount(); i++)
	{
		Vector3 position = game_nodes[i].pos_node;

		position.y = Ring::HALF_HEIGHT + m_world.getHeightOnDisk(position);

		mv_rings.push_back({ (position), i , game_nodes});
	}
	mv_rings[0].copy = 1;
}

void Game :: handleCollisions ()
{
	assert(isInitialized());

	for(unsigned int i = 0; i < mv_rods.size(); i++)
	{
		Rod& r_rod = mv_rods[i];
		if(!r_rod.isTaken() &&
		   Collision::cylinderVsCylinder(m_player.getPosition(),
		                                 Player::RADIUS,
		                                 Player::HALF_HEIGHT,
		                                 r_rod.getPosition(),
		                                 Rod::RADIUS,
		                                 Rod::HALF_HEIGHT))
		{
			r_rod.markTaken();
			m_score += r_rod.getPoints();
		}
	}

	for(unsigned int i = 0; i < mv_rings.size(); i++)
	{
		Ring& r_ring = mv_rings[i];
		if(!r_ring.isTaken() &&
		   Collision::cylinderVsCylinder(m_player.getPosition(),
		                                 Player::RADIUS,
		                                 Player::HALF_HEIGHT,
		                                 r_ring.getPosition(),
		                                 Ring::RADIUS,
		                                 Ring::HALF_HEIGHT))
		{
			r_ring.markTaken();
			m_score += Ring::POINTS;
		}
	}
}

bool Game :: invariant () const
{
	if(m_world.isInitialized() && m_world.getDiskCount() != mv_rods.size()) return false;
	if(m_world.isInitialized() && m_world.getDiskCount() != mv_rings.size()) return false;
	return true;
}

void Game::initSearchdata()
{
	for (int i = 0; i < game_nodes.size(); i++)
	{
		if(i!=0)
			game_nodes[i].search_data_node.push_back({ i-1, m_world.m_graph[i].disk_links[0].weight, 0.0, (m_world.m_graph[i].disk_links[0].weight + 0.0) });
		else
			game_nodes[i].search_data_node.push_back({ 0, m_world.m_graph[i].disk_links[0].weight, 0.0, (m_world.m_graph[i].disk_links[0].weight + 0.0) });
	}

}

void Game::bestSearchData()
{
	for (int k = 0; k < game_nodes.size(); k++)		
	{

		std::vector<Search_data> temp_searchsata;

		for (int l = 0; l < m_world.m_graph[k].disk_links.size(); l++)
		{
			if (k != 0)
				temp_searchsata.push_back({ k - 1, (m_world.m_graph[k].disk_links[l].weight + m_world.m_graph[k-1].disk_links[l].weight) , 0.0, ((m_world.m_graph[k].disk_links[l].weight + m_world.m_graph[k - 1].disk_links[l].weight) + 0.0) });
			else
				temp_searchsata.push_back({ 0, (m_world.m_graph[k].disk_links[l].weight ) , 0.0, ((m_world.m_graph[k].disk_links[l].weight) + 0.0) });
		}

		for (int l = 0; l < temp_searchsata.size(); l++)
		{
			if (temp_searchsata[l].g < game_nodes[k].search_data_node[0].g)
			{
				game_nodes[k].search_data_node[0].previous_node_id = temp_searchsata[l].previous_node_id;
				game_nodes[k].search_data_node[0].g = temp_searchsata[l].g;
				game_nodes[k].search_data_node[0].h = temp_searchsata[l].h;
				game_nodes[k].search_data_node[0].f = temp_searchsata[l].f;
			}
		}
		//if ( <

		
	}
}


void Game::Dijkstras_path(Vector3 source_pos, int source_node_id, Vector3 dest_pos, int dest_node_id )
{
	int current_node;
	if (source_node_id != dest_node_id)
	{
		for (current_node = dest_node_id; game_nodes[current_node].search_data_node[0].previous_node_id != source_node_id; current_node = game_nodes[current_node].search_data_node[0].previous_node_id)

			//while (game_nodes[current_node].search_data_node[0].previous_node_id != source_node_id)
		{
			path.push_back(current_node);
			//current_node = 

		}
		path.push_back(current_node);
		//path.push_back(source_node_id);
		//for (int i = 0; i < path.size(); i++)

		cout << "\n"<<path.size();
	}
	
}

