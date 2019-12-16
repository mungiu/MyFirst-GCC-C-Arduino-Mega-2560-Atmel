/*
 * loraWAN.h
 *
 * Created: 12/4/2019 1:12:37 PM
 *  Author: andre
 */ 

#pragma once

#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <event_groups.h>		
#include <lora_driver.h>



void lora_handler_create( UBaseType_t lora_handler_task_priority, EventGroupHandle_t xCreatedEventGroup );