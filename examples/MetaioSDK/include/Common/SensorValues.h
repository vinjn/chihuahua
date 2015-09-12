// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta
#ifndef _AS_SENSORVALUES_H_
#define _AS_SENSORVALUES_H_

#include "LLACoordinate.h"
#include "Rotation.h"
#include "STLCompatibility.h"
#include "Vector3d.h"

namespace metaio
{

/** Sensor reading with timestamp */
struct METAIO_DLL_API SensorReading
{ 
	double timestamp;	///< timestamp (in seconds since system boot)
	int accuracy;		///< accuracy - not yet really used (3 seems 'good'...)
	Vector3d values;	///< Three floating point numbers from a sensor.
	                    ///< The interpretation depends on the sensor used.

	/** Default constructor */
	SensorReading();

	/**
	* Sensor readings should be sorted by timestamp
	* \param rhs second parameter
	* \return  true if rhs is newer
	*/
	bool operator<(SensorReading const& rhs);
};

/** An encapsulation of all the sensors' readings with corresponding time stamps */
struct METAIO_DLL_API SensorValues
{
	LLACoordinate location; ///< Device location. Needed: SENSOR_LOCATION

	Vector3d gravity; ///< Normalized gravity vector. Needed: SENSOR_GRAVITY
	double gravityTimestamp; ///< timestamp (in seconds since system boot)

	float heading; ///< Heading in degrees, 0=North, 90=East, 180=South. Needed: SENSOR_HEADING
	double headingTimestamp; ///< timestamp (in seconds since system boot)

	metaio::Rotation attitude; ///< device attitude based on running sensors. Needed: SENSOR_ATTITUDE
	double attitudeTimestamp; ///< timestamp (in seconds since system boot)

	bool deviceIsMoving; ///< Indicates if device is moving. Needed: SENSOR_DEVICE_MOVEMENT

	stlcompat::Vector<SensorReading> historicalGyroscopeVector;	///< Historical raw gyroscope values [rad/s] with timestamps in [s] Needed: SENSOR_DEVICE_GYROSCOPE
	stlcompat::Vector<SensorReading> historicalAccelerometerVector; ///< Historical raw accelerometer values [m/s^2] with timestamps in [s] Needed: SENSOR_DEVICE_ACCELEROMETER
	stlcompat::Vector<SensorReading> historicalMagnetometerVector; ///< Historical raw magnetometer values [microTesla] with timestamps [s] Needed: SENSOR_DEVICE_MAGNETOMETER

	/** Default constructor */
	SensorValues();

	/** Copy constructor */
	SensorValues(const SensorValues& other);

	/**
 	 * Determine if the object has valid location
	 * \return true if valid, false otherwise
	 */
	bool hasLocation() const;

	/**
 	 * Determine if the object has valid gravity reading
	 * \return true if valid, false otherwise
	 */
	bool hasGravity() const;

	/**
 	 * Determine if the object has valid heading reading
	 * \return true if valid, false otherwise
	 */
	bool hasHeading() const;

	/**
 	 * Determine if the object has valid attitude reading
	 * \return true if valid, false otherwise
	 */
	bool hasAttitude() const;

	/**
	 * Clear all sensor readings.
	 */
	void clear();

	/** Assignment operator */
	SensorValues& operator=(const SensorValues& other);
};
}


#endif // _AS_SENSORVALUES_H_
