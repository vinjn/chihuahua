// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta
#ifndef __AS_SENSORSCOMPONENTWIN32_H__
#define __AS_SENSORSCOMPONENTWIN32_H__

#include "STLCompatibility.h"
#include "ISensorsComponent.h"

namespace metaio
{

/**
 * Dummy sensors component that provides static sensor values.
 *
 * \anchor ISensorsComponentDummy
 */
class SensorsComponentDummy: public ISensorsComponent
{
public:

	/** Default constructor.
	 */
	SensorsComponentDummy();

	/** Destructor.
	 */
	virtual ~SensorsComponentDummy();

	virtual int start(int sensors);
	virtual int stop(int sensors = SENSOR_ALL);
	virtual void setManualLocation(const metaio::LLACoordinate& location);
	virtual void resetManualLocation();
	virtual LLACoordinate getLocation() const;
	virtual Vector3d getGravity() const;
	virtual float getHeading() const;

	/** Returns all sensor measurements at once.
	* \returns Sensor values (lla, gravity, accelerometer ...) currently stored in the
	*  sensor component.
	*/
	virtual SensorValues getSensorValues();

	/** Sets all sensor measurements at once.
	* \param values Sensor values (lla, gravity, accelerometer ...) to be stored in the
	*  sensor component.
	*/
	void setSensorValues(const SensorValues& values);

	/** Sets a (simulated) location
	* \param coordinate LLA Coordinate
	*/
	void setLocation(const LLACoordinate& coordinate);

	/**  Sets a (simulated) gravity vector
	* \param vector the gravity vector
	*/
	void setGravity(const Vector3d& vector);

	/** Sets a simulated device heading (compass direction)
	* \param heading The compass direction
	*/
	void setHeading(float heading);

	/** Set attitude of the device.
	* \param attitude The attitude of the device (origin is unclear)
	*/
	void setAttitude(const metaio::Rotation& attitude);

	/** Set the gyroscope measurements.
	* \param readings The relative measurements in rad/s.
	*/
	void setHistoricalGyroscopeVector(const stlcompat::Vector<SensorReading> &readings);

	/** Set the accelerometer measurements.
	* \param readings The relative measurements in m/s^2.
	*/
	void setHistoricalAccelerometerVector(const stlcompat::Vector<SensorReading> &readings);
	
	/** Set the magnetometer measurements.
	* \param readings The relative measurements in microTesla.
	*/
	void setHistoricalMagnetometerVector(const stlcompat::Vector<SensorReading> &readings);
private:

	SensorValues m_sensorValues;
	LLACoordinate m_cachedLocation;
	int m_activeSensors;
};

}

#endif
