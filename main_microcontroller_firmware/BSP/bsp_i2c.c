
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
        .func = MXC_GPIO_FUNC_IN,
        .vssel = MXC_GPIO_VSSEL_VDDIO,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&i2c0_pins);

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
