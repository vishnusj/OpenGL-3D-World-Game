
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
#include "MovementGraph.h"

using namespace std;
using namespace ObjLibrary;

MovementGraph::MovementGraph(Vector3 pos, Disk did): Node_position(pos), disk_id(did)
{

}