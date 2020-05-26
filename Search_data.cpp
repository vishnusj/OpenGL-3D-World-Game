#include "Search_data.h"

Search_data::Search_data()
{

}
Search_data::Search_data(int node_id, double weight, double heur, double fval) : previous_node_id(node_id), g(weight), h(heur), f(fval)
{


}


Search_data::~Search_data()
{
}
