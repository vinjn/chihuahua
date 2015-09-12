// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta
#ifndef IARELOBJECT_H
#define IARELOBJECT_H

#include "Common/LLACoordinate.h"
#include <MetaioSDK/Common/Path.h>
#include "Common/STLCompatibility.h"

namespace metaio
{

    
/** Define a button
 */
struct ObjectButton
{
	stlcompat::String buttonID;		//!< ID of the button
	stlcompat::String buttonName;		//!< Name of the button
	stlcompat::String buttonValue;	//!< Value of the button

	/** constructor
	 */
	ObjectButton():
		buttonID(""),
		buttonName(""),
		buttonValue("")
	{ }
};

/** Define an ObjectPopup
 */
struct ObjectPopup
{
	stlcompat::String description;				//!< Description
	stlcompat::Vector<ObjectButton> buttons;	//!< All the buttons

	/** constructor
	 */
	ObjectPopup():
		description("")
	{ }
};

    
// predefined parameters for a POI
enum ObjectParameter
{
    ObjectParameterAuthor,
    ObjectParameterThumbnailURI,
    ObjectParameterIconURI,
    ObjectParameterPhone,
    ObjectParameterEmail,
    ObjectParameterHomepageURL,
    ObjectParameterEnabledRouting,
    ObjectParameterDate,
    ObjectParameterName,
    ObjectParameterShaderMaterial,
    ObjectParameterParent
};

/**
 * Represents an AREL object that is loaded through AREL
 */
class IARELObject
{
public:

	virtual ~IARELObject() {};

	/**
	 * Clone this object.
	 * The returned object must be deleted by the caller.
	 * \return Pointer to the new IARELObject
	 */
	virtual IARELObject* clone() const = 0;

    /** Returns the ID of the object
	 * \return id
	 */
	virtual stlcompat::String getID() const = 0;
    
	/** Get the local file for a URL
	 * \param url the URL
	 * \return local path of URL if present. otherwise empty string
	 */
	virtual metaio::Path getLocalFileForURL(const stlcompat::String& url) const = 0;
	
	/** returns the thumbnail URL for a POI
	 * \note the URL of the image is sometimes adjusted with a get parameter
	 * \return the URL
	 */
	virtual stlcompat::String getThumbnailURL() const = 0;

	/** Returns the title that should be displayed in the context menu
	 * If no name is present, the method will return description or author.
	 * \return title
	 */
	virtual stlcompat::String getTitle() const = 0;

    
    /** Return the object's location
     * \return the location
     */
    virtual metaio::LLACoordinate getLocation() const = 0;
    
    
	/** Returns the description of the POI
	 * \return description of the poi;
	 */
	virtual stlcompat::String getDescription() const = 0;

	/** Returns the name of the POI
	 * \return name of the poi;
	 */
	virtual stlcompat::String getName() const = 0;

	/** returns true if the POI has an LLA coordinate set
	 * \return true if it has lla values, false otherwise.
	 */
	virtual bool hasLLA() const = 0;

	/** Returns true if routing should be enabled, false otherwise
	 * \return true if enabled, false otherwise
	 */
	virtual bool isRoutingEnabled() const = 0;

	/** Checks if the geometry is visible in Unifeye
	 * It takes into account the maxDistance and minAccuracy values
	 * \return true if visible, false otherwise
	 * \sa setCurrentPosition
	 * \sa setCurrentDistanceLimit
	 */
	virtual  bool isVisible() const = 0;

	/** Checks if visible on Radar
	 * It takes into account the LLA, relative to screen and detection
	 */
	virtual bool isVisibleOnRadar() const = 0;

	/** Checks if visible on Map and List
	 * It takes into account the LLA, relative to screen and detection
	 */
	virtual bool isVisibleOnMapList() const = 0;

	/** returns the distance of the POI in meters based
	 * on the last known position
	 * \return distance of POI in meters
	 */
	virtual float getCurrentDistance() const = 0;


	/** Get the popup of the object
	 * Use this method to display the context menu of the poi
	 */
	virtual const ObjectPopup& getObjectPopup() const = 0;

    /** Query a parameter of the POI based on a predefined parameter value
	 * \param key the parameter to query for
	 * \return the value for the parameter or an empty string if not present
	 */
	virtual stlcompat::String getParameter(const ObjectParameter key) const = 0;
    
	/**
	 * Returns the value of a set AREL parameter.
	 *
	 * \param parameterName of the AREL parameter
	 * \return value of the AREL paramter if it is defined or an empty string if it is not found.
	 * */
	virtual stlcompat::String getARELParameter(const stlcompat::String& parameterName) const = 0;

};
}	// namespace metaio

#endif	// IARELOBJECT_Hs
