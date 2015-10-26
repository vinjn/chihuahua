IrrOculusVR
===========

Oculus Rift SDK 0.4.3 support for Irrlicht 1.8.1 using DirectX 9 and OpenGL (Windows). 

Supports both Oculus Rift DK1 and DK2. 

For DK2 only the extended mode is supported for now which means that the application should be launched in fullscreen to the secondary adapter. This can be done in Irrlicht's creation parameters.

The renderer can either take player's position and rotation around Y-axis as a parameter during
rendering or alternatively it's possible to link to player's head node. The linking is a good
solution for example for simulators while the other approach works nicely for FPS games.

main.cpp contains simple example using the FPS-approach.


Feel free to contact me about this through Twitter:
@Suvidriel

You can get the Oculus Rift SDK here:
https://developer.oculusvr.com/?action=dl
