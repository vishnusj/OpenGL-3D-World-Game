#pragma once
#include "Nodes.h"
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "ObjLibrary/Vector3.h"

#include "Disk.h"

#include "Links.h"

using namespace std;
using namespace ObjLibrary;

class MovementGraph
{
public:

	MovementGraph(Vector3 pos, Disk did);
	//~MovementGraph();

	Vector3 Node_position;
	Disk disk_id;
	std::vector<Links> disk_links;

	//Nodes node_found;
	
};