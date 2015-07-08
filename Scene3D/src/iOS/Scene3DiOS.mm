#include "Scene3DiOS.h"

#import <UIKit/UIKit.h>

void Scene3D_initializeFileSystem()
{
    print("initializeFileSystem()");

    fs = io::createFileSystem();

    NSBundle* Bundle = [NSBundle mainBundle];
    NSString* BundlePath = [Bundle bundlePath];
    
    core::stringc NewPath = [BundlePath cStringUsingEncoding:NSASCIIStringEncoding];
    
    fs->changeWorkingDirectoryTo(NewPath);
}