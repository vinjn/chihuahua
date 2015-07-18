#include "Scene3DiOS.h"

#import <UIKit/UIKit.h>

namespace Scene3D
{
    void initializeFileSystem()
    {
        os::Printer::Logger = new CLogger(NULL);
        
        print("initializeFileSystem()");
        
        fs = io::createFileSystem();
        
        NSBundle* Bundle = [NSBundle mainBundle];
        NSString* BundlePath = [Bundle bundlePath];
        
        core::stringc NewPath = [BundlePath cStringUsingEncoding:NSASCIIStringEncoding];
        
        fs->changeWorkingDirectoryTo(NewPath);
    }
    
    void setNodeTexture(long nodePtr, long texturePtr)
    {
        setNodeTextureAtLayer(nodePtr, 0, texturePtr);
    }
    
    void setRootNodeVisible(bool visible)
    {
        setNodeVisible(getRootNode(), visible);
    }
}
