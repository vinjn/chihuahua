// Copyright 2007-2014 metaio GmbH. All rights reserved.
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#import <metaioSDK/IMetaioSDKIOS.h>

namespace metaio
{
class IMetaioSDKIOS;
class IGeometry;
class ISensorsComponent;
}

/** The MetaioSDKViewcontroller takes care of the basic setup of a metaioSDK on iOS.
 * 
 * It 
 *  - initializes the renderer
 *  - starts the camera when the view appears
 *  - creates a sensorscomponent
 *  - handles rotation events 
 *  - handles unauthorized camera access
 */
@interface MetaioSDKViewController : GLKViewController<MetaioSDKDelegate>
{
	metaio::IMetaioSDKIOS*			m_pMetaioSDK;               //!< Pointer to the metaio SDK instance
	metaio::ISensorsComponent*		m_pSensorsComponent;        //!< Pointer to our SensorsComponent, which handles the sensor access.
    
    BOOL							m_didResize;                            //!< used to determine whether to call resizeRenderer
	bool							m_mustUpdateFrameAndRenderBufferIDs;    //!< used to determine whether to inform the renderer about new buffers
}

@property (strong, nonatomic) IBOutlet GLKView* glkView;

- (void)updateFrameAndRenderBuffers;

/** Is called everytime the renderer draws a new frame */
- (void)drawFrame;

- (BOOL)shouldEnableMultisampleAntialiasing;

/** This methods will call m_pMetaioSDK->startCamera() automatically when the view appears
 *  You can overwrite this method to adjust the default camera handling
 */
- (void) startDefaultCamera;


/** This method is called when the user disallowed the camera access for your application
 * Overwrite this method to provide custom instructions for the user to allow access
 */
- (void) handleUnauthorizedCameraAccess;


@end

