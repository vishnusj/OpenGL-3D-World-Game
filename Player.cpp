//
//  Player.cpp

#include <cassert>
#include <string>
#include <iostream>
#include <sstream>

#include "GetGlut.h"
#include "Pi.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/DisplayList.h"

#include "PhysicsFrameLength.h"
#include "DiskType.h"
#include "Heightmap.h"
#include "Disk.h"
#include "World.h"
#include "Player.h"

using namespace std;
using namespace ObjLibrary;
namespace
{
	// something powerful enough to compile in the right order
	#define FORWARD_DEFAULT Vector3(1.0, 0.0, 0.0)

	const Vector3 UP_VECTOR(0.0, 1.0, 0.0);
	const Vector3 GRAVITY = UP_VECTOR * -9.8;  // m/s^2

	const float JUMP_HORIZONTAL =  8.0f;
	const float JUMP_VERTICAL   = 12.0f;

	const string MODEL_NAME = "cbabe";
	//const string MODEL_NAME = "guard";
	const bool IS_LOAD_ANIMATION = true;
	const unsigned int RUN_FRAME_COUNT = 20;
	DisplayList g_stand_list;
	DisplayList g_jump_list;
	DisplayList ga_run_list[RUN_FRAME_COUNT];

	const unsigned int RUN_COUNTER_MAX = 2;
	const float RUN_FRAMES_FORWARD_PER_SECOND  = 20.0f;
	const float RUN_FRAMES_BACKWARD_PER_SECOND = 10.0f;



	const bool IS_CALCULATE_HALF_HEIGHT = false;
}



const float Player :: RADIUS      =    0.25f;
const float Player :: HALF_HEIGHT =    0.8f;
const float Player :: DEAD_Y      = -100.0f;



bool Player :: isModelsLoaded ()
{
	assert(g_jump_list.isReady() == g_stand_list.isReady());
	for(unsigned int i = 0; i < RUN_FRAME_COUNT; i++)
		assert(ga_run_list[i].isReady() == g_stand_list.isReady());

	return g_stand_list.isReady();
}

void Player :: loadModels ()
{
	assert(!isModelsLoaded());

	g_stand_list = ObjModel("Models/" + MODEL_NAME + "_stand.obj").getDisplayList();
	g_jump_list  = ObjModel("Models/" + MODEL_NAME + "_jump.obj" ).getDisplayList();
	for(unsigned int i = 0; i < RUN_FRAME_COUNT; i++)
	{
		if(IS_LOAD_ANIMATION || i == 0)
		{
			stringstream ss;
			ss << "Models/" << MODEL_NAME << "_run" << i << ".obj";
			ga_run_list[i] = ObjModel(ss.str()).getDisplayList();
		}
		else
		{
			assert(i > 0);
			assert(ga_run_list[0].isReady());
			ga_run_list[i] = ga_run_list[0];
		}
	}

	if(IS_CALCULATE_HALF_HEIGHT)
	{
		ObjModel stand("Models/" + MODEL_NAME + "_stand.obj");
		double y_min = 1.0e20;
		for(unsigned int v = 0; v < stand.getVertexCount(); v++)
			if(stand.getVertexY(v) < y_min)
				y_min = stand.getVertexY(v);
		cout << "Half height: " << y_min << endl;
	}

	assert(isModelsLoaded());
}



Player :: Player ()
		: m_position()
		, m_velocity()
		, m_forward(FORWARD_DEFAULT)
		, m_is_jumping(false)
		, m_run_counter(0)
		, m_run_frame(0.0f)
{
	assert(invariant());
}

Player :: Player (const ObjLibrary::Vector3& position)
		: m_position(position)
		, m_velocity()
		, m_forward(FORWARD_DEFAULT)
		, m_is_jumping(false)
		, m_run_counter(0)
		, m_run_frame(0.0f)
{
	assert(invariant());
}



const Vector3& Player :: getPosition () const
{
	return m_position;
}

const Vector3& Player :: getVelocity () const
{
	return m_velocity;
}

const Vector3& Player :: getForward () const
{
	return m_forward;
}

bool Player :: isJumping () const
{
	return m_is_jumping;
}

bool Player :: isDead () const
{
	return m_position.y <= DEAD_Y;
}

void Player :: draw () const
{
	assert(isModelsLoaded());

	glPushMatrix();
		float rotation = (float)(radiansToDegrees(m_forward.getRotationY()) - 90.0);
		glTranslated(m_position.x, m_position.y, m_position.z);
		glRotated(rotation, 0.0, 1.0, 0.0);

		if(m_is_jumping)
			g_jump_list.draw();
		else if(m_run_counter > 0)
		{
			double run_frame_fraction = fmod(m_run_frame, RUN_FRAME_COUNT);
			if(run_frame_fraction < 0.0)
				run_frame_fraction += RUN_FRAME_COUNT;
			assert(run_frame_fraction >= 0.0);
			assert(run_frame_fraction <  RUN_FRAME_COUNT);
			unsigned int run_frame_int = (unsigned int)(run_frame_fraction);
			assert(run_frame_int <  RUN_FRAME_COUNT);
			ga_run_list[run_frame_int].draw();
		}
		else
			g_stand_list.draw();
	glPopMatrix();
}



void Player :: init (const ObjLibrary::Vector3& position)
{
	m_position    = position;
	m_velocity.setZero();
	m_forward     = FORWARD_DEFAULT;
	m_is_jumping  = false;
	m_run_counter = 0;
	m_run_frame   = 0.0f;

	assert(invariant());
}

void Player :: update (const World& world)
{
	Vector3 old_position = m_position;

	moveAccordingToVelocity();

	bool        is_over_disk    = world.isCylinderOnAnyDisk(m_position, Player::RADIUS);
	const Disk& closest_disk    = world.getClosestDisk     (m_position);
	float       height_for_disk = closest_disk.getHeight   (m_position) + Player::HALF_HEIGHT;

	if(m_is_jumping)
	{
		applyGravity();

		if(is_over_disk && m_position.y < height_for_disk)
		{
			if(world.isCylinderOnAnyDisk(old_position, Player::RADIUS))
			{
				// land on disk
				m_position.y = height_for_disk;
				m_is_jumping = false;
			}
			else
			{
				// hit disk side, fall down outside
				m_position.x = old_position.x;
				m_position.z = old_position.z;
				m_velocity.x = 0.0;
				m_velocity.z = 0.0;
			}
		}

		if(isDead())
		{
			m_velocity.setZero();
			m_position.y = DEAD_Y;
		}
	}
	else
	{
		if(is_over_disk)
		{
			m_position.y = height_for_disk;
			applyFriction(DiskType::getFriction(closest_disk.getDiskType()));
			slide(world);
			decreaseRunCounter();
		}
		else
		{
			m_is_jumping = true;
			m_run_counter = 0;
		}
	}

	assert(invariant());
}

void Player :: accelerateForward (float acceleration)
{
	assert(!isJumping());

	float delta_velocity = acceleration * PHYSICS_FRAME_LENGTH;
	m_velocity += m_forward * delta_velocity;

	assert(invariant());
}

void Player :: accelerateLeft (float acceleration)
{
	assert(!isJumping());

	float delta_velocity = acceleration * PHYSICS_FRAME_LENGTH;
	m_velocity += m_forward.getRotatedY(HALF_PI) * delta_velocity;

	assert(invariant());
}

void Player :: turnLeft (float radians_per_second)
{
	m_forward.rotateY(radians_per_second * PHYSICS_FRAME_LENGTH);

	assert(invariant());
}

void Player :: doJump ()
{
	assert(!isJumping());

	m_velocity    = m_forward * JUMP_HORIZONTAL +
	                UP_VECTOR * JUMP_VERTICAL;
	m_is_jumping  = true;
	m_run_counter = 0;

	assert(invariant());
}

void Player :: markRunning (bool is_forward)
{
	m_run_counter = RUN_COUNTER_MAX;

	if(is_forward)
		m_run_frame += RUN_FRAMES_FORWARD_PER_SECOND  * PHYSICS_FRAME_LENGTH;
	else
		m_run_frame -= RUN_FRAMES_BACKWARD_PER_SECOND * PHYSICS_FRAME_LENGTH;

	assert(invariant());
}



void Player :: moveAccordingToVelocity ()
{
	m_position += m_velocity * PHYSICS_FRAME_LENGTH;

	assert(invariant());
}

void Player :: decreaseRunCounter ()
{
	if(m_run_counter > 0)
		m_run_counter--;

	assert(invariant());
}

void Player :: applyFriction (float friction_per_second)
{
	float friction_per_frame = pow(friction_per_second, PHYSICS_FRAME_LENGTH);
	m_velocity *= friction_per_frame;

	assert(invariant());
}

void Player :: applyGravity ()
{
	m_velocity += GRAVITY * PHYSICS_FRAME_LENGTH;

	// with a terminal velocity, we would approach a maximum speed asymptotically

	assert(invariant());
}

void Player :: slide (const World& world)
{
	assert(!isJumping());

	static const unsigned int SLIP_POINTS = 60;
	static const float        NO_LOWEST = 1.0e20f;
	static const float        TEST_DISTANCE = 0.01f;
	static const float        ACCELERATION_FACTOR = 10.0f;

	const Disk& closest_disk  = world.getClosestDisk(m_position);
	float       player_height = closest_disk.getHeight(m_position);

	Vector3 lowest_direction;
	float   lowest_height = NO_LOWEST;
	for(unsigned int i = 0; i < SLIP_POINTS; i++)
	{
		double  radians   = i * TWO_PI / SLIP_POINTS;
		Vector3 direction = Vector3::UNIT_X_PLUS.getRotatedY(radians);
		float   height    = closest_disk.getHeight(m_position + direction * TEST_DISTANCE);

		if(height < lowest_height)
		{
			lowest_direction = direction;
			lowest_height    = height;
		}
	}
	assert(lowest_height != NO_LOWEST);

	assert(TEST_DISTANCE > 0.0f);
	float slope = (player_height - lowest_height) / TEST_DISTANCE;
	float slope_slide = DiskType::getSlopeSlide(closest_disk.getDiskType());
	if(slope > slope_slide)
	{
		float acceleration_magnitude = (slope - slope_slide) * ACCELERATION_FACTOR;

		assert(lowest_direction.isNormal());
		assert(acceleration_magnitude >= 0.0f);
		m_velocity += lowest_direction * acceleration_magnitude * PHYSICS_FRAME_LENGTH;
	}
}

bool Player :: invariant () const
{
	if(m_forward.y != 0.0) return false;
	if(!m_forward.isNormal()) return false;
	return true;
}
