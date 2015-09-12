// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta
#ifndef _DLL_H_INCLUDED_
#define _DLL_H_INCLUDED_

#if defined(METAIO_STATIC_LIB)
	#define METAIO_DLL_API	// we don't have a dll file
#elif defined(METAIO_DLL_EXPORT)
	#if defined(_MSC_VER)	// compiling on Visual Studio
		#define METAIO_DLL_API __declspec(dllexport)
	#else
		#define METAIO_DLL_API __attribute__ ((visibility("default")))			
	#endif
#else
	#if defined(_MSC_VER)	// compiling on Visual Studio
		#define METAIO_DLL_API __declspec(dllimport)
	#else
		#define METAIO_DLL_API
	#endif
#endif

#endif