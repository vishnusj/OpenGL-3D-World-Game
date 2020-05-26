//
//  World.cpp
//

#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "ObjLibrary/Vector3.h"
#include "GetGlut.h"

#include "DiskType.h"
#include "Heightmap.h"
#include "Disk.h"
#include "Collision.h"
#include "World.h"
#include "Nodes.h"
#include "MovementGraph.h"

using namespace std;
using namespace ObjLibrary;



World :: World ()
		: m_radius(0.0f)
		, mv_disks()
{
	assert(invariant());
}

World :: World (const string& filename)
		: m_radius(0.0f)
		, mv_disks()
{
	assert(filename != "");

	init(filename);

	assert(invariant());
}



bool World :: isInitialized () const
{
	return !mv_disks.empty();
}

float World :: getRadius () const
{
	assert(isInitialized());

	return m_radius;
}

unsigned int World :: getDiskCount () const
{
	assert(isInitialized());

	return mv_disks.size();
}

const Disk& World :: getDisk (unsigned int disk) const
{
	assert(isInitialized());
	assert(disk < getDiskCount());

	assert(disk < mv_disks.size());
	return mv_disks[disk];
}

bool World :: isCylinderOnAnyDisk (const Vector3& position,
                                   float radius) const
{
	assert(isInitialized());
	assert(radius >= 0.0f);

	const Disk& closest_disk = getClosestDisk(position);
	return Collision::circleVsCircle(position,
	                                 radius,
	                                 closest_disk.getPosition(),
	                                 closest_disk.getRadius());
}

float World :: getHeightOnDisk (const ObjLibrary::Vector3& position) const
{
	assert(isInitialized());

	return getClosestDisk(position).getHeight(position);
}

unsigned int World :: getClosestDiskIndex (const ObjLibrary::Vector3& position) const
{
	unsigned int best_disk     = 0;
	double       best_distance = position.getDistanceXZ(mv_disks[0].getPosition()) - mv_disks[0].getRadius();

	for(unsigned int i = 1; i < mv_disks.size(); i++)
	{
		double distance = position.getDistanceXZ(mv_disks[i].getPosition()) - mv_disks[i].getRadius();
		if(distance < best_distance)
		{
			best_disk     = i;
			best_distance = distance;
		}
	}

	assert(best_disk < mv_disks.size());
	return best_disk;
}

const Disk& World :: getClosestDisk (const ObjLibrary::Vector3& position) const
{
	return mv_disks[getClosestDiskIndex(position)];
}

void World :: draw () 
{
	assert(isInitialized());

	for(unsigned int i = 0; i < mv_disks.size(); i++)
		mv_disks[i].draw();
	
}



void World :: init (const string& filename)
{
	assert(filename != "");

	mv_disks.clear();
	loadDisks(filename);
	
	initMovementGraph();
	
	assert(invariant());
}



void World :: loadDisks (const string& filename)
{
	assert(filename != "");
	assert(mv_disks.empty());

	ifstream fin(filename.c_str());
	if(!fin)
	{
		cerr << "Error in loadDisks: Could not open file \"" << filename << "\"" << endl;
		exit(1);
	}

	string firstline;
	getline(fin, firstline);
	if(firstline != "DISK version 1")
	{
		cerr << "Error in loadDisks: Invalid first line \"" << firstline << "\"" << endl;
		exit(1);
	}

	fin >> m_radius;
	if(m_radius <= 0.0)
	{
		cerr << "Error in loadDisks: Non-positive world radius" << endl;
		exit(1);
	}

	int disk_count;
	fin >> disk_count;
	if(disk_count < 0)
	{
		cerr << "Error in loadDisks: Negative disk count" << endl;
		exit(1);
	}

	mv_disks.reserve(disk_count);

	assert(disk_count >= 0);
	for(unsigned int i = 0; i < (unsigned int)(disk_count); i++)
	{
		double x;
		fin >> x;

		double z;
		fin >> z;

		float radius;
		fin >> radius;
		if(radius <= 0.0)
		{
			cerr << "Error in loadDisks: Non-positive disk radius" << endl;
			exit(1);
		}

		mv_disks.push_back( { Vector3(x, 0.0, z), radius } );

		if(!fin)
		{
			cerr << "Error in loadDisks: Not enough disks" << endl;
			exit(1);
		}
	}
}

bool World :: invariant () const
{
	if(m_radius < 0.0f) return false;
	return true;
}



void World::initMovementGraph()
{
	
	for (int i = 0; i < mv_disks.size() - 1; i++)
	{
		//mv_disks[i].disk_node_list.clear();
		for (int j = i + 1; j < mv_disks.size(); j++)
		{
			if (isTouching(mv_disks[i], mv_disks[j]))
			{
				//cout << "\nTouching";
				node_position_i = Nodes::calculateNodePosition(mv_disks[i], mv_disks[j]);
				node_position_j = Nodes::calculateNodePosition(mv_disks[j], mv_disks[i]);

				node_i = addNode(node_position_i, mv_disks[i]);
				node_j = addNode(node_position_j, mv_disks[j]);

				weight_ij = calculateweight(node_i, node_j);
				//cout << "Weight: " << weight_ij;

				addLink(node_i, node_j, weight_ij);

				
				for (int k = 0; k < mv_disks[i].disk_node_list.size(); k++)
				{

					//node_k = addNode(node_position_i, mv_disks[i]);
					weight_ik = calculateweightring(node_i, mv_disks[i].disk_node_list[k]);//Adjust***************************************
					addLink(node_i, mv_disks[i].disk_node_list[k], weight_ik);
				}

				for (int k = 0; k < mv_disks[j].disk_node_list.size(); k++)
				{

					//node_k = addNode(node_position_i, mv_disks[i]);
					weight_jk = calculateweightring(node_j, mv_disks[j].disk_node_list[k]); ///Adjust*****************************************
					addLink(node_j, mv_disks[j].disk_node_list[k], weight_jk);
				}


				mv_disks[i].disk_node_list.push_back(node_i);
				mv_disks[j].disk_node_list.push_back(node_j);
				
			}
		}
	}

	for (int i = 0; i < m_graph.size(); i++)
	{

		all_nodes.push_back(m_graph[i].Node_position);
		
	}

	for (int i = 0; i < m_graph.size(); i++)
	{
		for (int j = 0; j < m_graph[i].disk_links.size(); j++)
		{
			all_nodes[i].all_links.push_back({ m_graph[i].disk_links[j].other_node, m_graph[i].disk_links[j].weight });
		}
	}

	
}

bool World::isTouching(Disk disk_a, Disk disk_b)
{

	Vector3 pos_a = disk_a.getPosition();
	Vector3 pos_b = disk_b.getPosition();
	double dist_ab = pos_a.getDistance(pos_b);

	if (dist_ab < (disk_a.getRadius() + disk_b.getRadius() + 0.1))
		return 1;
	else
		return 0;
}

int World::addNode(Vector3 node_pos, Disk nodeondisk)
{
	
	m_graph.push_back({ node_pos, nodeondisk });

	
	return  m_graph.size()-1;
}

double World::calculateweight(int i, int j)
{

	double dist_nodes = m_graph[i].Node_position.getDistance(m_graph[j].Node_position);
	double cost_i = 1.0 / DiskType::getRingSpeedFactor(m_graph[i].disk_id.getDiskType());
	double cost_j = 1.0 / DiskType::getRingSpeedFactor(m_graph[j].disk_id.getDiskType());

	double weight = dist_nodes * ((cost_i + cost_j )/ 2.0);
	return weight;
}

double World::calculateweightring(int i, int j)
{
	Vector3 center_to_node1 = m_graph[i].Node_position - m_graph[i].disk_id.getPosition();

	Vector3 center_to_node2 = m_graph[j].Node_position - m_graph[j].disk_id.getPosition();

	double  arc_radians = center_to_node1.getAngle(center_to_node2);

	double arc_radius = m_graph[i].disk_id.getRadius() - 0.7;

	double arc_length = arc_radius * arc_radians;

	return arc_length;
}

void World::addLink(int i, int j, double weight)
{
	m_graph[i].disk_links.push_back({ m_graph[j].Node_position, weight });

	m_graph[j].disk_links.push_back({ m_graph[i].Node_position, weight });
}


void World::Display_movementGraph() 
{
	const float LINE_High = 1.0f;
	for (int i = 0; i < m_graph.size(); i++)
	{
				
		
		Vector3 source = all_nodes[i].pos_node;
		
		for (int j = 0; j < m_graph[i].disk_links.size(); j++)
		
		{
			
			Vector3 destination = m_graph[i].disk_links[j].other_node;

			glLineWidth(3.0);
			glColor3d(1.0, 1.0 - m_graph[i].disk_links[j].weight / 150.0, 0.0);
			glBegin(GL_LINE_STRIP);

				glVertex3d(source.x, source.y + LINE_High, source.z);
				glVertex3d(destination.x, destination.y + LINE_High, destination.z);
			glEnd();
			glLineWidth(1.0);

			


		}
		
	}
	
	
	
}


std::vector<Nodes> World::getNodes()
{
	return all_nodes;

}

