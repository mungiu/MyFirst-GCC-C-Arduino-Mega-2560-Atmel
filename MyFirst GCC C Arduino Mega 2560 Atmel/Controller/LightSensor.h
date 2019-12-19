/*
 * LightSensor.h
 *
 * Created: 06-12-2019 18:14:49
 *  Author: peJep
 */ 


#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

void getLightSensorMeasurement();
void tsl2591Callback(tsl2591ReturnCode_t rc/*, tsl2591_data lightMeasurement*/);
void createLightTask();



#endif /* LIGHTSENSOR_H_ */