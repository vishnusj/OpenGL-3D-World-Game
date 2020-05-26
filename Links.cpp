
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "ObjLibrary/Vector3.h"

#include "Links.h"

using namespace std;
using namespace ObjLibrary;
Links::Links()
{
}

Links::Links(Vector3 node, double weightfound) : other_node(node), weight(weightfound)
{

}

Links::~Links()
{
}

