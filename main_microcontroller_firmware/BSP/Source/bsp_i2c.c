
/* Private includes --------------------------------------------------------------------------------------------------*/

#include "bsp_i2c.h"
#include "i2c.h"
#include "mxc_errors.h"

/* Private variables -------------------------------------------------------------------------------------------------*/

// two I2C busses are used, one on the 1.8V domain and one on the 3.3v domain
static mxc_i2c_regs_t *hi2c_1V8 = MXC_I2C0_BUS0;
static mxc_i2c_regs_t *hi2c_3V3 = MXC_I2C1_BUS0;

/* Public function definitions ---------------------------------------------------------------------------------------*/

mxc_i2c_regs_t *bsp_i2c_get_handle(BSP_I2C_Bus_t bus)
{
    return bus == BSP_I2C_1V8_BUS ? hi2c_1V8 : hi2c_3V3;
}

BSP_I2C_Error_t bsp_i2c_start(BSP_I2C_Bus_t bus)
{
    mxc_i2c_regs_t *hi2c = bsp_i2c_get_handle(bus);

    if (MXC_I2C_Init(hi2c, 1, 0) != E_NO_ERROR)
    {
        return BSP_I2C_INITIALIZATION_ERROR;
    }

    if (bus == BSP_I2C_3V3_BUS)
    {
        // I2C pins default to VDDIO for the logical high voltage, we want VDDIOH for 3.3v pullups
        const mxc_gpio_cfg_t i2c1_pins = {
            .port = MXC_GPIO0,
            .mask = (MXC_GPIO_PIN_14 | MXC_GPIO_PIN_15),
            .pad = MXC_GPIO_PAD_NONE,
            .func = MXC_GPIO_FUNC_ALT1,
            .vssel = MXC_GPIO_VSSEL_VDDIOH,
            .drvstr = MXC_GPIO_DRVSTR_0,
        };
        MXC_GPIO_Config(&i2c1_pins);
    }

    if (MXC_I2C_SetFrequency(hi2c, MXC_I2C_STD_MODE) != MXC_I2C_STD_MODE)
    {
        return BPS_I2C_FREQUENCY_SET_ERROR;
    }

    return BSP_I2C_ERROR_ALL_OK;
}

BSP_I2C_Error_t bsp_i2c_stop(BSP_I2C_Bus_t bus)
{
    // set the pins as inputs
    if (bus == BSP_I2C_3V3_BUS)
    {
        const mxc_gpio_cfg_t i2c1_pins = {
            .port = MXC_GPIO0,
            .mask = (MXC_GPIO_PIN_14 | MXC_GPIO_PIN_15),
            .pad = MXC_GPIO_PAD_NONE,
            .func = MXC_GPIO_FUNC_IN,
            .vssel = MXC_GPIO_VSSEL_VDDIOH,
        };
        MXC_GPIO_Config(&i2c1_pins);
    }
    else
    {
        const mxc_gpio_cfg_t i2c1_pins = {
            .port = MXC_GPIO0,
            .mask = (MXC_GPIO_PIN_6 | MXC_GPIO_PIN_7),
            .pad = MXC_GPIO_PAD_NONE,
            .func = MXC_GPIO_FUNC_IN,
            .vssel = MXC_GPIO_VSSEL_VDDIO,
        };
        MXC_GPIO_Config(&i2c1_pins);
    }

    return MXC_I2C_Shutdown(bsp_i2c_get_handle(bus)) == E_NO_ERROR ? BSP_I2C_ERROR_ALL_OK : BSP_I2C_SHUTDOWN_ERROR;
}
