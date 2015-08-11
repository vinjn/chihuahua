// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the irrEdit editor
// For conditions of distribution and use, see copyright notice in irredit.h

#ifndef __IRR_SCRIPT_ENGINE_H_INCLUDED__
#define __IRR_SCRIPT_ENGINE_H_INCLUDED__

#include "IReferenceCounted.h"
#include "vector3d.h"

namespace irr
{
	class IrrlichtDevice;

namespace io
{
	class IAttributes;
	class IReadFile;
}
namespace scene
{
	class ISceneManager;
}
namespace script
{
	typedef void* ScriptFunctionParameterObject;
	typedef long (*ScriptFunction)(ScriptFunctionParameterObject);

//! Interface wrapping the integrated script engine
class IScriptEngine : public IReferenceCounted
{
public:

	//! Sets the scene manager the scripting environment should operate on
	virtual void setSceneManager(scene::ISceneManager* sceneManager) = 0;

	//! Returns the scene manager the scripting environment is operating on.
	/** You can set this scene manager using setSceneManager. Note that if you set
	it to 0, this function will return the main scene manager of the irrlicht device. */
	virtual scene::ISceneManager* getSceneManager() = 0;

	//! Returns the IrrlichtDevice associated with this script engine
	virtual IrrlichtDevice* getIrrlichtDevice() = 0;

	//! Executes some scripting code. 
	/** Returns true if successful. Possible errors, warnings and results will be 
	print out to the log */
	virtual bool executeCode(const char* code) = 0;

	//! Executes some scripting code. 
	/** Returns true if successful. Possible errors, warnings and results will be 
	print out to the log */
	virtual bool executeCodeFromFile(const char* fileName) = 0;

	//! Executes some scripting code. 
	/** Returns true if successful. Possible errors, warnings and results will be 
	print out to the log */
	virtual bool executeCodeFromFile(io::IReadFile* file) = 0;

	//! Executes a global function
	/** Returns true if successful. Possible errors, warnings and results will be 
	print out to the log */
	virtual bool executeFunction(const char* functionName) = 0;

	//! Adds a global function to the script engine
	/** Use a static method or a C-style function which returns long and takes 
	a single parameter of type irr::script::ScriptFunctionParameterObject for this, as 
	described by the ScriptFunction typedef. When called, use the createFunctionParameters()
	method to retrieve the parameters and other used when the function was called. */
	virtual bool addGlobalFunction(ScriptFunction function, const char* functionName) = 0;

	//! Returns function parameters used to call a function.
	/** When an external function was registered using addGlobalFunction() and is called
	by the script engine you can use this method to retrieve the parameters used to call
	the function. 
	\return: Returns a list of the parameters. Call IUnknown::drop() after the list
	is no longer needed. */
	virtual io::IAttributes* createParameterListFromScriptObject(ScriptFunctionParameterObject obj) = 0;

	//! Makes a function return a user data pointer.
	/** To be called in a function called by the scripting engine. If you return a value
	using this, you need to return 1 in your function too. */
	virtual void setReturnValue(void* returnValue) = 0;

	//! Makes a function return a float value.
	/** To be called in a function called by the scripting engine. If you return a value
	using this, you need to return 1 in your function too. */
	virtual void setReturnValue(float returnValue) = 0;

	//! Makes a function return a string.
	/** To be called in a function called by the scripting engine. If you return a value
	using this, you need to return 1 in your function too. */
	virtual void setReturnValue(const char* returnValue) = 0;

	//! Makes a function return a bool.
	/** To be called in a function called by the scripting engine. If you return a value
	using this, you need to return 1 in your function too. */
	virtual void setReturnValue(bool returnValue) = 0;

	//! Makes a function return an int.
	/** To be called in a function called by the scripting engine. If you return a value
	using this, you need to return 1 in your function too. */
	virtual void setReturnValue(int returnValue) = 0;

	//! Makes a function return an 3d vector.
	/** To be called in a function called by the scripting engine. If you return a value
	using this, you need to return 1 in your function too. */
	virtual void setReturnValue(core::vector3df returnValue) = 0;
};
	
} // end namespace
} // end namespace

#endif