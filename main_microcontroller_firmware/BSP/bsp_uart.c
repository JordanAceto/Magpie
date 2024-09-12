
/* Private includes --------------------------------------------------------------------------------------------------*/

#include "bsp_uart.h"
#include "mxc_errors.h"

/* Public function definitions ---------------------------------------------------------------------------------------*/

int bsp_nordic_uart_init()
{
    const mxc_gpio_cfg_t nordic_uart_pins = {
        .port = MXC_GPIO1,
        .mask = MXC_GPIO_PIN_9 | MXC_GPIO_PIN_10,
        .pad = MXC_GPIO_PAD_WEAK_PULL_UP,
        .func = MXC_GPIO_FUNC_ALT3,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&nordic_uart_pins);

    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_UART0);

    return MXC_UART_Init(BSP_NORDIC_UART_HANDLE, BSP_NORDIC_UART_BAUD, MAP_A);
}

int bsp_nordic_uart_deinit()
{
    const mxc_gpio_cfg_t nordic_uart_pins = {
        .port = MXC_GPIO1,
        .mask = MXC_GPIO_PIN_9 | MXC_GPIO_PIN_10,
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_IN,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&nordic_uart_pins);

    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_UART0);

    return MXC_UART_Shutdown(BSP_NORDIC_UART_HANDLE);
}

int bsp_console_uart_init()
{
    const mxc_gpio_cfg_t console_uart_pins = {
        .port = MXC_GPIO1,
        .mask = MXC_GPIO_PIN_12 | MXC_GPIO_PIN_13,
        .pad = MXC_GPIO_PAD_WEAK_PULL_UP,
        .func = MXC_GPIO_FUNC_ALT3,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&console_uart_pins);

    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_UART1);

    return MXC_UART_Init(BSP_CONSOLE_UART_HANDLE, BSP_CONSOLE_UART_BAUD, MAP_B);
}

int bsp_console_uart_deinit()
{
    const mxc_gpio_cfg_t console_uart_pins = {
        .port = MXC_GPIO1,
        .mask = MXC_GPIO_PIN_12 | MXC_GPIO_PIN_13,
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_IN,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&console_uart_pins);

    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_UART1);

    return MXC_UART_Shutdown(BSP_CONSOLE_UART_HANDLE);
}

int bsp_gnss_uart_init()
{
    const mxc_gpio_cfg_t gnss_uart_pins = {
        .port = MXC_GPIO1,
        .mask = MXC_GPIO_PIN_9 | MXC_GPIO_PIN_10,
        .pad = MXC_GPIO_PAD_WEAK_PULL_UP,
        .func = MXC_GPIO_FUNC_ALT3,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&gnss_uart_pins);

    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_UART2);

    return MXC_UART_Init(BSP_GNSS_UART_HANDLE, BSP_GNSS_UART_BAUD, MAP_B);
}

int bsp_gnss_uart_deinit()
{
    const mxc_gpio_cfg_t gnss_uart_pins = {
        .port = MXC_GPIO1,
        .mask = MXC_GPIO_PIN_28 | MXC_GPIO_PIN_29,
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_IN,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&gnss_uart_pins);

    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_UART2);

    return MXC_UART_Shutdown(BSP_GNSS_UART_HANDLE);
}
