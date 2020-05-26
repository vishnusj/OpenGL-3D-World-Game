#include <fstream>
#include <vector>


#include "ObjLibrary/Vector3.h"

#include "Disk.h"

#include "Nodes.h"
//#include "Links.h"

using namespace std;
using namespace ObjLibrary;



Nodes::Nodes()
{
}


Nodes::~Nodes()
{
}

Nodes::Nodes(Vector3 node_pos) :pos_node(node_pos), cost(0.0), h(0.0), f(0.0)
{

}

Nodes::Nodes(Vector3 node_pos, double c, double v) : pos_node(node_pos), cost(c), h(v), f(c+v)
{

}


Vector3 Nodes::calculateNodePosition(Disk disk_a, Disk disk_b)
{
	Vector3 long_directionVector = disk_b.getPosition() - disk_a.getPosition();
	Vector3 unit_distanceVector = long_directionVector.getNormalized();
	double dist_a = disk_a.getRadius() - 0.7;


	Vector3 pos_aNode = disk_a.getPosition() + unit_distanceVector * dist_a;

	return pos_aNode;
}