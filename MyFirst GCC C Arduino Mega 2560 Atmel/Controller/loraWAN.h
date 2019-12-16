/**
 * \file
 * \authors Andrei Mungiu, Jakub Duchon, Jeppe Graasbøll Jensen, Maria Louisa Failli
 * \version 1.0.0
 *
 * \defgroup controllers Controllers for sensors and modules
 * \brief This file contains a method responsible for creating a LoRaWAN Handler Task
 */

#pragma once

#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <event_groups.h>		
#include <lora_driver.h>

// Creates a Task which will be responsible for launching the LoRaWAN module.
/**
 * \ingroup controllers
 * \brief A method responsible for creating a LoRaWAN Handler Task
 *
 * A method creating a LoRaWAN Handler Task. The task is responsible for all features of the LoRaWAN module;
 * Converting data from/to bytes, receiving and sending data to the network.
 *
 * \param[in] lora_handler_task_priority priority of the task
 * \param[in] xCreatedEventGroup the main EventGroup responsible for a work distribution
 */
void lora_handler_create( UBaseType_t lora_handler_task_priority, EventGroupHandle_t xCreatedEventGroup );