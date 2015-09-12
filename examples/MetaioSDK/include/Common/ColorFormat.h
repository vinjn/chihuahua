// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta
#ifndef _AS_COLORFORMAT_H_
#define _AS_COLORFORMAT_H_

namespace metaio
{
namespace common
{

/** Color Formats
*
*   Important note:
*	
*	For consistency and readability the formats were updated.
*	You may have to perform slight modifications of your application code if
*	you make use of color formats.
*
*	ECF_GRAY -> ECF_GRAY8
*
*	ECF_R8G8B8 -> ECF_RGB8	
*	ECF_B8G8R8 -> ECF_BGR8
*
*	ECF_A8R8G8B8 -> ECF_RGBA8
*	ECF_A8B8G8R8 -> ECF_BGRA8
*
*	ECF_YUV420SP -> NV12
*	ECF_YUV420SP -> NV21
*/
enum ECOLOR_FORMAT
{
    ECF_RGB8 = 2,	///< 24 bit per pixel. 8 bit for red, green and blue
    ECF_BGR8 = 3,	///< 24 bit per pixel. 8 bit for red, green and blue
    ECF_RGBA8 = 4,	///< 32 bit per pixel. 8 bit for red, green, blue and alpha
    ECF_BGRA8 = 5,	///< 32 bit per pixel. 8 bit for blue, green, red and alpha
	
    /**
    YUV4:2:2(YCbCr)
    Bits 31-24	Bits 23-16	Bits 15-8	Bits 7-0
    	 V (Cr)	Y [n+1 pixel]	U (Cb)	Y [n pixel]
    */
	ECF_YUY2 = 8,			///< 32 bit per 2 pixels. Layout YUYV
	ECF_YV12 = 10,			///< 12 bit YV12 planar YUV image format
    ECF_GRAY8 = 11,			///< 8 bit gray image format
	ECF_NV21 = 12,			///< YUV420SP with UV order
	ECF_NV12 = 13,			///< YUV420SP with VU order
    ECF_D16 = 100,	        ///< depth format, 2-byte unsigned shorts for depth(usually measure in mm)
	ECF_UV32 = 101,			///< UV Map, 2 floats for each component (U,V coordinates)
	ECF_FLIR8 = 200,		///< FLIR (thermal vision) 1 byte
	ECF_UNKNOWN = 999		///< unknown image format
};

}} // metaio::common
#endif