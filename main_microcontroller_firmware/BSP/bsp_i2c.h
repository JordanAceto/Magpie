/**
 * @file      bsp_i2c.h
 * @brief     This module is responsible for starting and stopping the I2C busses used by the sytem, and for acquiring
 *            handles to the I2C interfaces used.
 *
 * This module requires:
 * - Shared use of I2C0 and I2C1
 * - Shared use of I2C pins P0.6, P0.7, P0.14, and P0.15
 */

#ifndef BSP_I2C_H_INCLUDED__
#define BSP_I2C_H_INCLUDED__

/* Includes ----------------------------------------------------------------------------------------------------------*/

#include "i2c.h"

/* Public enumerations -----------------------------------------------------------------------------------------------*/

/**
 * @brief enumerated BSP I2C errors are represented here.
 */
typedef enum
{
    BSP_I2C_ERROR_ALL_OK,
    BSP_I2C_INITIALIZATION_ERROR,
    BPS_I2C_FREQUENCY_SET_ERROR,
    BSP_I2C_SHUTDOWN_ERROR,
} BSP_I2C_Error_t;

/**
 * @brief enumerated I2C busses used by the sytem are represented here.
 */
typedef enum
{
    BSP_I2C_1V8_BUS,
    BSP_I2C_3V3_BUS,
} BSP_I2C_Bus_t;

/* Public function declarations --------------------------------------------------------------------------------------*/

/**
 * @brief `bsp_i2c_get_handle(b)` is a pointer to the I2C handle represented by bus `b`
 *
 * @param bus the I2C handle to get.
 *
 * @retval the I2C bus represented by the given bus enumeration.
 */
mxc_i2c_regs_t *bsp_i2c_get_handle(BSP_I2C_Bus_t bus);

/**
 * @brief `bsp_i2c_start(b)` initializes and starts I2C bus `b` as an I2C master.
 *
 * @param bus the bus to start.
 *
 * @post the I2C bus is initialized and ready to use. The GPIO pins associated with bus `b` are pulled up to the
 * appropriate voltage.
 *
 * @retval `BSP_I2C_ERROR_ALL_OK` if starting bus `b` was successful, else an error code.
 */
BSP_I2C_Error_t bsp_i2c_start(BSP_I2C_Bus_t bus);

/**
 * @brief `bsp_i2c_stop(b)` deinitializes and stops I2C bus `b`.
 *
 * @param bus the I2C bus to stop.
 *
 * @post bus `b` is stopped and the GPIO pins associated with that bus are placed in a high impedance state.
 *
 * @retval `BSP_I2C_ERROR_ALL_OK` if stopping bus `b` was successful, else an error code.
 */
BSP_I2C_Error_t bsp_i2c_stop(BSP_I2C_Bus_t bus);

#endif
