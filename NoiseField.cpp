//
//  NoiseField.cpp
//

#include <cassert>
#include <climits>
#include <cmath>

#include "NoiseField.h"



NoiseField :: NoiseField (float grid_size,
                          float amplitude,
                          unsigned int seed_x1,
                          unsigned int seed_x2,
                          unsigned int seed_y1,
                          unsigned int seed_y2,
                          unsigned int seed_q0,
                          unsigned int seed_q1,
                          unsigned int seed_q2)
		: m_grid_size(grid_size)
		, m_amplitude(amplitude)
		, m_seed_x1(seed_x1)
		, m_seed_x2(seed_x2)
		, m_seed_y1(seed_y1)
		, m_seed_y2(seed_y2)
		, m_seed_q0(seed_q0)
		, m_seed_q1(seed_q1)
		, m_seed_q2(seed_q2)
{
	assert(invariant());
}



float NoiseField :: value (float x, float y) const
{
	int x0 = (int)(floor(x / m_grid_size));
	int y0 = (int)(floor(y / m_grid_size));
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float x_frac = x / m_grid_size - x0;
	float y_frac = y / m_grid_size - y0;

	float x_fade1 = fade(x_frac);
	float y_fade1 = fade(y_frac);
	float x_fade0 = 1.0f - x_fade1;
	float y_fade0 = 1.0f - y_fade1;

	unsigned int value00 = pseudorandom(x0, y0);
	unsigned int value01 = pseudorandom(x0, y1);
	unsigned int value10 = pseudorandom(x1, y0);
	unsigned int value11 = pseudorandom(x1, y1);

	unsigned int value0 = (unsigned int)(value00 * y_fade0) +
	                      (unsigned int)(value01 * y_fade1);
	unsigned int value1 = (unsigned int)(value10 * y_fade0) +
	                      (unsigned int)(value11 * y_fade1);
	unsigned int value = (unsigned int)(value0 * x_fade0) +
	                     (unsigned int)(value1 * x_fade1);

	float value_scaled = ((float)(value) / UINT_MAX) * 2.0f - 1.0f;
	return value_scaled * m_amplitude;
}

float NoiseField :: perlin (float x, float y) const
{
	int x0 = (int)(floor(x / m_grid_size));
	int y0 = (int)(floor(y / m_grid_size));
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float x_frac = x / m_grid_size - x0;
	float y_frac = y / m_grid_size - y0;

	float x_fade1 = fade(x_frac);
	float y_fade1 = fade(y_frac);
	float x_fade0 = 1.0f - x_fade1;
	float y_fade0 = 1.0f - y_fade1;

	Vec2 lattice00 = lattice(x0, y0);
	Vec2 lattice01 = lattice(x0, y1);
	Vec2 lattice10 = lattice(x1, y0);
	Vec2 lattice11 = lattice(x1, y1);

	Vec2 direction00(     - x_frac,      - y_frac);
	Vec2 direction01(     - x_frac, 1.0f - y_frac);
	Vec2 direction10(1.0f - x_frac,      - y_frac);
	Vec2 direction11(1.0f - x_frac, 1.0f - y_frac);

	float value00 = dotProduct(lattice00, direction00);
	float value01 = dotProduct(lattice01, direction01);
	float value10 = dotProduct(lattice10, direction10);
	float value11 = dotProduct(lattice11, direction11);

	float value0 = value00 * y_fade0 +
	               value01 * y_fade1;
	float value1 = value10 * y_fade0 +
	               value11 * y_fade1;
	float value = value0 * x_fade0 +
	              value1 * x_fade1;

	return value * m_amplitude;
}



void NoiseField :: init (float grid_size,
                         float amplitude,
                         unsigned int seed_x1,
                         unsigned int seed_x2,
                         unsigned int seed_y1,
                         unsigned int seed_y2,
                         unsigned int seed_q0,
                         unsigned int seed_q1,
                         unsigned int seed_q2)
{
	m_grid_size = grid_size;
	m_amplitude = amplitude;
	m_seed_x1 = seed_x1;
	m_seed_x2 = seed_x2;
	m_seed_y1 = seed_y1;
	m_seed_y2 = seed_y2;
	m_seed_q0 = seed_q0;
	m_seed_q1 = seed_q1;
	m_seed_q2 = seed_q2;

	assert(invariant());
}



unsigned int NoiseField :: pseudorandom (unsigned int x,
                                         unsigned int y) const
{
	unsigned int n = (m_seed_x1 * x) ^
	                 (m_seed_y1 * y);
	unsigned int quad_term = m_seed_q2 * n * n +
	                         m_seed_q1 * n     +
	                         m_seed_q0;
	return quad_term ^
	       (m_seed_x2 * x) ^
	       (m_seed_y2 * y);
}

float NoiseField :: fade (float n) const
{
	//return n;
	//return (1 - cos(n * 3.14159265f)) * 0.5f;
	//return (-2 * n + 3) * n * n;
	return ((6 * n - 15) * n + 10) * n * n * n;
}

NoiseField::Vec2 NoiseField :: lattice (int x, int y) const
{
	unsigned int value = pseudorandom(x, y);
	float radians = value * 6.28318530f;  // 2pi
	return Vec2(cos(radians), sin(radians));
}

float NoiseField :: dotProduct (const NoiseField::Vec2& a,
                                const NoiseField::Vec2& b) const
{
	return a.x * b.x + a.y * b.y;
}

bool NoiseField :: invariant () const
{
	if(m_grid_size <= 0.0f) return false;
	return true;
}


