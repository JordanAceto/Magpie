/**
 * NOTE: On the FTC spin the I2C0 pullups are tied to VREGO_A which is an intrnally generated 1.8V source. VREGO_A is
 * always enabled, but the downstream devices can be powered on and off via LDO enable pins and load switches. This
 * means that the downstream devices will see 1.8V through the pullup resistors even when they are powered off.
 *
 * To avoid violating the voltage levels of the downstream devices when they are powered down, we drive the two pins
 * for I2C0 low when the devices are powered down. This wastes a small amount of power in the resistors. In the next
 * spin we can avoid this issue by tying the pullups to the normal 1.8V rail.
 */

/* Private includes --------------------------------------------------------------------------------------------------*/

#include "bsp_i2c.h"
#include "mxc_errors.h"

/* Public function definitions ---------------------------------------------------------------------------------------*/

int bsp_1v8_i2c_init()
{
    const mxc_gpio_cfg_t i2c0_pins = {
        .port = MXC_GPIO0,
        .mask = (MXC_GPIO_PIN_6 | MXC_GPIO_PIN_7),
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_ALT1,
        .vssel = MXC_GPIO_VSSEL_VDDIO,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&i2c0_pins);

    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_I2C0);

    int res = E_NO_ERROR;

    if ((res = MXC_I2C_Init(BSP_I2C_1V8_BUS_HANDLE, 1, 0)) != E_NO_ERROR)
    {
        return res;
    }

    if ((res = MXC_I2C_SetFrequency(BSP_I2C_1V8_BUS_HANDLE, BSP_I2C_1V8_BUS_SPEED)) != BSP_I2C_1V8_BUS_SPEED)
    {
        return res;
    }

    return E_NO_ERROR;
}

int bsp_1V8_i2c_deinit()
{
    const mxc_gpio_cfg_t i2c0_pins = {
        .port = MXC_GPIO0,
        .mask = (MXC_GPIO_PIN_6 | MXC_GPIO_PIN_7),
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_OUT, // this is a workaround to avoid violating downstream devices
        .vssel = MXC_GPIO_VSSEL_VDDIO,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&i2c0_pins);
    // drive the pins low to avoid violating downstream devices, we can remove this when we move to the next spin
    gpio_write_pin(&i2c0_pins, false);

    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_I2C0);

    return MXC_I2C_Shutdown(BSP_I2C_1V8_BUS_HANDLE);
}

int bsp_3v3_i2c_init()
{
    const mxc_gpio_cfg_t i2c1_pins = {
        .port = MXC_GPIO0,
        .mask = (MXC_GPIO_PIN_14 | MXC_GPIO_PIN_15),
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_ALT1,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&i2c1_pins);

    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_I2C1);

    int res = E_NO_ERROR;

    if ((res = MXC_I2C_Init(BSP_I2C_3V3_BUS_HANDLE, 1, 0)) != E_NO_ERROR)
    {
        return res;
    }

    if ((res = MXC_I2C_SetFrequency(BSP_I2C_3V3_BUS_HANDLE, BSP_I2C_3V3_BUS_SPEED)) != BSP_I2C_3V3_BUS_SPEED)
    {
        return res;
    }

    return E_NO_ERROR;
}

int bsp_3v3_i2c_deinit()
{
    const mxc_gpio_cfg_t i2c1_pins = {
        .port = MXC_GPIO0,
        .mask = (MXC_GPIO_PIN_14 | MXC_GPIO_PIN_15),
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_IN,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&i2c1_pins);

    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_I2C1);

    return MXC_I2C_Shutdown(BSP_I2C_3V3_BUS_HANDLE);
}
