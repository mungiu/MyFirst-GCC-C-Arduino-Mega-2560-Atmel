/**
 * \file
 * \authors Andrei Mungiu, Jakub Duchon, Jeppe Graasbøll Jensen, Maria Louisa Failli
 * \version 1.0.0
 *
 * \defgroup lightsensor_controller Controller for the light sensor
 * \brief This file contains function responsible for getting measurements from the sensor and saving them in a struct
 */

#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

/**
 * \ingroup lightsensor_controller
 * \brief A function responsible for getting measurements from the sensor
 *
 * Reads a measurement from the light sensor tsl2591. The driver must be initialized before the method call.
 *
 */

void getLightSensorMeasurement();
<<<<<<< HEAD

/**
 * \ingroup lightsensor_controller
 * \brief A callback function responsible for saving measurements to a struct
 *
 * \param[in] rc return code
 */
void tsl2591Callback(tsl2591ReturnCode_t rc);
=======
void tsl2591Callback(tsl2591ReturnCode_t rc/*, tsl2591_data lightMeasurement*/);
void createLightTask();
>>>>>>> master



#endif /* LIGHTSENSOR_H_ */