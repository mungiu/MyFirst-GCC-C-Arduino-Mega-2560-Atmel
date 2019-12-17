/**
 * \file
 * \authors Andrei Mungiu, Jakub Duchon, Jeppe Graasbøll Jensen, Maria Louisa Failli
 * \version 1.0.0
 *
 * \defgroup temperature_humidity_controller Controller for the temperature and humidity sensor
 * \brief This file contains function responsible for getting measurements from the sensor
 */

#include <stdio.h>

#ifndef TEMPHUMSENSOR_H_
#define TEMPHUMSENSOR_H_

/**
 * \ingroup temperature_humidity_controller
 * \brief A function responsible for getting measurements from the sensor
 *
 * Reads a measurement from the temperature and humidity sensor hih8120. The driver must be initialized before the method call.
 */
void getTemperatureHumiditySensorMeasurement();


#endif /* TEMPHUMSENSOR_H_ */