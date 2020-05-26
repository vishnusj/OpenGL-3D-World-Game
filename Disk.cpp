//
//  Disk.cpp
//

#include <cassert>
#include <iostream>

#include "GetGlut.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/TextureManager.h"

#include "Pi.h"
#include "Random.h"
#include "DiskType.h"
#include "Disk.h"


using namespace std;
using namespace ObjLibrary;
namespace
{
	const double SQRT2      = 1.4142135623730950488016887242097;
	const double HALF_SQRT2 = SQRT2 * 0.5;

	bool g_is_initialized = false;
	DisplayList ga_disk_list[DiskType::COUNT];



	const bool DEBUGGING_GET_HEIGHT = false;
}



const float Disk :: RADIUS_MIN =  6.0f;
const float Disk :: RADIUS_MAX = 50.0f;



unsigned int Disk :: calculateDiskType (float radius)
{
	assert(radius >= RADIUS_MIN);
	assert(radius <= RADIUS_MAX);

	if(radius < 8.0f)
		return DiskType::RED_ROCK;
	else if(radius < 12.0f)
		return DiskType::LEAFY;
	else if(radius < 20.0f)
		return DiskType::ICY;
	else if(radius < 30.0f)
		return DiskType::SANDY;
	else
		return DiskType::GREY_ROCK;
}

bool Disk :: isModelsLoaded ()
{
	return g_is_initialized;
}

void Disk :: loadModels ()
{
	assert(!isModelsLoaded());

	g_is_initialized = true;

	for(unsigned int i = 0; i < DiskType::COUNT; i++)
		ga_disk_list[i] = ObjModel(DiskType::getModelName(i)).getDisplayList();

	assert(isModelsLoaded());
}



Disk :: Disk ()
		: m_position()
		, m_radius(RADIUS_MIN)
		, m_disk_type(calculateDiskType(m_radius))
		, m_heightmap(m_disk_type, false)
		, m_rotation_radians(random1((float)(TWO_PI)))
{
	assert(invariant());
}

Disk :: Disk (const Vector3& position, float radius) : m_position(position), m_radius(radius), m_disk_type(calculateDiskType(m_radius)), m_heightmap(m_disk_type, true)
		, m_rotation_radians(random1((float)(TWO_PI)))
{
	assert(position.y == 0);
	assert(radius >= RADIUS_MIN);
	assert(radius <= RADIUS_MAX);

	assert(invariant());
}



const Vector3& Disk :: getPosition () const
{
	return m_position;
}

float Disk :: getRadius () const
{
	return m_radius;
}

unsigned int Disk :: getDiskType () const
{
	return m_disk_type;
}
	
float Disk :: getHeight (const Vector3& position) const
{
	if(DEBUGGING_GET_HEIGHT)
	{
		cout << "getHeight" << endl;
		cout << "\tDisk position: " << m_position << endl;
		cout << "\tDisk radius: " << m_radius << endl;
		cout << "\tDisk rotation: " << m_rotation_radians << " radians" << endl;
		cout << "\tHeightmap size: " << m_heightmap.getSideLength() << endl;
		cout << "\tPlayer position: " << position << endl;
	}

	Vector3 translated_position = position - m_position;
	if(DEBUGGING_GET_HEIGHT)
		cout << "\ttranslated_position: " << translated_position << endl;

	Vector3 rotated_position = translated_position.getRotatedY(-m_rotation_radians);
	if(DEBUGGING_GET_HEIGHT)
		cout << "\trotated_position: " << rotated_position << endl;

	assert(m_radius > 0.0f);
	Vector3 scaled_position = rotated_position / m_radius;
	if(DEBUGGING_GET_HEIGHT)
		cout << "\tscaled_position: " << scaled_position << endl;

	Vector3 translated_position2 = scaled_position - Vector3(-HALF_SQRT2, 0.0, -HALF_SQRT2);
	if(DEBUGGING_GET_HEIGHT)
		cout << "\ttranslated_position2: " << translated_position2 << endl;

	assert(SQRT2 > 0.0f);
	Vector3 scaled_position2 = translated_position2 / SQRT2;
	if(DEBUGGING_GET_HEIGHT)
		cout << "\tscaled_position2: " << scaled_position2 << endl;

	float heightmap_i = (float)(scaled_position2.x) * m_heightmap.getSideLength();
	float heightmap_j = (float)(scaled_position2.z) * m_heightmap.getSideLength();
	if(DEBUGGING_GET_HEIGHT)
	{
		cout << "\tHeightmap coordinates: (" << heightmap_i << ", " << heightmap_j << ")" << endl;
		cout << "\tHeight: " << m_heightmap.getHeight(heightmap_i, heightmap_j) << endl;
	}
	return m_heightmap.getHeight(heightmap_i, heightmap_j);
}

void Disk :: draw () const
{
	assert(isModelsLoaded());

	float rotation_degrees = (float)(m_rotation_radians * 180.0f / PI);

	glPushMatrix();
		// translate from world coordinates to disk coordinates
		glTranslated(m_position.x, m_position.y, m_position.z);
		glRotated(rotation_degrees, 0.0, 1.0, 0.0);
		glScaled(m_radius, 1.0, m_radius);

		// draw disk
		assert(m_disk_type < DiskType::COUNT);
		ga_disk_list[m_disk_type].draw();

		// translate from disk coordinates to heightmap coordinates
		glTranslated(-HALF_SQRT2, 0.0, -HALF_SQRT2);
		glScaled(SQRT2, 1.0, SQRT2);

		// draw heightmap
		glColor3d(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		TextureManager::activate(DiskType::getTextureName(m_disk_type));
		m_heightmap.draw();  // covers [0, 1] along x znd z
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}



bool Disk :: invariant () const
{
	if(m_position.y != 0) return false;
	if(m_radius < RADIUS_MIN) return false;
	if(m_radius > RADIUS_MAX) return false;
	if(m_disk_type != calculateDiskType(m_radius)) return false;
	if(m_rotation_radians > TWO_PI) return false;
	return true;
}
