//
//  Random.h
//
//  A module to provide psuedorandom numbers in conveniant
//    ranges.
//
//  These functions cannot just be called random becuase this
//    causes a conflict with the POSIX random function.
//

#ifndef RANDOM_H
#define RANDOM_H

#include <cassert>
#include <cstdlib>



//
//  seedRandom
//
//  Purpose: To seed the refault random number generator.
//  Parameter(s): N/A
//  Precondition: N/A
//  Returns: N/A
//  Side Effect: The default random number generator is seeded.
//  Note: Calls the srand() function.
//
void seedRandom ();

//
//  random0
//
//  Purpose; To generate a psuedorandom number in the range
//           [0, 1).
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A pseudorandom number in the range [0, 1).  The
//           value returned can be 0 but cannot be 1.
//  Side Effect: N/A
//  Note: Calls the rand() function.
//
inline double random0 ()
{
	// RAND_MAX can be MAX_INT (and is on g++)
	return (double)(rand()) / (RAND_MAX + 1.0);
}

//
//  random1
//
//  Purpose; To generate a psuedorandom number in the range from
//           0 to the specified maximum.
//  Parameter(s):
//    <1> max: The exclusive maximum
//  Precondition(s):
//    <1> 0 <= max
//  Returns: A pseudorandom number in the range [0, max).  If
//           max == 0, (T)(0) is always returned.  Otherwise the
//           value returned can be 0 but cannot be max.
//  Side Effect: N/A
//  Note: Calls the rand() function
//  T must define:
//    <1> bool operator<= (int, T)
//    <2> U operator* (T, double)
//    <3> typecast from U to T
//
template <typename T>
inline T random1 (T max)
{
	assert(0 <= max);

	return (T)(max * random0());
}



//
//  random2
//
//  Purpose: To generate a psuedorandom number in the specified
//           range.
//  Parameter(s):
//    <1> min: The inclusive minimum
//    <2> max: The exclusive maximum
//  Precondition(s):
//    <1> min <= max
//  Returns: A pseudorandom number in the range [min, max).  If
//           min == max, min is always returned.  Otherwise the
//           value returned can be min but cannot be max.
//  Side Effect: N/A
//  Note: Calls the rand() function
//  T must define:
//    <1> bool operator<= (T, T)
//    <2> T operator- (T, T)
//    <3> T operator+ (T, T)
//    <4> U operator* (T, double)
//    <5> typecast from U to T
//
template <typename T>
inline T random2 (T min, T max)
{
	assert(min <= max);

	return min + (T)((max - min) * random0());
}

//
//  randomBool
//
//  Purpose; To generate a random boolean value.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: true or false, with equal probabilities.
//  Side Effect: N/A
//  Note: Calls the rand() function
//
inline bool randomBool ()
{
	static const int HALF_RAND_MAX = RAND_MAX / 2;
	return (rand() < HALF_RAND_MAX);
}



#endif
