
/* Private includes --------------------------------------------------------------------------------------------------*/

#include "board.h"
#include "bsp_pushbutton.h"
#include "bsp_status_led.h"

#include "mxc_errors.h"
#include "uart.h"

/* Private variables -------------------------------------------------------------------------------------------------*/

// UART to communicate to a host computer via the debug header/DAP interface
static mxc_uart_regs_t *ConsoleUart = MXC_UART_GET_UART(CONSOLE_UART);

/* Private function declarations -------------------------------------------------------------------------------------*/

static int Console_Init(void);

/* Public function definitions ---------------------------------------------------------------------------------------*/

int Board_Init(void)
{
    status_led_init();

    pushbuttons_init();

    int err;

    if ((err = Console_Init()) != E_NO_ERROR)
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

/* Private function definitions --------------------------------------------------------------------------------------*/

int Console_Init(void)
{
    return MXC_UART_Init(ConsoleUart, CONSOLE_BAUD, MAP_B); // MAP_B -> UART pins on P1.12 and P1.13
}
