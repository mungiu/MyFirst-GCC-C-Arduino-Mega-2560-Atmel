/**
 * \file
 * \authors Andrei Mungiu, Jakub Duchon, Jeppe Graasbøll Jensen, Maria Louisa Failli
 * \version 1.0.0
 *
 * \defgroup co2_controller Controller for the co2 sensor
 * \brief This file contains methods responsible for getting measurements from the sensor and saving them in a struct
 */

#ifndef CO2SENSOR_H_
#define CO2SENSOR_H_

/**
 * \ingroup co2_controller
 * \brief A method responsible for getting measurements from the sensor
 *
 * Reads a measurement from the co2 sensor mh_z19. The driver must be initialized before the method call.
 *
 */
void getCo2SensorMeasurement();

/**
 * \ingroup co2_controller
 * \brief A callback function responsible for saving measurements to a struct
 *
 * \param[in] pointer to measurement data
 */
void my_co2_call_back(uint16_t ppm);

#endif /* CO2SENSOR_H_ */