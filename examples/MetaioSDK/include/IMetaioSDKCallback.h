// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta
#ifndef __AS_METAIOSDK_CALLBACK__
#define __AS_METAIOSDK_CALLBACK__

#include "Common/Dll.h"
#include "Common/ImageStruct.h"
#include "Common/STLCompatibility.h"
#include "Common/TrackingValues.h"
#include "Rendering/RenderEvent.h"

namespace metaio
{
// forward declarations
class IGeometry;
class Path;


/**
 * The metaio SDK Callback interface.
 *
 * These functions should be implemented for handling events triggered by the metaio SDK.
 */
class METAIO_DLL_API IMetaioSDKCallback
{
public:

	/** 
	 * Virtual destructor 
	 */
	virtual ~IMetaioSDKCallback() {};

	/**
	 * This is triggered as soon as the SDK is ready, e.g. splash screen is finished.
	 */
	virtual void onSDKReady() {};

	/**
	 * This is called everytime SDK encounters an error.
	 * See ErrorCodes.h for a list of error codes.
	 *
	 * \param errorCode A code representing type of the error (see ErrorCodes.h)
	 * \param errorDescription Description of the error
	 */
	virtual void onError(const int errorCode, const stlcompat::String& errorDescription) {};

	/**
	 * This is called everytime SDK encounters a warning.
	 * See WarningCodes.h for a list of warning codes.
	 *
	 * \param warningCode Code A code representing type of the warning (see WarningCodes.h)
	 * \param warningDescription Description of the warning
	 */
	virtual void onWarning(const int warningCode, const stlcompat::String& warningDescription) {};

	/**
	 * This function will be triggered, if an animation has ended
	 *
	 * \param geometry the geometry which has finished animating
	 * \param animationName the name of the just finished animation
	 */
	virtual void onAnimationEnd(metaio::IGeometry* geometry, const stlcompat::String& animationName) {};

	/**
	 * This function will be triggered, if an animation/movietexture-playback has ended
	 *
	 * \param geometry the geometry which has finished animating/movie-playback
	 * \param moviePath path to the movie file
	 * \return void
	 */
	virtual void onMovieEnd(metaio::IGeometry* geometry, const metaio::Path& moviePath) {};

	/**
	 * Callback that delivers the next camera image.
	 *
	 * The image will have the dimensions of the current capture resolution.
	 * To request this callback, call requestCameraFrame()
	 *
	 * \param cameraFrame the latest camera image
	 *
	 * \note you must copy the ImageStuct::buffer, if you need it for later.
	 */
	virtual void onNewCameraFrame(metaio::ImageStruct* cameraFrame) {};


	/**
	 * Callback that notifies that camera image has been saved.
	 *
	 * To request this callback, call requestCameraFrame(filepath, width, height)
	 *
	 * \param filePath File path in which image is written, or empty path in case of failure
	 */
	virtual void onCameraImageSaved(const metaio::Path& filePath) {};

	/**
	 * Callback for changes in rendering
	 *
	 * \param renderEvent Describes the render event (e.g. geometry became visible)
	 */
	virtual void onRenderEvent(const RenderEvent& renderEvent) {};

	/**
	 * Callback that delivers screenshot as new ImageStruct.
	 * The ImageStruct and its buffer will be released by the SDK after this method call.
	 * Note: This callback is called on the render thread.
	 * 
	 * \param image Screenshot image
	 * \sa IMetaioSDK::requestScreenshot
	 */
	virtual void onScreenshotImage(metaio::ImageStruct* image) {};

	/**
	 * Callback that notifies that screenshot has been saved to a file.
	 * If the screenshot is not written to a file, the filepath will be
	 * an empty string.
	 * Note: This callback is called on the render thread.
	 * 
	 * \param filePath File path where screenshot image has been written
	 * \sa IMetaioSDK::requestScreenshot
	 */
	virtual void onScreenshotSaved(const metaio::Path& filePath) {};

	/**
	* Callback to inform that tracking state has changed.
	*
	* This callback reports initialized, found and lost states only. To retrieve actual tracking
	* information, use IMetaioSDK::getTrackingValues.
	* Use IMetaioSDK::setTrackingEventCallbackReceivesAllChanges to receive all tracking values,
	* independent of the tracking state.
	*
	* Note that this function is called in rendering thread, thus it would block
	* rendering. It should return as soon as possible without any expensive
	* processing.
	*
	* \param trackingValues current tracking values
	*
	*/
	virtual void onTrackingEvent(const metaio::stlcompat::Vector<metaio::TrackingValues>& trackingValues) {};

	/**
	* Callback to notify the application about an instant tracker event.
	*
	* If "success" is true, "file" will contain a file path you either specified when starting the
	* instant tracking or a temporarily result. Since SDK 6.0, this path is unused for "INSTANT_3D"
	* because the new 3D tracking keeps running when an instant target was found.
	*
	* \param	success		True on success
	* \param	filePath	Path to the tracking configuration (do not use for "INSTANT_3D")
	*/
	virtual void onInstantTrackingEvent(bool success, const metaio::Path& filePath) {};

private:
	/**
	 * NOT SUPPORTED ANYMORE
	 * \param filepath -
	 * \deprecated This signature is not supported anymore, use the one with metaio::Path!
	 */
	virtual void onCameraImageSaved(const stlcompat::String& filepath) METAIOSDK_CPP11_FINAL;

	/**
	 * NOT SUPPORTED ANYMORE
	 * \param success -
	 * \param file -
	 * \deprecated This signature is not supported anymore, use the one with metaio::Path!
	 */
	virtual void onInstantTrackingEvent(bool success, const stlcompat::String& file) METAIOSDK_CPP11_FINAL;

	/**
	 * NOT SUPPORTED ANYMORE
	 * \param geometry -
	 * \param movieName -
	 * \deprecated This signature is not supported anymore, use the one with metaio::Path!
	 */
	virtual void onMovieEnd(metaio::IGeometry* geometry, const stlcompat::String& movieName) METAIOSDK_CPP11_FINAL;

	/**
	 * NOT SUPPORTED ANYMORE
	 * \param filepath -
	 * \deprecated This signature is not supported anymore, use the one with metaio::Path!
	 */
	virtual void onScreenshotSaved(const stlcompat::String& filepath) METAIOSDK_CPP11_FINAL;
};

}

#endif
