// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta

#ifndef __GEOMETRY_HIT_H__
#define __GEOMETRY_HIT_H__

#include "../Common/Dll.h"
#include "../Common/Vector3d.h"

namespace metaio 
{

class IGeometry;
	
/// Defines geometry and coordinates of a picked geometry (used e.g. by getAllGeometriesFromViewportCoordinates)
struct METAIO_DLL_API GeometryHit
{
	/// Picked geometry
	IGeometry*				geometry;
	
	/**
	 * Position in coordinates of the geometry's COS where the ray hit the geometry
	 *
	 * Only valid if you set useTriangleTest parameter to true when calling
	 * getAllGeometriesFromViewportCoordinates. Otherwise this value is undefined.
	 */
	Vector3d				cosCoordinates;
	
	/**
	 * Position in object coordinates where the ray hit the geometry
	 *
	 * Only valid if you set useTriangleTest parameter to true when calling
	 * getAllGeometriesFromViewportCoordinates. Otherwise this value is undefined.
	 */
	Vector3d				objectCoordinates;

	/**
	 * Normalized normal of the intersected triangle on the geometry
	 *
	 * Only valid if you set useTriangleTest parameter to true when calling
	 * getAllGeometriesFromViewportCoordinates. Otherwise this value is undefined.
	 */
	Vector3d				normal;
	
	/// Constructor
	GeometryHit();
};
	
} //namespace metaio

#endif //__GEOMETRY_HIT_H__
