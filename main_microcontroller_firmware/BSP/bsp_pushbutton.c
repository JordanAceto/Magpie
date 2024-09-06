
/* Private includes --------------------------------------------------------------------------------------------------*/

#include "mxc_device.h"

#include "board.h"
#include "bsp_pushbutton.h"

/* Private variables -------------------------------------------------------------------------------------------------*/

static const mxc_gpio_cfg_t user_pushbutton = {
    .port = MXC_GPIO0,
    .mask = MXC_GPIO_PIN_5,
    .pad = MXC_GPIO_PAD_PULL_UP,
    .func = MXC_GPIO_FUNC_IN,
    .vssel = MXC_GPIO_VSSEL_VDDIOH,
};

static const mxc_gpio_cfg_t ble_en_pushbutton = {
    .port = MXC_GPIO0,
    .mask = MXC_GPIO_PIN_8,
    .pad = MXC_GPIO_PAD_PULL_UP,
    .func = MXC_GPIO_FUNC_IN,
    .vssel = MXC_GPIO_VSSEL_VDDIOH,
};

/* Public function definitions ---------------------------------------------------------------------------------------*/

void pushbuttons_init()
{
    MXC_GPIO_Config(&user_pushbutton);
    MXC_GPIO_Config(&ble_en_pushbutton);
}

Button_State_t user_pushbutton_state()
{
    // TODO, code to debounce the switch and get the other states

    // note: the button has a pullup which is pulled to ground, so the action is inverting
    return gpio_read_pin(&user_pushbutton) ? BUTTON_STATE_NOT_PRESSED : BUTTON_STATE_PRESSED;
}

Button_State_t ble_enable_pushbutton_state()
{
    // TODO, code to debounce the switch and get the other states

    // note: the button has a pullup which is pulled to ground, so the action is inverting
    return gpio_read_pin(&ble_en_pushbutton) ? BUTTON_STATE_NOT_PRESSED : BUTTON_STATE_PRESSED;
}
