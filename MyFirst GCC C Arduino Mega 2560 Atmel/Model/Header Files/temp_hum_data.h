/**
 * \file
 * \authors Andrei Mungiu, Jakub Duchon, Jeppe Graasbøll Jensen, Maria Louisa Failli
 * \version 1.0.0
 *
 * \defgroup temp_hum_data Temperature and Humidity Data Class
 * \brief This file contains functions responsible for manipulating with temp_hum data struct
 *
 * This file contains functions responsible for initializing, getting, changing and deleting a temp_hum data struct (type)
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct temp_hum_data_t* ptemp_hum_data;

/**
 * \ingroup temp_hum_data
 * \brief Creates and initializes a temp_hum data struct (type)
 *
 * Acts as a constructor; Creates and initializes a temp_hum data struct (type) and returns pointer to that data struct.
 *
 * \param[in] temp_data_value Value of a measurement
 * \param[in] hum_data_value Value of a measurement
 * \param[in] is_corrupt_data Boolean stating if a data are corrupted (were not read and written successfully)
 *
 * \return pointer to a created struct
 */
ptemp_hum_data create_temp_hum_data(uint16_t temp_data_value, uint16_t hum_data_value, bool is_corrupt_data);

/**
 * \ingroup temp_hum_data
 * \brief Sets is_corrupt_data value in a temp_hum data struct (type)
 *
 * Acts as a setter; Sets is_corrupt_data value in a temp_hum data struct (type).
 *
 * \param[in] temp_hum_data Pointer to a temp_hum data struct
 * \param[in] is_corrupt_data Boolean stating if a data are corrupted (were not read and/or written successfully)
 */
void set_is_corrupt_data_th(ptemp_hum_data temp_hum_data, bool is_corrupt_data);

/**
 * \ingroup temp_hum_data
 * \brief Retrieves and returns the is_corrupted boolean value stored in a temp_hum data struct
 *
 * Acts as a getter; Retrieves and returns a is_corrupted boolean value stored in a temp_hum data struct
 *
 * \param[in] bool_currupt_data Pointer to a temp_hum data struct
 *
 * \return Value of is_corrupted_data
 */
bool get_is_corrupt_data_th(ptemp_hum_data bool_currupt_data);

/**
 * \ingroup temp_hum_data
 * \brief Sets the hum_data_value value in a temp_hum data struct (type)
 *
 * Acts as a setter; Sets the hum_data_value value in a temp_hum data struct (type).
 *
 * \param[in] temp_hum_data Pointer to a temp_hum data struct
 * \param[in] hum_data_value Value of a measurement
 */
void set_hum_data(ptemp_hum_data temp_hum_data, uint16_t hum_data_value);

/**
 * \ingroup temp_hum_data
 * \brief Retrieves and returns the hum_data_value value stored in a temp_hum data struct
 *
 * Acts as a getter; Retrieves and returns the hum_data_value value stored in a temp_hum data struct
 *
 * \param[in] temp_hum_data Pointer to a temp_hum data struct
 *
 * \return Value of hum_data_value
 */
uint16_t get_hum_data(ptemp_hum_data temp_hum_data);

/**
 * \ingroup temp_hum_data
 * \brief Sets the temp_data_value value in a temp_hum data struct (type)
 *
 * Acts as a setter; Sets the temp_data_value value in a temp_hum data struct (type).
 *
 * \param[in] temp_hum_data Pointer to a temp_hum data struct
 * \param[in] temp_data_value Value of a measurement
 */
void set_temp_data(ptemp_hum_data temp_hum_data, uint16_t temp_data_value);

/**
 * \ingroup temp_hum_data
 * \brief Retrieves and returns the temp_data_value value stored in a temp_hum data struct
 *
 * Acts as a getter; Retrieves and returns the temp_data_value value stored in a temp_hum data struct
 *
 * \param[in] temp_data_value Pointer to a temp_hum data struct
 *
 * \return Value of temp_data_value
 */
uint16_t get_temp_data(ptemp_hum_data temp_hum_data);

/**
 * \ingroup temp_hum_data
 * \brief Destroys a temp_hum data struct and frees up it's memory
 *
 * Destroys a temp_hum data struct and frees up it's memory.
 *
 * \param[in] temp_hum_data Pointer to a temp_hum data struct
 */
void destroy_temp_hum_data(ptemp_hum_data temp_hum_data);

void print_temp_hum_data(ptemp_hum_data temp_hum_data);
