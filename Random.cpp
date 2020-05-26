//
//  Random.cpp
//

#include <cassert>
#include <ctime>
#include <cstdlib>

#include "Random.h"



void seedRandom ()
{
	srand((unsigned int)time(NULL));

	//
	//  Seeding a pseudorandom number with similar time
	//    values, such as a few seconds or minutes apart,
	//    produces similar results for the first few
	//    pseudorandom numbers generated.  Although the
	//    numbers become less similar as more are generated,
	//    the first few numbers often have the largest
	//    outcome of the program, which can give biased
	//    results when testing.
	//
	//  To avoid this problem, the first few pesudorandom
	//    numbers generated are discarded.
	//

	for(unsigned int i = 0; i < 0x20; i++)
		rand();
}
