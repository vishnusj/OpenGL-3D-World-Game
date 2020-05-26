//
//  Rod.cpp
//

#include <cassert>

#include "GetGlut.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/DisplayList.h"

#include "Rod.h"

using namespace ObjLibrary;
namespace
{
	bool g_is_initilaized = false;

	DisplayList g_display_list;
}



const float Rod :: RADIUS      = 0.1f;
const float Rod :: HALF_HEIGHT = 1.0f;



bool Rod :: isModelsLoaded ()
{
	return g_is_initilaized;
}

void Rod :: loadModels ()
{
	assert(!isModelsLoaded());

	g_display_list = ObjModel("Models/Rod.obj").getDisplayList();

	g_is_initilaized = true;

	assert(isModelsLoaded());
}



Rod :: Rod ()
		: m_position()
		, m_points(POINTS_MIN)
		, m_is_taken(false)
{
	assert(invariant());
}

Rod :: Rod (const ObjLibrary::Vector3& position,
            unsigned int points)
		: m_position(position)
		, m_points(points)
		, m_is_taken(false)
{
	assert(points >= POINTS_MIN);
	assert(points <= POINTS_MAX);

	assert(invariant());
}



const Vector3& Rod :: getPosition () const
{
	return m_position;
}

unsigned int Rod :: getPoints () const
{
	return m_points;
}

bool Rod :: isTaken () const
{
	return m_is_taken;
}

void Rod :: draw () const
{
	assert(isModelsLoaded());

	if(isTaken())
		return;

	glPushMatrix();
		glTranslated(m_position.x, m_position.y, m_position.z);
		g_display_list.draw();
	glPopMatrix();
}



void Rod :: markTaken ()
{
	m_is_taken = true;

	assert(invariant());
}



bool Rod :: invariant () const
{
	if(m_points < POINTS_MIN) return false;
	if(m_points > POINTS_MAX) return false;
	return true;
}
