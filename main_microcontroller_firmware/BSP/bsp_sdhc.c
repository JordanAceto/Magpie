/**
 * IMPORTANT: as of this writing, the preprocessor definition `MSDK_NO_GPIO_CLK_INIT` has no effect on the SDHC lib,
 * this means that ALL of the SDHC pins will be configured when we call `MXC_SDHC_Init()`. The problem with this is
 * that we use P1.6 for an unrelated task. This pin is the SDHC Write Protect pin, but we don't use it for that.
 *
 * Because of this we need to make sure that initializing an de-initializing the SDHC does not overwrite the pin config
 * for P1.6. We need to read the pin config for P1.6, stash it, and then reconfigure P1.6 when we're done configuring
 * the SDHC peripheral. This remains as a TODO.
 */

/* Private includes --------------------------------------------------------------------------------------------------*/

#include "bsp_sdhc.h"
#include "sdhc_lib.h"

/* Private defines ---------------------------------------------------------------------------------------------------*/

#define SD_CARD_INIT_NUM_RETRIES (100)

#define SDHC_CONFIG_BLOCK_GAP (0)
#define SDHC_CONFIG_CLK_DIV (0x0b0)

/* Public function definitions ---------------------------------------------------------------------------------------*/

int bsp_sdhs_init()
{
    // TODO: get the configuration state of P1.6 and save it so we can re-init that pin

    const mxc_gpio_cfg_t sdhc_pins = {
        .port = MXC_GPIO1,
        .mask = MXC_GPIO_PIN_0 | MXC_GPIO_PIN_1 | MXC_GPIO_PIN_2 | MXC_GPIO_PIN_3 | MXC_GPIO_PIN_4 | MXC_GPIO_PIN_5,
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_ALT1,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&sdhc_pins);

    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_SDHC);

    const mxc_sdhc_cfg_t sdhc_cfg = {
        .bus_voltage = MXC_SDHC_Bus_Voltage_3_3,
        .block_gap = SDHC_CONFIG_BLOCK_GAP,
        .clk_div = SDHC_CONFIG_CLK_DIV,
    };

    int res = E_NO_ERROR;

    if ((res = MXC_SDHC_Init(&sdhc_cfg)) != E_NO_ERROR)
    {
        return res;
    }

    // TODO: re-init P1.6 with the saved config from above

    return E_NO_ERROR;
}

int bsp_sdhs_deinit()
{
    const mxc_gpio_cfg_t sdhc_pins = {
        .port = MXC_GPIO1,
        .mask = MXC_GPIO_PIN_0 | MXC_GPIO_PIN_1 | MXC_GPIO_PIN_2 | MXC_GPIO_PIN_3 | MXC_GPIO_PIN_4 | MXC_GPIO_PIN_5,
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_IN,
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .drvstr = MXC_GPIO_DRVSTR_0,
    };
    MXC_GPIO_Config(&sdhc_pins);

    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_SDHC);

    // TODO: make sure that the SDHC shutdown doesn't mess with pin P1.6

    return MXC_SDHC_Shutdown();
}
