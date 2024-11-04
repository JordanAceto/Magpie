/**
 * @file      afe_control.h
 * @brief     A software module for controlling the Analog Front End (AFE) is represented here.
 * @details   The gain is set by switching resistors in a differential opamp circuit. Eight discrete gain settings are
 * available. AFE channels are enabled and disabled via load switches controlled by GPIO pins and an I2C controlled
 * load switch.
 *
 * This module requires:
 * - Shared use of I2C0 using 7-bit addresses 0x4E and 0x4F
 * - Shared use of I2C1 using 7-bit address 0x71
 * - Exclusive use of P0.11 and P0.12
 */

#ifndef AFE_GAIN_CTL_H_
#define AFE_GAIN_CTL_H_

/* Includes ----------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>

/* Public enumerations -----------------------------------------------------------------------------------------------*/

/**
 * @brief enumerated AFE gain control channels are represented here.
 */
typedef enum
{
    AFE_CONTROL_CHANNEL_0 = 0,
    AFE_CONTROL_CHANNEL_1,
} AFE_Control_Channel_t;

/**
 * @brief  Enumerated AFE gain settings are represented here.
 */
typedef enum
{
    AFE_CONTROL_GAIN_5dB = 5,
    AFE_CONTROL_GAIN_10dB = 10,
    AFE_CONTROL_GAIN_15dB = 15,
    AFE_CONTROL_GAIN_20dB = 20,
    AFE_CONTROL_GAIN_25dB = 25,
    AFE_CONTROL_GAIN_30dB = 30,
    AFE_CONTROL_GAIN_35dB = 35,
    AFE_CONTROL_GAIN_40dB = 40,
    AFE_CONTROL_GAIN_UNDEFINED = 255, // sentinal value for undefined or invalid gain settings
} AFE_Control_Gain_t;

/* Public function declarations --------------------------------------------------------------------------------------*/

/**
 * @brief `afe_control_init()` initializes the AFE gain control circuitry.
 *
 * @pre the I2C busses on the 1.8V and 3.3V domain are configured as I2C masters and have pullup resistors to 1.8V
 * and 3.3V respectively. The LDOs are powered on via `bsp_power_on_LDOs()`.
 *
 * @post the AFE gain control is initialized and ready to use.
 *
 * @retval `E_NO_ERROR` if successful, else a negative error code.
 */
int afe_control_init();

/**
 * @brief `afe_control_enable(c)` enables AFE channel `c`, powering it on.
 *
 * @pre `afe_gain_ctl_init()` has been called with all its preconditions met. The LDOs are powered on via
 * `bsp_power_on_LDOs()`.
 *
 * @param channel the enumerated AFE channel to enable.
 *
 * @post the given channel is powered on.
 *
 * @retval `E_NO_ERROR` if successful, else a negative error code.
 */
int afe_control_enable(AFE_Control_Channel_t channel);

/**
 * @brief `afe_control_disable(c)` disables AFE channel `c`, powering it off.
 *
 * @pre `afe_gain_ctl_init()` has been called with all its preconditions met. The LDOs are powered on via
 * `bsp_power_on_LDOs()`.
 *
 * @param channel the enumerated AFE channel to disable.
 *
 * @post the given channel is powered off.
 *
 * @retval `E_NO_ERROR` if successful, else a negative error code.
 */
int afe_control_disable(AFE_Control_Channel_t channel);

/**
 * @brief `afe_control_channel_is_enabled(c)` is true iff AFE channel `c` is enabled.
 *
 * @pre `afe_gain_ctl_init()` has been called with all its preconditions met. The LDOs are powered on via
 * `bsp_power_on_LDOs()`.
 *
 * @param channel the channel to check
 *
 * @retval true if the given channel is enabled, else false
 */
bool afe_control_channel_is_enabled(AFE_Control_Channel_t channel);

/**
 * @brief `afe_control_set_gain(c, g)` sets the gain of the AFE channel `c` to enumerated gain setting `g`
 *
 * @param channel the enumerated channel to set the gain of.
 *
 * @param gain the enumerated gain setting to use.
 *
 * @pre `afe_gain_ctl_init()` has been called with all its preconditions met and channel `c` is enabled. The LDOs
 * are powered on via `bsp_power_on_LDOs()`.
 *
 * @post The gain of AFE channel `c` is changed to enumerated gain setting `g`.
 *
 * @retval `E_NO_ERROR` if successful, else a negative error code.
 */
int afe_control_set_gain(AFE_Control_Channel_t channel, AFE_Control_Gain_t gain);

/**
 * @brief `afe_gain_ctl_get_gain(c)` is the current AFE gain setting of AFE channel `c`.
 *
 * @param channel the enumerated channel to read the gain of.
 *
 * @pre `afe_gain_ctl_init()` has been called with all its preconditions met and channel `c` is enabled. The LDOs
 * are powered on via `bsp_power_on_LDOs()`.
 *
 * @return the current gain setting, or `AFE_GAIN_SETTING_UNDEFINED` if there was an error reading the gain.
 */
AFE_Control_Gain_t afe_control_get_gain(AFE_Control_Channel_t channel);

#endif /* AFE_GAIN_CTL_H_ */
