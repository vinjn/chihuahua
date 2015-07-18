#pragma once

#include "../Scene3D.h"

namespace Scene3D
{
    void initializeFileSystem();
    
    void setNodeTexture(long nodePtr, long texturePtr);
    
    void setRootNodeVisible(bool visible);
}


