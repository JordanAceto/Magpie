
/* Private includes --------------------------------------------------------------------------------------------------*/

#include "board.h"
#include "bsp_pushbutton.h"
#include "bsp_status_led.h"
#include "bsp_uart.h"

#include "mxc_errors.h"
#include "uart.h"

/* Private variables -------------------------------------------------------------------------------------------------*/

/**
 * The pin that controls the power to the LDOs, set HIGH to power on the LDOs, set LOW to power off the LDOs.
 */
static const mxc_gpio_cfg_t ldo_enable_pin = {
    .port = MXC_GPIO0,
    .mask = MXC_GPIO_PIN_30,
    .pad = MXC_GPIO_PAD_NONE,
    .func = MXC_GPIO_FUNC_OUT,
    .vssel = MXC_GPIO_VSSEL_VDDIOH,
};

/* Public function definitions ---------------------------------------------------------------------------------------*/

int Board_Init(void)
{
    MXC_GPIO_Config(&ldo_enable_pin);
    bsp_power_off_LDOs();

    status_led_init();

    pushbuttons_init();

    int err;

    if ((err = bsp_console_uart_init()) != E_NO_ERROR)
    {
        return err;
    }

    return E_NO_ERROR;
}

void bsp_power_on_LDOs()
{
    gpio_write_pin(&ldo_enable_pin, true);
}

void bsp_power_off_LDOs()
{
    gpio_write_pin(&ldo_enable_pin, false);
}

void gpio_write_pin(const mxc_gpio_cfg_t *pin, bool state)
{
    state ? MXC_GPIO_OutSet(pin->port, pin->mask) : MXC_GPIO_OutClr(pin->port, pin->mask);
}

void gpio_toggle_pin(const mxc_gpio_cfg_t *pin)
{
    MXC_GPIO_OutToggle(pin->port, pin->mask);
}

bool gpio_read_pin(const mxc_gpio_cfg_t *pin)
{
    return (bool)MXC_GPIO_InGet(pin->port, pin->mask);
}
