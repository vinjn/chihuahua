// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta

#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include "Dll.h"
#include "Vector3d.h"

namespace metaio {
	
/**
 * BoundingBox of the geometry.
 */
struct METAIO_DLL_API BoundingBox
{
	Vector3d min;	///< Vector containing the minimum x,y,z values
	Vector3d max;	///< Vector containing the maximum x,y,z values
};
	
} //namespace metaio

#endif //__BOUNDING_BOX_H__
