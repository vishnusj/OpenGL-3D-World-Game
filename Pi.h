//
//  Pi.h
//
//  This is the Pi constant.  I am fed up with name conflicts.
//

#ifndef PI_H
#define PI_H



const double PI = 3.14159265358979323846264338327950288419716939937510;

const double SIXTH_PI         = PI / 6.0;
const double QUARTER_PI       = PI * 0.25;
const double THIRD_PI         = PI / 3.0;
const double HALF_PI          = PI * 0.5;
const double TWO_THIRD_PI     = PI / 1.5;
const double THREE_QUARTER_PI = PI * 0.75;
const double FIVE_SIXTH_PI    = PI / 1.2;

const double SEVEN_SIXTH_PI   = PI * 7.0 / 6.0;
const double FIVE_QUARTER_PI  = PI * 1.25;
const double FOUR_THIRD_PI    = PI / 0.75;
const double THREE_HALF_PI    = PI * 1.5;
const double FIVE_THIRD_PI    = PI / 0.6;
const double SEVEN_QUARTER_PI = PI * 1.75;
const double ELEVEN_SIXTH_PI  = PI * 11.0 / 6.0;

const double TWO_PI = PI * 2.0;



//
//  degreesToRadians
//
//  Purpose: To convert the specified angle in degrees to an
//	     angle in radians.
//  Parameter(s):
//	<1> degrees: The angle in degrees
//  Precondition(s): N/A
//  Returns: Angle degrees in radians.
//  Side Effect: N/A
//
inline float degreesToRadians (float degrees)
{ return degrees * 0.017453292519943295769236907684886f; }
inline double degreesToRadians (double degrees)
{ return degrees * 0.017453292519943295769236907684886; }
inline long double degreesToRadians (long double degrees)
{ return degrees * 0.017453292519943295769236907684886L; }

//
//  radiansToDegrees
//
//  Purpose: To convert the specified angle in radians to an
//	     angle in degrees.
//  Parameter(s):
//	<1> radians: The angle in radians
//  Precondition(s): N/A
//  Returns: Angle radians in degrees.
//  Side Effect: N/A
//
inline float radiansToDegrees (float radians)
{ return radians * 57.295779513082320876798154814105f; }
inline double radiansToDegrees (double radians)
{ return radians * 57.295779513082320876798154814105; }
inline long double radiansToDegrees (long double radians)
{ return radians * 57.295779513082320876798154814105L; }



#endif
