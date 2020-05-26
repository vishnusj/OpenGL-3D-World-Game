#pragma once

#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "ObjLibrary/Vector3.h"
//#include "Nodes.h"


class Search_data
{
public:
	Search_data();
	Search_data(int node_id, double weight, double heur, double fval);
	~Search_data();

	int previous_node_id;
	double g;
	double h;
	double f;
};

