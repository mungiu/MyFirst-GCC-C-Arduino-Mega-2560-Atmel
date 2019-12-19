/*
 * Co2Sensor.h
 *
 * Created: 06-12-2019 18:13:55
 *  Author: peJep
 */ 


#ifndef CO2SENSOR_H_
#define CO2SENSOR_H_

void getCo2SensorMeasurement();
void my_co2_call_back(uint16_t ppm);
void createCo2Task();


#endif /* CO2SENSOR_H_ */