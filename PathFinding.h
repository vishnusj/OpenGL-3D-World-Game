#pragma once
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "ObjLibrary/Vector3.h"
#include "Nodes.h"
#include "Disk.h"

class PathFinding
{
public:
	PathFinding();
	~PathFinding();

	Vector3 path_position;
	Disk path_disk;
	int node_id;
};

