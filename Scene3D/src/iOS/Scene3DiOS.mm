#include "Scene3DiOS.h"

#import <UIKit/UIKit.h>

void Scene3D_initializeFileSystem()
{
    os::Printer::Logger = new CLogger(NULL);
	
    print("initializeFileSystem()");

    fs = io::createFileSystem();

    NSBundle* Bundle = [NSBundle mainBundle];
    NSString* BundlePath = [Bundle bundlePath];
    
    core::stringc NewPath = [BundlePath cStringUsingEncoding:NSASCIIStringEncoding];
    
    fs->changeWorkingDirectoryTo(NewPath);
}

void Scene3D_setNodeTexture(long nodePtr, long texturePtr)
{
    Scene3D_setNodeTextureAtLayer(nodePtr, 0, texturePtr);
}