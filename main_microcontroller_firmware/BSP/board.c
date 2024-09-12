
/* Private includes --------------------------------------------------------------------------------------------------*/

#include "board.h"
#include "bsp_pushbutton.h"
#include "bsp_status_led.h"
#include "bsp_uart.h"

#include "mxc_errors.h"
#include "uart.h"

/* Public function definitions ---------------------------------------------------------------------------------------*/

int Board_Init(void)
{
    status_led_init();

    pushbuttons_init();

    int err;

    if ((err = bsp_console_uart_init()) != E_NO_ERROR)
    {
        return err;
    }

    return E_NO_ERROR;
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
