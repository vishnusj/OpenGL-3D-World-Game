//
//  DiskType.h
//
//  A module to store the different types of disk.
//

#ifndef DISK_TYPE_H
#define DISK_TYPE_H

#include <cassert>
#include <string>



//
//  DiskType
//
//  A namespace containing constants for the different disk types.
//
namespace DiskType
{
	//
	//  COUNT
	//
	//  The number of disk types.
	//
	const unsigned int COUNT = 5;

	//
	//  RED_ROCK
	//
	//  The smallest type of disk, made of red stone.  It has no
	//    special properties.
	//
	const unsigned int RED_ROCK = 0;

	//
	//  LEAFY
	//
	//  A leafy disk, resembling of a tree.  It is slow to move on.
	//
	const unsigned int LEAFY = 1;

	//
	//  ICY
	//
	//  An icy disk.  It is slippery.
	//
	const unsigned int ICY = 2;

	//
	//  SANDY
	//
	//  A sandy disk.  It is less slippery than the icy disks.
	//
	const unsigned int SANDY = 3;

	//
	//  GREY_ROCK
	//
	//  The largest type of disk, made of grey stone.  It has no
	//    special properties.
	//
	const unsigned int GREY_ROCK = 4;



	//
	//  getModelName
	//
	//  Purpose: To determine the model used to display a disk
	//           the specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The name of the model file for a disk of type
	//           disk_type.
	//  Side Effect: N/A
	//
	inline const std::string& getModelName (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const std::string A_MODEL[COUNT] =
		{
			"Models/DiskA.obj",	//  RED_ROCK
			"Models/DiskB.obj",	//  LEAFY
			"Models/DiskC.obj",	//  ICY
			"Models/DiskD.obj",	//  SANDY
			"Models/DiskE.obj",	//  GREY_ROCK
		};

		return A_MODEL[disk_type];
	}

	//
	//  getSideLength
	//
	//  Purpose: To determine how many cells across the
	//           heightmap on the top of a disk of the specified
	//           type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The number of cells in th heightmap for a disk
	//           of type disk_type.
	//  Side Effect: N/A
	//
	inline unsigned int getSideLength (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const unsigned int A_SIDE_LENGTH[COUNT] =
		{
			16,	//  RED_ROCK
			32,	//  LEAFY
			48,	//  ICY
			64,	//  SANDY
			80,	//  GREY_ROCK
		};

		return A_SIDE_LENGTH[disk_type];
	}

	//
	//  getTexureRepeatCount
	//
	//  Purpose: To determine how many times the texture is
	//           repeated across the top of a disk of the
	//           specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The name of the material for the top of a disk
	//           of type disk_type.
	//  Side Effect: N/A
	//
	inline unsigned int getTexureRepeatCount (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const unsigned int A_REPEAT_COUNT[COUNT] =
		{
			1,	//  RED_ROCK
			2,	//  LEAFY
			3,	//  ICY
			4,	//  SANDY
			5,	//  GREY_ROCK
		};

		return A_REPEAT_COUNT[disk_type];
	}

	//
	//  getTextureName
	//
	//  Purpose: To determine the texture used to display the
	//           top of a disk the specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The name of the texture for the top of a disk
	//           of type disk_type.
	//  Side Effect: N/A
	//
	inline const std::string& getTextureName (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const std::string A_TEXTURE[COUNT] =
		{
			"Models/DiskA.bmp",	//  RED_ROCK
			"Models/DiskB.bmp",	//  LEAFY
			"Models/DiskC.bmp",	//  ICY
			"Models/DiskD.bmp",	//  SANDY
			"Models/DiskE.bmp",	//  GREY_ROCK
		};

		return A_TEXTURE[disk_type];
	}

	//
	//  getRodPoints
	//
	//  Purpose: To determine the number of points for a rod on
	//           a disk of the specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The points for the rod.
	//  Side Effect: N/A
	//
	inline unsigned int getRodPoints (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const unsigned int A_POINTS[COUNT] =
		{
			1,	//  RED_ROCK
			2,	//  LEAFY
			3,	//  ICY
			4,	//  SANDY
			5,	//  GREY_ROCK
		};

		return A_POINTS[disk_type];
	}

	//
	//  getRingSpeedFactor
	//
	//  Purpose: To determine the speed factor for rings moving
	//           across a disk of the specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The speed factor for the disk.
	//  Side Effect: N/A
	//
	inline float getRingSpeedFactor (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const float A_SPEED_FACTOR[COUNT] =
		{
			1.0f,	//  RED_ROCK
			0.5f,	//  LEAFY
			0.25f,	//  ICY
			0.75f,	//  SANDY
			1.0f,	//  GREY_ROCK
		};

		return A_SPEED_FACTOR[disk_type];
	}

	//
	//  getFriction
	//
	//  Purpose: To determine the strength of friction for a
	//           disk of the specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The strength of friction, expressed as the
	//           fraction of the velocity remaining after 1
	//           second.  This value is always in the range
	//           (0.0, 1.0].
	//  Side Effect: N/A
	//
	inline float getFriction (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const float A_FRICTION[COUNT] =
		{
			0.005f,	//  RED_ROCK
			0.0001f,//  LEAFY
			0.5f,	//  ICY
			0.2f,	//  SANDY
			0.005f,	//  GREY_ROCK
		};

		return A_FRICTION[disk_type];
	}

	//
	//  getAccelerationFactor
	//
	//  Purpose: To determine the strength of acceleration for
	//           the player on a disk of the specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The strength of acceleration, expressed as the
	//           fraction of the maximum acceleration.  This
	//           value is always in the range (0.0, 1.0].
	//  Side Effect: N/A
	//
	inline float getAccelerationFactor (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const float A_FACTOR[COUNT] =
		{
			1.0f,	//  RED_ROCK
			0.5f,	//  LEAFY
			0.25f,	//  ICY
			0.25f,	//  SANDY
			1.0f,	//  GREY_ROCK
		};

		return A_FACTOR[disk_type];
	}

	//
	//  getSlopeSlide
	//
	//  Purpose: To determine the steepness of the slope at
	//           which the player begins to slide downhill on a
	//           disk of the specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of disk
	//  Precondition(s):
	//    <1> disk_type < COUNT
	//  Returns: The steepness of the slope, expressed as rise
	//           over run.
	//  Side Effect: N/A
	//
	inline float getSlopeSlide (unsigned int disk_type)
	{
		assert(disk_type < COUNT);

		static const float A_SLOPE[COUNT] =
		{
			0.4f,	//  RED_ROCK
			1.0f,	//  LEAFY
			0.2f,	//  ICY
			0.2f,	//  SANDY
			0.4f,	//  GREY_ROCK
		};

		return A_SLOPE[disk_type];
	}



};  // end of namespace DiskType

#endif
