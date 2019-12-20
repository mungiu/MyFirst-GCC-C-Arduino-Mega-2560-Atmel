/**
 * \file
 * \authors Andrei Mungiu, Jakub Duchon, Jeppe Graasbøll Jensen, Maria Louisa Failli
 * \version 1.0.0
 *
 * \defgroup co2_data_class CO2 Data Class
 * \brief This file contains functions responsible for manipulating with CO2 data struct
 *
 * This file contains functions responsible for initializing, getting, changing and deleting a CO2 data struct (type)
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct co2_data_t* pco2_data;

/**
 * \ingroup co2_data_class
 * \brief Creates and initializes a CO2 data struct (type)
 *
 * Acts as a constructor; Creates and initializes a CO2 data struct (type) and returns pointer to that data struct.
 *
 * \param[in] co2_data_value Value of a measurement
 * \param[in] is_corrupt_data Boolean stating if a data are corrupted (were not read and written successfully)
 *
 * \return pointer to a created struct
 */
pco2_data create_co2_data(uint16_t co2_data_value, bool is_corrupt_data);

/**
 * \ingroup co2_data_class
 * \brief Sets values in a CO2 data struct (type)
 *
 * Acts as a setter; Sets values in a CO2 data struct (type).
 *
 * \param[in] co2_data_value Value of a measurement
 * \param[in] is_corrupt_data Boolean stating if a data are corrupted (were not read and/or written successfully)
 */
void set_is_corrupt_data_c(pco2_data co2_data, bool is_corrupt_data);

/**
 * \ingroup co2_data_class
 * \brief Retrieves and returns the is_corrupted boolean value stored in a CO2 data struct
 *
 * Acts as a getter; Retrieves and returns a is_corrupted boolean value stored in a CO2 data struct
 *
 * \param[in] co2_data Pointer to a CO2 data struct
 *
 * \return Value of is_corrupted_data
 */
bool get_is_corrupt_data_c(pco2_data co2_data);

/**
 * \ingroup co2_data_class
 * \brief Sets values in a CO2 data struct (type)
 *
 * Acts as a setter; Sets values in a CO2 data struct (type).
 *
 * \param[in] co2_data Pointer to a CO2 data struct
 * \param[in] co2_data_value Value of a measurement
 */
void set_co2_data(pco2_data co2_data, uint16_t co2_data_value);

/**
 * \ingroup co2_data_class
 * \brief Retrieves and returns a co2_data_value stored in a CO2 data struct
 *
 * Acts as a getter; Retrieves and returns the co2_data_value stored in a CO2 data struct
 *
 * \param[in] co2_data Pointer to a CO2 data struct
 *
 * \return Value of co2_data_value
 */
uint16_t get_co2_data(pco2_data co2_data_value);

/**
 * \ingroup co2_data_class
 * \brief Destroys a CO2 data struct and frees up it's memory
 *
 * Destroys a CO2 data struct and frees up it's memory.
 *
 * \param[in] co2_data Pointer to a CO2 data struct
 */
void destroy_co2_data(pco2_data co2_data);

void print_co2_data(pco2_data co2_data);
