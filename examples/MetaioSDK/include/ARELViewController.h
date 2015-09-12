// Copyright 2007-2014 metaio GmbH. All rights reserved.
#import <metaioSDK/MetaioSDKViewController.h>
#import <metaioSDK/GestureHandlerIOS.h>
#import <metaioSDK/IARELInterpreterIOS.h>

/** ViewController that allows you to play a local AREL file
 *
 * There are 2 ways to use this ViewController
 * 1.) Set the arelFilePath before presenting the viewcontroller
 * 2.) Subclass the ViewController, overwrite onSDKReady and call loadARELFile.
 */
@interface ARELViewController : MetaioSDKViewController<UIGestureRecognizerDelegate, IARELInterpreterIOSDelegate>
{
    metaio::IARELInterpreterIOS*	m_pARELInterpreter;
    GestureHandlerIOS*				m_pGestureHandlerIOS;
}

// Path to the AREL file. If you set this property, the ViewController will automatically load it
// in the onSDKReady function
@property (strong, nonatomic) NSString*                  arelFilePath;

// The webview that hosts the AREL UI and logic
@property (strong, nonatomic) IBOutlet UIWebView*        arelWebView;

// Progressview that will automatically be displayed when loading an AREL package (if set)
@property (strong, nonatomic) IBOutlet UIProgressView*   progressView;

 
 /** Loads an AREL package.
  * 
  * You can use this function to (re)load an AREL package. 
  * If arelFilePath is not set, this method should be called in onSDKReady
  *
  *
  * \param arelFilePath the path to the AREL xml file or zip package
  */
- (void) loadARELFile: (const metaio::Path&) arelFilePath;


/** Callback when the SDK is ready.
 * 
 * After this method has been called, you can load an AREL file.
 *
 */
 - (void)onSDKReady;

 
 @end
 
