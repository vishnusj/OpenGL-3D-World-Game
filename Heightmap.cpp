//
//  Heightmap.h
//

#include <cassert>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>  // for min, max

#include "GetGlut.h"
#include "Pi.h"
#include "Random.h"
#include "NoiseField.h"
#include "ObjLibrary/Vector2.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/DisplayList.h"

#include "DiskType.h"
#include "Heightmap.h"

using namespace std;
using namespace ObjLibrary;
namespace
{
	//
	//  sqr
	//
	//  Purpose: To square the specified value.
	//  Parameter(s):
	//    <1> v: The value to square
	//  Precondition(s): N/A
	//  Returns: v * v.
	//  Side Effect: N/A
	//
	template <typename T>
	inline T sqr (T v)
	{
		return v * v;
	}

	//
	//  cube
	//
	//  Purpose: To cube the specified value.
	//  Parameter(s):
	//    <1> v: The value to cube
	//  Precondition(s): N/A
	//  Returns: v * v.
	//  Side Effect: N/A
	//
	template <typename T>
	inline T cube (T v)
	{
		return v * v * v;
	}

	//
	//  intPow
	//
	//  Purpose: To calculate the result of a value raised to an
	//           non-negative integer exponent.
	//  Parameter(s):
	//    <1> v: The value
	//    <2> e: The exponent
	//  Precondition(s): N/A
	//  Returns: v^e.
	//  Side Effect: N/A
	//
	template <typename T>
	inline T intPow (T v, unsigned int e)
	{
		switch(e)
		{
		case 0: return (T)(0);
		case 1: return v;
		case 2: return sqr(v);
		case 3: return cube(v);
		default:
			{
				T v_po_e_div_2 = intPow(v, e / 2);
				T v_po_e_lower = v_po_e_div_2 * v_po_e_div_2;
				if(e % 2 == 0)
					return v_po_e_lower;
				else
					return v_po_e_lower * v;
			}
		}
	}



	const bool DEBUGGING_3D_MESH = false;
	const bool DEBUGGING_SET_HEIGHTS_RED_ROCK = false;
	const bool DEBUGGING_SET_HEIGHTS_ICY      = false;
	const bool DEBUGGING_SET_HEIGHTS_SANDY    = false;

}  // end of anonymouse namespace



const float Heightmap :: HEIGHT_EDGE = 0.0f;



Heightmap :: Heightmap ()
		: m_side_length(SIDE_LENGTH_MIN)
		, m_texture_repeat(TEXTURE_REPEAT_MIN)
		, mv_heights(m_side_length * m_side_length, HEIGHT_EDGE)
		, m_mesh()
{
	assert(invariant());
}

Heightmap :: Heightmap (unsigned int disk_type,
                        bool is_finalize)
		: m_side_length(DiskType::getSideLength(disk_type))
		, m_texture_repeat(DiskType::getTexureRepeatCount(disk_type))
		, mv_heights(m_side_length * m_side_length, HEIGHT_EDGE)
		, m_mesh()
{
	assert(disk_type < DiskType::COUNT);
	assert(DiskType::getSideLength(disk_type) >= SIDE_LENGTH_MIN);
	assert(DiskType::getTexureRepeatCount(disk_type) >= TEXTURE_REPEAT_MIN);

	switch(disk_type)
	{
	case DiskType::RED_ROCK:  setHeightsRedRock();  break;
	case DiskType::LEAFY:     setHeightsLeafy();    break;
	case DiskType::ICY:       setHeightsIcy();      break;
	case DiskType::SANDY:     setHeightsSandy();    break;
	case DiskType::GREY_ROCK: setHeightsGreyRock(); break;
	}

	finalize();

	assert(isFinalized());
	assert(invariant());
}



bool Heightmap :: isFinalized () const
{
	return m_mesh.isReady();
}

unsigned int Heightmap :: getSideLength () const
{
	return m_side_length;
}

unsigned int Heightmap :: getTextureRepeat () const
{
	return m_texture_repeat;
}
	
float Heightmap :: getVertexHeight (unsigned int i,
                                    unsigned int j) const
{
	assert(i < getSideLength());
	assert(j < getSideLength());

	assert(getVertexIndex(i, j) < mv_heights.size());
	return mv_heights[getVertexIndex(i, j)];
}

float Heightmap :: getVertexHeightTolerant (int i, int j) const
{
	if(               i  <= 0)             return HEIGHT_EDGE;
	if((unsigned int)(i) >= m_side_length) return HEIGHT_EDGE;
	if(               j  <= 0)             return HEIGHT_EDGE;
	if((unsigned int)(j) >= m_side_length) return HEIGHT_EDGE;

	assert(getVertexIndex(i, j) < mv_heights.size());
	return mv_heights[getVertexIndex(i, j)];
}

float Heightmap :: getHeight (float i, float j) const
{
	if(i <= 0.0f)          return HEIGHT_EDGE;
	if(i >= m_side_length) return HEIGHT_EDGE;
	if(j <= 0.0f)          return HEIGHT_EDGE;
	if(j >= m_side_length) return HEIGHT_EDGE;

	int i0 = (int)(i);
	int i1 = i0 + 1;
	float fraction_i1 = i - i0;

	int j0 = (int)(j);
	int j1 = j0 + 1;
	float fraction_j1 = j - j0;

	float height_00 = getVertexHeightTolerant(i0, j0);
	float height_10 = getVertexHeightTolerant(i1, j0);
	float height_01 = getVertexHeightTolerant(i0, j1);
	float height_11 = getVertexHeightTolerant(i1, j1);

	//
	//      0    if     2
	//    ^   +-------+
	//    |   |     / |
	//  j |   |   /   | else
	//    |   | /     |
	//    0   +-------+
	//      1           3
	//        0------->
	//            i
	//

	if(fraction_i1 < fraction_j1)
	{
		float fraction_01 = fraction_j1 - fraction_i1;
		return height_00 * (1.0f - fraction_j1) +
		       height_11 * fraction_i1 +
		       height_01 * fraction_01;
	}
	else
	{
		float fraction_10 = fraction_i1 - fraction_j1;
		return height_00 * (1.0f - fraction_i1) +
		       height_11 * fraction_j1 +
		       height_10 * fraction_10;
	}
}


void Heightmap :: draw () const
{
	assert(isFinalized());

	assert(m_mesh.isReady());
	m_mesh.draw();
}

void Heightmap :: setVertexHeight (unsigned int i,
                                   unsigned int j,
                                   float height)
{
	assert(!isFinalized());
	assert(i < getSideLength());
	assert(j < getSideLength());
	assert((i >= 1 && j >= 1) || height == HEIGHT_EDGE);

	assert(getVertexIndex(i, j) < mv_heights.size());
	mv_heights[getVertexIndex(i, j)] = height;

	assert(invariant());
}

void Heightmap :: finalize ()
{
	assert(!isFinalized());

	//
	//    0   1   2   3   0
	//  0 +---+---+---+---+ 0
	//    | / | / | / | / |
	//  3 +---+---+---+---+ 3
	//    | / | / | / | / |       VERTEX
	//  2 +---+---+---+---+ 2   HEIGHTS IN
	//    | / | / | / | / |       VECTOR
	//  1 +---+---+---+---+ 1
	//    | / | / | / | / |
	//  0 +---+---+---+---+ 0
	//    0   1   2   3   0
	//
	//    0   2   4   6   8
	//    +---+---+---+---+    VERTEX
	//    | / | / | / | / |    DRAWING
	//    +---+---+---+---+     ORDER
	//    1   3   5   7   9
	//

	if(DEBUGGING_3D_MESH)
		cout << "Constructing mesh" << endl;

	m_mesh.begin();
		for(unsigned int i0 = 0; i0 < m_side_length; i0++)
		{
			unsigned int i1 = i0 + 1;

			// using modulus to avoid array index out of bounds on last vertex
			unsigned int i0_index = i0 % m_side_length;
			unsigned int i1_index = i1 % m_side_length;

			float i_fraction0 = (float)(i0) / (float)(m_side_length);
			float i_fraction1 = (float)(i1) / (float)(m_side_length);

			float tex_i0 = (float)(i0) / m_side_length * m_texture_repeat;
			float tex_i1 = (float)(i1) / m_side_length * m_texture_repeat;

			glBegin(GL_TRIANGLE_STRIP);
				for(unsigned int j = 0; j <= m_side_length; j++)
				{
					// using modulus to avoid array index out of bounds on last vertex
					unsigned int j_index = j % m_side_length;
					float j_fraction = (float)(j) / (float)(m_side_length);
					float tex_j = (float)(j) / m_side_length * m_texture_repeat;

					float height0 = getVertexHeight(i0_index, j_index);
					float height1 = getVertexHeight(i1_index, j_index);

					glTexCoord2d(tex_i1, tex_j); glVertex3d(i_fraction1, height1, j_fraction);
					glTexCoord2d(tex_i0, tex_j); glVertex3d(i_fraction0, height0, j_fraction);
				}
			glEnd();
		}
	m_mesh.end();

	if(DEBUGGING_3D_MESH)
		cout << "\tFinished" << endl;

	assert(isFinalized());
	assert(invariant());
}

void Heightmap :: unfinalize ()
{
	assert(isFinalized());

	m_mesh.makeEmpty();

	assert(!isFinalized());
	assert(invariant());
}



unsigned int Heightmap :: getVertexIndex (unsigned int i,
                                          unsigned int j) const
{
	assert(i < getSideLength());
	assert(j < getSideLength());

	return j * m_side_length + i;
}

void Heightmap :: setHeightsRedRock ()
{
	assert(!isFinalized());

	static const unsigned int RING_COUNT = 8 + 1;
	assert(RING_COUNT >= 1);
	assert(DiskType::getSideLength(DiskType::RED_ROCK) + 1 == RING_COUNT * 2 - 1);

	if(DEBUGGING_SET_HEIGHTS_RED_ROCK)
		cout << "setHeightsRedRock" << endl;

	// choose a height for each ring
	float a_ring_height[RING_COUNT];
	a_ring_height[0] = 0.0f;
	a_ring_height[1] = 0.0f;
	for(unsigned int r = 2; r < RING_COUNT; r++)
	{
		assert(r >= 1);
		a_ring_height[r] = a_ring_height[r - 1] + random2(-1.0f, 2.0f);
	}

	if(DEBUGGING_SET_HEIGHTS_RED_ROCK)
	{
		cout << "Rings:";
		for(unsigned int r = 0; r < RING_COUNT; r++)
			cout << "\t" << a_ring_height[r];
		cout << endl;
	}

	// set vertexes to ring height
	int center = m_side_length / 2;

	if(DEBUGGING_SET_HEIGHTS_RED_ROCK)
	{
		cout << "Vertexes:" << endl;
		cout << "\tCenter: " << center << endl;
	}

	for(unsigned int j = 1; j < m_side_length; j++)
		for(unsigned int i = 1; i < m_side_length; i++)
		{
			unsigned int diff_i   = abs((int)(i) - center);
			unsigned int diff_j   = abs((int)(j) - center);
			unsigned int diff_max = (diff_i > diff_j) ? diff_i : diff_j;

			if(DEBUGGING_SET_HEIGHTS_RED_ROCK)
				cout << "\t(" << i << ", " << j << ")  diffs: " << diff_i << ", " << diff_j << " => " << diff_max << endl;

			assert(diff_max <= RING_COUNT - 1);
			unsigned int ring = RING_COUNT - 1 - diff_max;

			if(DEBUGGING_SET_HEIGHTS_RED_ROCK)
				cout << "\t\tRing: " << ring << " (" << a_ring_height[ring] << ")" << endl;

			setVertexHeight(i, j, a_ring_height[ring]);
		}
}

void Heightmap :: setHeightsLeafy ()
{
	assert(!isFinalized());

	//
	//  i2 = i * 2 - 1
	//  j2 = j * 2 - 1
	//  dist    = min(sqrt(i2^2 + j2^2), 1)
	//  radians = atan2(j2, i2);
	//
	//  Straight lines:
	//    Left:   i^4 * (1 - i) * 12
	//    Center: (cos(pi * i2) + 1) * 0.5
	//    Right:  (1 - i)^4 * i * 12
	//
	//  Rings:
	//    Inner:               max(1 - dist,           0)^2
	//    Middle:     sin(pi * min(    dist^0.8 * 4/3, 1)  )
	//    Outer:  max(sin(pi *         dist^1.6            ), 0)
	//
	//  Arms:
	//    Inner:  (sqrt(    dist) -      dist ) * 4
	//    Middle: dist2^2 * (1 - dist)^2 * 16
	//    Outer:  (sqrt(1 - dist) - (1 - dist)) * 4
	//

	// choose some random parameters for the formula
	const double LL = random2(-1.0f, 1.0f);
	const double LC = random2(-1.0f, 1.0f);
	const double LR = random2(-1.0f, 1.0f);

	const double RI = random2(-1.0f, 1.0f);
	const double RM = random2(-1.0f, 1.0f);
	const double RO = random2(-1.0f, 1.0f);

	const unsigned int ARM_COUNT = min(random2(0, 7), random2(0, 7));
	const double ARM_RADIANS = random1(TWO_PI);
	const double ARM_UP      = randomBool() ? 1.0f : -1.0f;
	const double AI = random2(0.0f, 1.0f) * ARM_UP;
	const double AM = random2(0.0f, 1.0f) * ARM_UP;
	const double AO = random2(0.0f, 1.0f) * ARM_UP;

	// calculate the height for each vertex
	for(unsigned int j = 1; j < m_side_length; j++)
		for(unsigned int i = 1; i < m_side_length; i++)
		{
			// basic variables
			double i1 = (double)(i) / (double)(m_side_length);
			double j1 = (double)(j) / (double)(m_side_length);
			double i2 = i1 * 2.0 - 1.0;
			double j2 = j1 * 2.0 - 1.0;
			double dist    = min(sqrt(sqr(i2) + sqr(j2)), 1.0);
			double radians = atan2(j2, i2);

			// line/ring combination
			double line_left   = intPow(i1, 4) * (1.0f - i1) * 12.0f;
			double line_center = (cos(PI * i2) + 1) * 0.5;
			double line_right  = intPow(1.0f - i1, 4) * (i1) * 12.0f;
			double line_sum    = line_left * LL + line_center * LC + line_right * LR;

			double ring_inner  =     sqr(     max(1.0 -      dist,                   0.0));
			double ring_middle =     sin(PI * min(       pow(dist, 0.8) * 4.0 / 3.0, 1.0));
			double ring_outer  = max(sin(PI *            pow(dist, 1.6)                  ), 0.0);
			double ring_sum    = ring_inner * RI + ring_middle * RM + ring_outer * RO;

			double non_arm_height = line_sum * ring_sum;

			// arms
			double arm_inner  = (sqrt(dist) - dist) * 4.0;
			double arm_middle = sqr(dist) * sqr(1.0 - dist) * 16.0;
			double arm_outer  = (sqrt(1.0 - dist) - (1.0 - dist)) * 4.0;
			double arm_sum    = arm_inner * AI + arm_middle * AM + arm_outer * AO;

			double arm_magnitude = (sin(radians * ARM_COUNT + ARM_RADIANS) + 1.0) * 0.5;
			double arm_height    = arm_magnitude * arm_sum;

			// putting it together
			double weighted_sum = non_arm_height * 5.0 + arm_height * 3.0;
			setVertexHeight(i, j, (float)(weighted_sum));
		}
}

void Heightmap :: setHeightsIcy ()
{
	assert(!isFinalized());

	static const unsigned int POINT_COUNT  = 200;
	static const double       HEIGHT_SCALE = 0.7;

	if(DEBUGGING_SET_HEIGHTS_ICY)
		cout << "setHeightsIcy" << endl;

	float half_size = (m_side_length - 1.0f) * 0.5f;
	Vector2 half_offset(half_size + 1.0, half_size + 1.0);

	Vector2 a_points [POINT_COUNT];
	double  a_heights[POINT_COUNT];
	for(unsigned int p = 0; p < POINT_COUNT; p++)
	{
		double distance   = max(max(random0(), random0()), max(random0(), random0())) * half_size;
		double height_max = (half_size - distance) * HEIGHT_SCALE;
		a_points [p] = half_offset + Vector2::getRandomSphereVector() * distance;
		a_heights[p] = random2(-height_max, height_max);
	}

	if(DEBUGGING_SET_HEIGHTS_ICY)
	{
		cout << "Center: " << half_offset << endl;
		cout << "Points:" << endl;
		for(unsigned int p = 0; p < POINT_COUNT; p++)
			cout << "\t" << a_points[p] << ", " << a_heights[p] << endl;

		cout << "Vertexes:" << endl;
	}

	for(unsigned int j = 1; j < m_side_length; j++)
		for(unsigned int i = 1; i < m_side_length; i++)
		{
			Vector2 position(i, j);
			double lowest  = 0.0;
			double highest = 0.0;

			for(unsigned int p = 0; p < POINT_COUNT; p++)
			{
				double distance_scaled = position.getDistance(a_points[p]) * HEIGHT_SCALE;
				double high = a_heights[p] - distance_scaled;
				double low  = a_heights[p] + distance_scaled;

				if(high > highest)
					highest = high;
				if(low < lowest)
					lowest = low;
			}

			double sum = highest + lowest;
			if(DEBUGGING_SET_HEIGHTS_ICY)
				cout << "\t(" << i << ", " << j << "): " << highest << " - " << fabs(lowest) << " = " << sum << endl;

			setVertexHeight(i, j, (float)(sum));
		}
}

void Heightmap :: setHeightsSandy ()
{
	assert(!isFinalized());

	static const float CELL_SIZE = 16.0f;
	static const float AMPLITUDE = 8.0f;

	if(DEBUGGING_SET_HEIGHTS_SANDY)
		cout << "setHeightsSandy" << endl;

	NoiseField noise(CELL_SIZE, AMPLITUDE,
	                 (unsigned int)(random0() * UINT_MAX),
	                 (unsigned int)(random0() * UINT_MAX),
	                 (unsigned int)(random0() * UINT_MAX),
	                 (unsigned int)(random0() * UINT_MAX),
	                 (unsigned int)(random0() * UINT_MAX),
	                 (unsigned int)(random0() * UINT_MAX),
	                 (unsigned int)(random0() * UINT_MAX));

	if(DEBUGGING_SET_HEIGHTS_SANDY)
	{
		cout << "Created noise field" << endl;
		cout << "Vertexes:" << endl;
	}

	for(unsigned int j = 1; j < m_side_length; j++)
		for(unsigned int i = 1; i < m_side_length; i++)
		{
			float edge_factor = getEdgeFactor(i, j);
			float noise_at    = noise.perlin((float)(i), (float)(j));

			if(DEBUGGING_SET_HEIGHTS_SANDY)
				cout << "\t(" << i << ", " << j << "): " << edge_factor << " * " << noise_at << " = " << (noise_at * edge_factor) << endl;

			setVertexHeight(i, j, edge_factor * noise_at);
		}
}

void Heightmap :: setHeightsGreyRock ()
{
	assert(!isFinalized());

	static const unsigned int LEVEL_COUNT = 5;
	static const float A_CELL_SIZE[LEVEL_COUNT] =
	{	32.0f,	16.0f,	8.0f,	4.0f,	2.0f,	};
	static const float A_AMPLITUDE[LEVEL_COUNT] =
	{	10.0f,	7.0f,	5.0f,	3.5f,	2.5f,	};  // ~2^(-N/2)

	NoiseField a_noise[LEVEL_COUNT];

	for(unsigned int i = 0; i < LEVEL_COUNT; i++)
	{
		a_noise[i] = NoiseField(A_CELL_SIZE[i], A_AMPLITUDE[i],
	                            (unsigned int)(random0() * UINT_MAX),
	                            (unsigned int)(random0() * UINT_MAX),
	                            (unsigned int)(random0() * UINT_MAX),
	                            (unsigned int)(random0() * UINT_MAX),
	                            (unsigned int)(random0() * UINT_MAX),
	                            (unsigned int)(random0() * UINT_MAX),
	                            (unsigned int)(random0() * UINT_MAX));
	}

	for(unsigned int j = 1; j < m_side_length; j++)
		for(unsigned int i = 1; i < m_side_length; i++)
		{
			float edge_factor = getEdgeFactor(i, j);
			float noise_at    = 0.0f;
			for(unsigned int l = 0; l < LEVEL_COUNT; l++)
				noise_at += a_noise[l].perlin((float)(i), (float)(j));

			setVertexHeight(i, j, edge_factor * noise_at);
		}
}

float Heightmap :: getEdgeFactor (unsigned int i,
                                  unsigned int j) const
{
	assert(i < getSideLength());
	assert(j < getSideLength());

	static const unsigned int EXPONENT = 6;

	float i1 = i / (m_side_length + 1.0f);
	float i2 = 1.0f - i1;

	float j1 = j / (m_side_length + 1.0f);
	float j2 = 1.0f - j1;

	return (1.0f - max(max(intPow(i1, EXPONENT), intPow(i2, EXPONENT)),
	                   max(intPow(j1, EXPONENT), intPow(j2, EXPONENT)))) / ( 1.0f - intPow(0.5f, EXPONENT));
}

bool Heightmap :: invariant () const
{
	if(m_side_length < SIDE_LENGTH_MIN) return false;
	if(m_texture_repeat < TEXTURE_REPEAT_MIN) return false;
	if(mv_heights.size() != m_side_length * m_side_length) return false;
	if(m_mesh.isPartial()) return false;
	for(unsigned int i = 0; i < m_side_length; i++)
		if(mv_heights[getVertexIndex(i, 0)] != HEIGHT_EDGE)
			return false;
	for(unsigned int j = 0; j < m_side_length; j++)
		if(mv_heights[getVertexIndex(0, j)] != HEIGHT_EDGE)
			return false;
	return true;
}

