// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta
#ifndef _IFACERECONSTRUCTIONGUIDE_H_
#define _IFACERECONSTRUCTIONGUIDE_H_

namespace metaio
{

class IMetaioSDK;
class TrackingValues;
class Vector2d;

/** Public interface for face reconstruction guide.
 *  Displays graphics to guide a user through the process of face reconstruction.
 */
class IFaceReconstructionGuide
{
public:
	/** Construct a Face Reconstruction Guide with a given instance of the Metaio SDK.
	 *  \param metaioSDK Pointer to an instance of the Metaio SDK. Ownership of the 
	 *         pointer is retained by the caller.
	 *  \return Pointer to a dynamically allocated instance of a face reconstruction guide.
	 *          The caller is responsible for deleting the pointer.
	 */
	static IFaceReconstructionGuide* createFaceReconstructionGuide(IMetaioSDK* metaioSDK);

	/** Default destructor.
	 */
	virtual ~IFaceReconstructionGuide() {};

	/** Update reconstruction graphics (if applicable) using the provided tracking values
	 *  and check for completion of reconstruction.
	 *  \param trackingValues Tracking values from the sensor for the current frame
	 *  \return true if face reconstruction is complete, otherwise false
	 */
	virtual bool updateAndCheckComplete(const TrackingValues &trackingValues) = 0;

	virtual void setEnabled(bool enable) = 0;
};

}

#endif