//
//  NoiseField.h
//



//
//  NoiseField
//
//  Class Invariant:
//    <1> m_grid_size > 0.0f
//
class NoiseField
{
private:
	struct Vec2
	{
		float x;
		float y;

		Vec2 (float X, float Y)
			: x(X), y(Y)
		{ }
	};

public:
	NoiseField () = default;
	NoiseField (float grid_size,
	            float amplitude,
	            unsigned int seed_x1,
	            unsigned int seed_x2,
	            unsigned int seed_y1,
	            unsigned int seed_y2,
	            unsigned int seed_q0,
	            unsigned int seed_q1,
	            unsigned int seed_q2);

	float value (float x, float y) const;
	float perlin (float x, float y) const;

	void init (float grid_size,
	           float amplitude,
	           unsigned int seed_x1,
	           unsigned int seed_x2,
	           unsigned int seed_y1,
	           unsigned int seed_y2,
	           unsigned int seed_q0,
	           unsigned int seed_q1,
	           unsigned int seed_q2);

private:
	unsigned int pseudorandom (unsigned int x,
	                           unsigned int y) const;
	float fade (float n) const;
	Vec2 lattice (int x, int y) const;
	float dotProduct (const Vec2& a, const Vec2& b) const;
	bool invariant () const;

private:
	float m_grid_size;
	float m_amplitude;
	unsigned int m_seed_x1;
	unsigned int m_seed_x2;
	unsigned int m_seed_y1;
	unsigned int m_seed_y2;
	unsigned int m_seed_q0;
	unsigned int m_seed_q1;
	unsigned int m_seed_q2;
};

