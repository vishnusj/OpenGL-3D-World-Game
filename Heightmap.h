//
//  Heightmap.h
//
//  A module to represent the heightmap on top of a disk.
//

#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <cassert>
#include <vector>

#include "ObjLibrary/DisplayList.h"



//
//  Heightmap
//
//  A class to represent the heightmap for 1 disk.  A Heightmap
//    is always sqaure, and has a side length indicating how
//    many cells are along each side.  The heightmap has a
//    number of vertexes along each side one more than the
//    number of cells.
//
//      0   1   2   3   0
//    0 +---+---+---+---+ 0  \   side length 4
//      | / | / | / | / |    |   4 * 4 = 16 cells total
//    3 +---+---+---+---+ 3  |
//      | / | / | / | / |    |   5 vertexes per side
//    2 +---+---+---+---+ 2   >  5 * 5 = 25 vertexes total
//      | / | / | / | / |    |
//    1 +---+---+---+---+ 1  |   4 vertexes represented per side
//      | / | / | / | / |    |   4 * 4 = 16 represented vertexes
//    0 +---+---+---+---+ 0  /   25 - 16 = 9 duplicate vertexes
//      0   1   2   3   0
//
//  Each vertex has a height.  For edge vertexes, this height
//    must always be 0.0f.  Other vertexes may have any height.
//    For mathematical reasons, only one vertex is stored for
//    the edge in each direction, and the value is used for both
//    sides of the heightmap.  As a consequence, a hightmap has
//    the same number of vertexes as cells in each direction.
//    The "extra" vertex is numbered 0.
//
//  A heightmap must be finalized before it is used.  When a
//    Heightmap is initialized, the 3D mesh for its surface is
//    constructed and the vertex heights become immutable.  If
//    necessary, the Heightmap can be un-finalized, restoring it
//    to its mutable form.
//
//  When a Heightmap is drawn, occupies the space from [0, 1]
//    along the x and z axes.  The client code should apply the
//    appropriate scaling to the horizontal size of the
//    heightmap.  No scaling should be applied to the vertical
//    (Y) dimension.
//
//  Class Invariant:
//    <1> m_side_length >= SIDE_LENGTH_MIN
//    <2> m_texture_repeat >= SIDE_LENGTH_MIN
//    <3> mv_heights.size() == m_side_length * m_side_length
//    <4> !m_mesh.isPartial()
//    <5> mv_heights[getVertexIndex(i, 0)] == HEIGHT_EDGE
//                                  WHERE 0 <= i < m_side_length
//    <6> mv_heights[getVertexIndex(0, j)] == HEIGHT_EDGE
//                                  WHERE 0 <= j < m_side_length
//
class Heightmap
{
public:
	//
	//  SIDE_LENGTH_MIN
	//
	//  The minimum side length for a Heightmap.
	//
	static const unsigned int SIDE_LENGTH_MIN = 1;

	//
	//  TEXTURE_REPEAT_MIN
	//
	//  The minimum number of times the texture repeats across a
	//    Heightmap.
	//
	static const unsigned int TEXTURE_REPEAT_MIN = 1;

	//
	//  HEIGHT_EDGE
	//
	//  The height of the edge vertexes.
	//
	static const float HEIGHT_EDGE;

public:
	//
	//  Default Constructor
	//
	//  Purpose: To create a new, default Heightmap without
	//           finalizing it.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new Heightmap is created with a side
	//               length of SIDE_LENGTH_MIN and a texture
	//               repeat of TEXTURE_REPEAT_MIN.  All vertexes
	//               are set to have a height of HEIGHT_EDGE.
	//               The new Heightmap is not finalized.
	//
	Heightmap ();

	//
	//  Initializing Constructor
	//
	//  Purpose: To create a new Heightmap for a disk of the
	//           specified type.
	//  Parameter(s):
	//    <1> disk_type: The type of the disk
	//    <2> is_finalize: Whther to finalize the Heightmap
	//  Precondition(s):
	//    <1> disk_type < DiskType::COUNT
	//    <2> DiskType::getSideLength(disk_type) >=
	//                                           SIDE_LENGTH_MIN
	//    <3> DiskType::getTexureRepeatCount(disk_type) >=
	//                                        TEXTURE_REPEAT_MIN
	//  Returns: N/A
	//  Side Effect: A new Heightmap is created for a disk of type
	//               disk_type.  is_finalize == true, the new
	//               Heightmap is finalized.
	//
	Heightmap (unsigned int disk_type,
	           bool is_finalize);

	Heightmap (const Heightmap& original) = default;
	~Heightmap () = default;
	Heightmap& operator= (const Heightmap& original) = default;

	//
	//  isFinalized
	//
	//  Purpose: To determine if this Heightmap has been finalized.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether this Heightmap has been finalized.
	//  Side Effect: N/A
	//
	bool isFinalized () const;

	//
	//  getSideLength
	//
	//  Purpose: To determine the side length for this Heightmap.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The side length.
	//  Side Effect: N/A
	//
	unsigned int getSideLength () const;

	//
	//  getTextureRepeat
	//
	//  Purpose: To determine the number of times the texture
	//           repeats across this Heightmap.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: The number of times the texture repeats.
	//  Side Effect: N/A
	//
	unsigned int getTextureRepeat () const;

	//
	//  getVertexHeight
	//
	//  Purpose: To determine the height of the specified vertex
	//           in this Heightmap.
	//  Parameter(s):
	//    <1> i
	//    <2> j: The coordinates of the vertex
	//  Precondition(s):
	//    <1> i < getSideLength()
	//    <2> j < getSideLength()
	//  Returns: The height of the vertex with coordinates
	//           (i, j).
	//  Side Effect: N/A
	//
	float getVertexHeight (unsigned int i,
	                       unsigned int j) const;

	//
	//  getVertexHeightTolerant
	//
	//  Purpose: To determine the height of the specified vertex
	//           in this Heightmap.
	//  Parameter(s):
	//    <1> i
	//    <2> j: The coordinates of the vertex
	//  Precondition(s): N/A
	//  Returns: The height of the vertex with coordinates
	//           (i, j).  If there is no such vertex in the
	//           heightmap, HEIGHT_EDGE is returned.
	//  Side Effect: N/A
	//
	float getVertexHeightTolerant (int i, int j) const;

	//
	//  getHeight
	//
	//  Purpose: To determine the height of this Heightmap at
	//           the specified position.
	//  Parameter(s):
	//    <1> i
	//    <2> j: The coordinates of the vertex
	//  Precondition(s): N/A
	//  Returns: The height of this Hrightmap at position
	//           (i, j).  If position (i, j) is outside this
	//           Heightmap, HEIGHT_EDGE is returned.
	//  Side Effect: N/A
	//
	float getHeight (float i, float j) const;

	//
	//  draw
	//
	//  Purpose: To display this Heightmap.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isFinalized()
	//  Returns: N/A
	//  Side Effect: This Heightmap is displayed.  It is scaled
	//               to cover the space from [0, 1] alomg the x-
	//               and z-axes, regardless of the side length.
	//
	void draw () const;

	//
	//  setVertexHeight
	//
	//  Purpose: To change the height of the specified vertex in
	//           this Heightmap.
	//  Parameter(s):
	//    <1> i
	//    <2> j: The coordinates of the vertex
	//    <3> height: The new vertex height
	//  Precondition(s):
	//    <1> !isFinalized()
	//    <2> i < getSideLength()
	//    <3> j < getSideLength()
	//    <4> (i >= 1 && j >= 1) || height == HEIGHT_EDGE
	//  Returns: N/A
	//  Side Effect: The height of the vertex with coordinates
	//               (i, j) is set to height.
	//
	void setVertexHeight (unsigned int i,
	                      unsigned int j,
	                      float height);

	//
	//  finalize
	//
	//  Purpose: To finalize this Heightmap.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> !isFinalized()
	//  Returns: N/A
	//  Side Effect: The 3d geometry for this Heightmap is
	//               constructed.
	//
	void finalize ();

	//
	//  unfinalize
	//
	//  Purpose: To make this Heightmap no longer finalized.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> isFinalized()
	//  Returns: N/A
	//  Side Effect: The 3d geometry for this Heightmap is
	//               destroyed.
	//
	void unfinalize ();

private:
	//
	//  Helper Function: getVertexIndex
	//
	//  Purpose: To determine the vector index for the specified
	//           vertex.
	//  Parameter(s):
	//    <1> i
	//    <2> j: The coordinates of the vertex
	//  Precondition(s):
	//    <1> i < getSideLength()
	//    <2> j < getSideLength()
	//  Returns: The vector index of the vertex with coordinates
	//           (i, j).
	//  Side Effect: N/A
	//
	unsigned int getVertexIndex (unsigned int i,
	                             unsigned int j) const;

	//
	//  Helper Function: setHeightsRedRock
	//  Helper Function: setHeightsLeafy
	//  Helper Function: setHeightsIcy
	//  Helper Function: setHeightsSandy
	//  Helper Function: setHeightsGrey
	//
	//  Purpose: To calculate the vertex heights for a disk of
	//           the indicated disk type.
	//  Parameter(s): N/A
	//  Precondition(s):
	//    <1> !isFinalized()
	//  Returns: N/A
	//  Side Effect: The vertex heights are calculated.
	//
	void setHeightsRedRock ();
	void setHeightsLeafy ();
	void setHeightsIcy ();
	void setHeightsSandy ();
	void setHeightsGreyRock ();

	//
	//  Helper Function: getEdgeFactor
	//
	//  Purpose: To determine the multiplier factor for the
	//           perlin noise based on the distance to the
	//           heightmap edge.
	//  Parameter(s):
	//    <1> i
	//    <2> j: The coordinates of the vertex
	//  Precondition(s):
	//    <1> i < getSideLength()
	//    <2> j < getSideLength()
	//  Returns: The hight multiplier for vertex (i, j).
	//  Side Effect: N/A
	//
	float getEdgeFactor (unsigned int i,
	                     unsigned int j) const;

	//
	//  Helper Function: invariant
	//
	//  Purpose: To determine if the class invariant is true.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: Whether the class invariant is true.
	//  Side Effect: N/A
	//
	bool invariant () const;

private:
	unsigned int m_side_length;
	unsigned int m_texture_repeat;
	std::vector<float> mv_heights;
	ObjLibrary::DisplayList m_mesh;
};



#endif
