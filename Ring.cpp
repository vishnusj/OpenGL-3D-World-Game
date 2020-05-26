//
//  Ring.cpp
//

#include <cassert>

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
#include "World.h"
#include "Ring.h"


using namespace std;
using namespace ObjLibrary;
namespace
{
	const float ROTATION_RATE   = 75.0f;  // degrees per meter
	const float MOVE_SPEED_BASE =  2.5f;  // meters per second

	DisplayList g_display_list;
}



const float        Ring :: RADIUS      = 0.7f;
const float        Ring :: HALF_HEIGHT = 0.1f;
const unsigned int Ring :: POINTS      = 1;



bool Ring :: isModelsLoaded ()
{
	return g_display_list.isReady();
}

void Ring :: loadModels ()
{
	assert(!isModelsLoaded());

	g_display_list = ObjModel("Models/Ring.obj").getDisplayList();

	assert(isModelsLoaded());
}



Ring :: Ring ()
		: m_position()
		, m_rotation(random1(360.0f))
		, m_is_taken(false)
		, m_target_position()
		, nodepos()
{
}

Ring :: Ring (const ObjLibrary::Vector3& position, int i, std::vector<Nodes> ring_Nodes)
		: m_position(position)
		, m_rotation(random1(360.0f))
		, m_is_taken(false)
		, m_target_position(position)
		, source_id(i)
		, ring_nodes(ring_Nodes)
		, nodepos(position)
{
}



const Vector3& Ring :: getPosition () const
{
	return m_position;
}

bool Ring :: isTaken () const
{
	return m_is_taken;
}

void Ring :: draw () const
{
	assert(isModelsLoaded());

	if(isTaken())
		return;

	glPushMatrix();
		glTranslated(m_position.x, m_position.y, m_position.z);
		glRotated(m_rotation, 0.0, 1.0, 0.0);
		g_display_list.draw();
	glPopMatrix();
	
}

void Ring :: drawPath ()
{
	if (isNodePosition() && !drawList.empty())
		drawList.erase(drawList.begin());
	
	const float LINE_ABOVE = 0.5f;

	glLineWidth(3.0);
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
		glVertex3d(m_position       .x, m_position       .y,              m_position       .z);
		glVertex3d(m_position       .x, m_position       .y + LINE_ABOVE, m_position       .z);
		for (int i = 0; i < drawList.size(); i++)
		{
			glVertex3d(drawList[i].pos_node.x, drawList[i].pos_node.y + LINE_ABOVE, drawList[i].pos_node.z);
		}
		
	glEnd();
	glLineWidth(1.0);

	
}



void Ring :: update (World& world, std::vector<Nodes> target_node_find)
{
	if(!isTaken())
	{
		if (isNodePosition())
		{
			
			if (isTargetPosition())
			{

				OpenList.clear();
				OpenList2.clear();
				ClosedList.clear();
				ClosedList2.clear();
				FinalClosedList.clear();
				startpos = m_position;
				chooseTarget(target_node_find.size(), target_node_find);
				
				MM(world);

				//
				//FinalClosedList

				
				
			}
			if (!FinalClosedList.empty())
			{
				
				getnextnode();
			}
				
			else if (FinalClosedList.empty())
			{
				FinalClosedList.push_back({ m_target_position,  0.0 , 0.0});
				startpos = m_position;
				if (copy == 1)
				{
					drawList = FinalClosedList;
					drawListsphere = drawList;
				}
			}
				
		}
		
		
			
		moveTowardsTarget(world);
		
		

		// maintain disk height
		m_position.y = HALF_HEIGHT + world.getHeightOnDisk(m_position);
	}
}

void Ring :: markTaken ()
{
	m_is_taken = true;
}

void Ring::getnextnode()
{
	nodepos = FinalClosedList.front().pos_node;
	FinalClosedList.pop_front();
}

bool Ring :: isTargetPosition () const
{
	return (m_position.x == m_target_position.x && m_position.z == m_target_position.z);
}

bool Ring::isNodePosition() const
{
	return (m_position.x == nodepos.x && m_position.z == nodepos.z);
}

void Ring::moveTowardsTarget(const World& world)
{
	unsigned int disk_type = world.getClosestDisk(m_position).getDiskType();
	float move_speed = MOVE_SPEED_BASE * DiskType::getRingSpeedFactor(disk_type);
	float move_distance = move_speed * PHYSICS_FRAME_LENGTH;

	m_rotation += ROTATION_RATE * move_distance;

	if (nodepos == m_target_position)
	{
		Vector3 here_to_target = m_target_position - m_position;
		Vector3 forward = here_to_target.getComponentXZ();
		if (!forward.isZero())
			m_position += forward.getCopyWithNorm(move_distance);

		// stop at target position
		if (m_position.isDistanceXZLessThan(m_target_position, move_distance))
			m_position = m_target_position;
	}

	else
	{

		Vector3 here_to_target = nodepos - m_position;

		// move straight to target position
		Vector3 forward = here_to_target.getComponentXZ();
		if (!forward.isZero())
			m_position += forward.getCopyWithNorm(move_distance);
		//if(forward.isZero() && m_position != nodepos)
			//m_position = nodepos;

		//stop at a node
		if (m_position.isDistanceXZLessThan(nodepos, move_distance))
		{
			m_position = nodepos;

			// stop at target position
			//if (m_position.isDistanceXZLessThan(m_target_position, move_distance))
			//	m_position = m_target_position;
		}

	}
	
}


void Ring::chooseTarget(int node_id, std::vector<Nodes> target_node_find)
{
	unsigned int node_index = random1(node_id);
	target_id = node_index;
	assert(node_index <  target_node_find.size());
	m_target_position = target_node_find[node_index].pos_node;


	
}

void Ring::MM(World& world)
{
	

	

	//double low_cost = 100.0;

	OpenList.push_back(ring_nodes[source_id]);
	OpenList2.push_back(ring_nodes[target_id]);
	ClosedList.push_back(ring_nodes[source_id]);
	ClosedList2.push_back(ring_nodes[target_id]);

	//cout << "Low_Cost: " << low_cost;

	while (!OpenList.empty() || !OpenList2.empty())
	{
		goal = 0;
		std::sort(OpenList.begin(), OpenList.end(), &Ring::valuesort);
		std::sort(OpenList2.begin(), OpenList2.end(), &Ring::valuesort);
		
		//Select Current Node from the OpenLists
		if (similarClosedList())
		{
			//ClosedList.push_back({ current_node.pos_node,  current_node.cost,  m_target_position.getDistanceXZ(current_node.pos_node) });
			//ClosedList2.push_back({ current_node2.pos_node,  current_node2.cost,  m_target_position.getDistanceXZ(current_node2.pos_node) });
			//ClosedList.erase(ClosedList.begin(), ClosedList.begin() + 2);
			//ClosedList2.erase(ClosedList2.end(), ClosedList2.end() - 2);
			//ClosedList2.pop_back();
			//ClosedList2.pop_front();
			//ClosedList.pop_back();
			//ClosedList.pop_back();
			//ClosedList2.pop_back();
			
			if (!ClosedList2.empty())
			{
//				ClosedList2.pop_back();
	//			if (!ClosedList2.empty())
				{
					ClosedList2.pop_front();
				}
			}

			for (int j = 0; j < index_j-1; j++)
			{
				FinalClosedList.push_front(ClosedList2[j]);
			}

			//for (std::deque<Nodes>::iterator it = ClosedList.end(); it != ClosedList.begin(); --it)
			for(int i= index_i; i>0; i--)
			{
				FinalClosedList.push_front(ClosedList[i]);
			}
			//if (!FinalClosedList.empty())
				//FinalClosedList.pop_front();

			if (copy == 1)
			{
				drawList = FinalClosedList;
				drawListsphere = drawList;
			}
				
			
			goal = 1;
			OpenList.clear();
			OpenList2.clear();
		}

		else
		{
			if ((OpenList.front().f <= OpenList2.front().f) && !OpenList.empty())
			{
				current_node = OpenList.front();

				Neighbours.clear();

				for (int j = 0; j < current_node.all_links.size(); j++)
				{
					int tempID = getNodeID(current_node.all_links[j].other_node);///////////////////////
					ring_nodes[tempID].cost = calculateweight(ring_nodes[tempID].pos_node, current_node.pos_node, world);
					ring_nodes[tempID].h = m_target_position.getDistanceXZ(current_node.pos_node);
					ring_nodes[tempID].f = ring_nodes[tempID].cost + ring_nodes[tempID].h;
					Neighbours.push_back(ring_nodes[tempID]);
				}

				for (int i = 0; i < Neighbours.size(); i++)
				{
					if (!check_similarity_closedList(Neighbours[i].pos_node))
					{
						if (!check_similarity_OpenList(Neighbours[i].pos_node))
						{
							Neighbours[i].cost += current_node.cost;
							OpenList.push_back({ Neighbours[i] });//.pos_node, (Neighbours[i].cost + current_node.cost) });
						}
						else
						{
							if (Neighbours[i].cost < OpenList[temp].cost && Neighbours[i].pos_node == OpenList[temp].pos_node)
							{
								OpenList.erase(OpenList.begin() + temp);
								Neighbours[i].cost += current_node.cost;
								OpenList.push_back({ Neighbours[i] });//.pos_node, (Neighbours[i].cost + current_node.cost) });

							}
						}
					}
				}

				//ClosedList.push_back({ current_node.pos_node,  current_node.cost });
				ClosedList.push_back({ current_node.pos_node,  current_node.cost,  m_target_position.getDistanceXZ(current_node.pos_node) });
				OpenList.pop_front();
				
			}

			else if ((OpenList2.front().f <= OpenList.front().f) && !OpenList2.empty())
			{
				current_node2 = OpenList2.front();

				Neighbours.clear();

				for (int j = 0; j < current_node2.all_links.size(); j++)
				{
					int tempID = getNodeID(current_node2.all_links[j].other_node);///////////////////////
					ring_nodes[tempID].cost = calculateweight(ring_nodes[tempID].pos_node, current_node2.pos_node, world);
					ring_nodes[tempID].h = m_target_position.getDistanceXZ(current_node2.pos_node);
					ring_nodes[tempID].f = ring_nodes[tempID].cost + ring_nodes[tempID].h;
					Neighbours.push_back(ring_nodes[tempID]);
				}

				for (int i = 0; i < Neighbours.size(); i++)
				{
					if (!check_similarity_closedList2(Neighbours[i].pos_node))
					{
						if (!check_similarity_OpenList2(Neighbours[i].pos_node))
						{
							Neighbours[i].cost += current_node2.cost;
							OpenList2.push_back({ Neighbours[i] });//.pos_node, (Neighbours[i].cost + current_node.cost) });
						}
						else
						{
							if (Neighbours[i].cost < OpenList2[temp].cost && Neighbours[i].pos_node == OpenList2[temp].pos_node)
							{
								OpenList2.erase(OpenList2.begin() + temp);
								Neighbours[i].cost += current_node2.cost;
								OpenList2.push_back({ Neighbours[i] });//.pos_node, (Neighbours[i].cost + current_node.cost) });

							}
						}
					}
				}

				//ClosedList.push_back({ current_node.pos_node,  current_node.cost });
				ClosedList2.push_back({ current_node2.pos_node,  current_node2.cost,  m_target_position.getDistanceXZ(current_node2.pos_node) });
				OpenList2.pop_front();
				
			}
		}

		

		
	
		
	}


}

bool Ring::valuesort(const Nodes& a, const Nodes& b)
{
	//cout << "Compare: " << a.search_data_node[0].g << "& " << a.search_data_node[0].g;
	if (a.cost == b.cost)
		return a.pos_node.x < b.pos_node.x;
	else
		return a.f < b.f;
	
}


bool Ring::check_similarity_closedList(Vector3 check)
{
	bool flag = 0;
	
	for (int j = 0; j < ClosedList.size(); j++)
	{
		if (check == ClosedList[j].pos_node)
		{
			
			return 1; //Return true if similar
		}
	}
	return 0;
}

bool Ring::check_similarity_closedList2(Vector3 check)
{
	bool flag = 0;

	for (int j = 0; j < ClosedList2.size(); j++)
	{
		if (check == ClosedList2[j].pos_node)
		{

			return 1; //Return true if similar
		}
	}
	return 0;
}


bool Ring::check_similarity_OpenList(Vector3 check)
{
	bool flag = 0;

	for (int j = 0; j < OpenList.size(); j++)
	{
		if (check == OpenList[j].pos_node)
		{
			temp = j;
			return 1; //Return true if similar
		}
	}
	return 0;
}

bool Ring::check_similarity_OpenList2(Vector3 check)
{
	bool flag = 0;

	for (int j = 0; j < OpenList2.size(); j++)
	{
		if (check == OpenList2[j].pos_node)
		{
			temp = j;
			return 1; //Return true if similar
		}
	}
	return 0;
}

bool Ring::similarClosedList()
{
	for (int i = 0; i < ClosedList.size(); i++)
	{
		for (int j = 0; j < ClosedList2.size(); j++)
		{
			if (ClosedList2[j].pos_node == ClosedList[i].pos_node)
			{
				index_j = j;
				index_i = i;
				return 1;
			}
		}

	}
	return 0;
}

int Ring::getNodeID(Vector3 pos)
{
	for (int i = 0; i < ring_nodes.size(); i++)
	{
		if (ring_nodes[i].pos_node == pos)
			return i;
	}
	return 0;
}


double Ring::calculateweight(Vector3 posa, Vector3 posb, World& world)
{

	unsigned int disk_type = world.getClosestDisk(startpos).getDiskType();
	float move_speed = MOVE_SPEED_BASE * DiskType::getRingSpeedFactor(disk_type);
	


	

	 double rad = world.getClosestDisk(startpos).getRadius();
	 Vector3 pos = world.getClosestDisk(m_position).getPosition();
		//pos.addComponentsAll(rad);
	if((pos.getDistanceXZ(posa) < rad) && (pos.getDistanceXZ(posb)))
	{
		
		Vector3 center_to_node1 = posa - world.getClosestDisk(startpos).getPosition();

		Vector3 center_to_node2 = posb - world.getClosestDisk(startpos).getPosition();

		double  arc_radians = center_to_node1.getAngle(center_to_node2);

		double arc_radius = world.getClosestDisk(startpos).getRadius() - 0.7;

		double arc_length = arc_radius * arc_radians;

		return arc_length;
	}

	else
	{
		double dist_nodes = posa.getDistance(posb);
		double cost_i = 1.0 / move_speed;
		double cost_j = 1.0 / move_speed;

		double weight = dist_nodes * ((cost_i + cost_j) / 2.0);
		return weight;
	}

}

void Ring::drawSpheres() const
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	glColor3d(1.0, 1.0, 1.0);
	glPushMatrix();
		glTranslated(m_target_position.x, 0.1, m_target_position.z);
		//glScaled(3.0, 3.0, 3.0);
		glutSolidSphere(3.0, 50, 50);

	glPopMatrix();

	glColor3d(0.0, 1.0, 1.0);
	glPushMatrix();
		glTranslated(startpos.x, 0.1, startpos.z);
		//glScaled(3.0, 3.0, 3.0);
		glutSolidSphere(3.0, 50, 50);

	glPopMatrix();

//	glColor3d(0.0, 0.5, 1.0);
	for (int i = 0; i < ClosedList.size(); i++)
	{
		double c = (0.5 / ClosedList.size());
		glColor3d(0.0, 1.0 - (i *c ), 1.0);
		glPushMatrix();

		glTranslated(ClosedList[i].pos_node.x, 0.1, ClosedList[i].pos_node.z);
		//glScaled(3.0, 3.0, 3.0);
		glutSolidSphere(1.5, 50, 50);

		glPopMatrix();
	}


	
	for (int i = 0; i < ClosedList2.size(); i++)
	{
		double c = (1 / ClosedList2.size());
		glColor3d(1.0, 1.0 - (i* c), 1.0);
		glPushMatrix();

		glTranslated(ClosedList2[i].pos_node.x, 0.1, ClosedList2[i].pos_node.z);
		//glScaled(3.0, 3.0, 3.0);
		glutSolidSphere(1.5, 50, 50);

		glPopMatrix();
	}
}

/*for (int i = 0; i < Neighbours.size(); i++)
			{
				
				{
					if (Neighbours[i].pos_node != OpenList[k].pos_node)
					{

						
						//gl = gl-1;
					}
					else
					{
						

					}
				}
			}*/