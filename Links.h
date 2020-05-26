#pragma once

#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "ObjLibrary/Vector3.h"
//#include "Nodes.h"


using namespace std;
using namespace ObjLibrary;

class Links
{
public:
	Links();
	Links(Vector3 node, double weightfound);
	~Links();
	
	Vector3 other_node;
	double weight;
};

