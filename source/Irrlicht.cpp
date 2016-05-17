// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"

static const char* const copyright = "Irrlicht Engine (c) 2002-2012 Nikolaus Gebhardt";

#ifdef _IRR_WINDOWS_
	#include <windows.h>
	#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(_WIN32_WCE)
		#include <crtdbg.h>
	#endif // _DEBUG
#endif

#include "irrlicht.h"
#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_
#include "CIrrDeviceWin32.h"
#endif

#ifdef _IRR_COMPILE_WITH_OSX_DEVICE_
#include "MacOSX/CIrrDeviceMacOSX.h"
#endif

#ifdef _IRR_COMPILE_WITH_WINDOWS_CE_DEVICE_
#include "CIrrDeviceWinCE.h"
#endif

#ifdef _IRR_COMPILE_WITH_X11_DEVICE_
#include "CIrrDeviceLinux.h"
#endif

#ifdef _IRR_COMPILE_WITH_IPHONE_DEVICE_
#include "iOS/CIrrDeviceiOS.h"
#endif

#ifdef _IRR_COMPILE_WITH_ANDROID_DEVICE_
// #include "Android/CIrrDeviceAndroid.h"
#endif

#ifdef _IRR_COMPILE_WITH_SDL_DEVICE_
#include "CIrrDeviceSDL.h"
#endif

#ifdef _IRR_COMPILE_WITH_FB_DEVICE_
#include "CIrrDeviceFB.h"
#endif

#ifdef _IRR_COMPILE_WITH_CONSOLE_DEVICE_
#include "CIrrDeviceConsole.h"
#endif

namespace ue
{

namespace core
{
	const matrix4 IdentityMatrix(matrix4::EM4CONST_IDENTITY);
	ue::core::stringc LOCALE_DECIMAL_POINTS(".");
}

namespace video
{
	SMaterial IdentityMaterial;
}

} // end namespace ue


#if defined(_IRR_WINDOWS_API_) && !defined(_IRR_STATIC_LIB_)

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
	// _crtBreakAlloc = 139;

    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(__BORLANDC__) && !defined (_WIN32_WCE) && !defined (_IRR_XBOX_PLATFORM_)
				_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
			#endif
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

#endif // defined(_IRR_WINDOWS_)

