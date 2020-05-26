#pragma once
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "ObjLibrary/Vector3.h"
#include "Search_data.h"
#include "Disk.h"

#include "Links.h"

using namespace std;
using namespace ObjLibrary;

class Nodes 
{
public:
	Nodes();
	~Nodes();

	Nodes(Vector3 node_pos);
	Nodes(Vector3 node_pos, double c, double v);

	Vector3 pos_node;
	static ObjLibrary::Vector3 calculateNodePosition(Disk disk_a, Disk disk_b);
	std::vector<Search_data> search_data_node;
	std::vector<Links> all_links;
	double cost, h, f;
};