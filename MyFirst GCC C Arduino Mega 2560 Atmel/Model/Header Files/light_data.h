/**
 * \file
 * \authors Andrei Mungiu, Jakub Duchon, Jeppe Graasbøll Jensen, Maria Louisa Failli
 * \version 1.0.0
 *
 * \defgroup light_data_class Light Data Class
 * \brief This file contains functions responsible for manipulating with a Light data struct
 *
 * This file contains functions responsible for initializing, getting, changing and deleting a Light data struct (type)
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct light_data_t* plight_data;

/**
 * \ingroup light_data_class
 * \brief Creates and initializes a Light data struct (type)
 *
 * Acts as a constructor; Creates and initializes a Light data struct (type) and returns pointer to that data struct.
 *
 * \param[in] _fullRaw Full raw value of a measurement
 * \param[in] _visibleRaw Visible raw value of a measurement
 * \param[in] _infraredRaw Infrared raw value of a measurement
 * \param[in] _lux Lux value of a measurement
 * \param[in] is_corrupt_data Boolean stating if a data are corrupted (were not read and written successfully)
 *
 * \return pointer to a created struct
 */
plight_data create_light_data(uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux, bool corrupt_data);

/**
 * \ingroup light_data_class
 * \brief Sets values in a light data struct (type)
 *
 * Acts as a setter; Sets values in a light data struct (type).
 *
 * \param[in] light_data Value of a measurement
 * \param[in] bool_corrupt_data Boolean stating if a data are corrupted (were not read and/or written successfully)
 */
void set_is_corrupt_data(plight_data light_data, bool bool_corrupt_data);

/**
 * \ingroup light_data_class
 * \brief Retrieves and returns the is_corrupted boolean value stored in a light data struct
 *
 * Acts as a getter; Retrieves and returns a is_corrupted boolean value stored in a light data struct
 *
 * \param[in] light_data Pointer to a light data struct
 *
 * \return Value of is_corrupted_data
 */
bool get_is_corrupt_data(plight_data light_data);

/**
 * \ingroup light_data_class
 * \brief Sets values in a light data struct (type)
 *
 * Acts as a setter; Sets values in a light data struct (type).
 *
 * \param[in] light_data Pointer to a light data struct
 * \param[in] _fullRaw Full raw value of a measurement
 * \param[in] _visibleRaw Visible raw value of a measurement
 * \param[in] _infraredRaw Infrared raw value of a measurement
 * \param[in] _lux Lux value of a measurement
 */
void set_light_data(plight_data light_data, uint16_t _fullRaw, uint16_t _visibleRaw, uint16_t _infraredRaw, float _lux);

/**
 * \ingroup light_data_class
 * \brief Retrieves and returns a _infraredRaw value stored in a light data struct
 *
 * Acts as a getter; Retrieves and returns the _infraredRaw value stored in a light data struct
 *
 * \param[in] infraredRaw Pointer to a light data struct
 *
 * \return Value of _infraredRaw
 */
uint16_t get_infraredRaw(plight_data infraredRaw);

/**
 * \ingroup light_data_class
 * \brief Destroys a light data struct and frees up it's memory
 *
 * Destroys a light data struct and frees up it's memory.
 *
 * \param[in] light_data Pointer to a light data struct
 */
void destory_light_data(plight_data light_data);

void print_light_data(plight_data light_data);

