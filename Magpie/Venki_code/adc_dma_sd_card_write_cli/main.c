
/* Private includes --------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include "led.h"
#include "board.h"
#include "mxc_delay.h"
#include "tmr.h"
#include "ad4630.h"
#include "audio_dma.h"
#include "data_converters.h"
#include "decimation_filter.h"
#include "demo_config.h"
#include "gpio_helpers.h"
#include "sd_card.h"
#include "wav_header.h"

/* Headers required for example */
#include "led.h"
#include "cli.h"
#include "ctype.h"
#include "stdlib.h"
#include "string.h"
#include "uart.h"
#include "time.h"

/* Definitions used for CLI example ----------------------------------------------------------------------------------------------*/

#define CUSTOM_COMMANDS_ARRAY_SIZE 2
#define MAX_STRING_SIZE 100
#define CLI_UART MXC_UART0
#define PRINT_LOG(msg) printf("%s\n\r", msg)

/* Command declerations ----------------------------------------------------------------------------------------------*/
int cli_led_toogle(int argc, char *argv[]);
int cli_wav(int argc, char *argv[]);


/* Globals used for CLI example ----------------------------------------------------------------------------------------------*/
const command_t commands[CUSTOM_COMMANDS_ARRAY_SIZE] =
{
        {
            "write_wav_file", "[write_wav_file] [bit_depth] [sample_rate]", "Starts the ADC/DMA and continuously records audio in blocking fashion until the time is up. Example write_wav_file 24 96000", cli_wav
        },

        {
            "led_toggle", "[led_toggle] on command line", "Changes blue LED status", cli_led_toogle
        },

};

static int toggle_event = 0;
static int wav_event = 0;
static int cli_bit_depth, cli_sample_rate;
int num1, num2;

/* Funcitons used for CLI example ----------------------------------------------------------------------------------------------*/
int cli_led_toogle(int argc, char *argv[])
{
    if (argc != 1)
    {
        toggle_event = -1;
        return -1;
    }

    toggle_event = 1;
    return 0;
}

int cli_wav(int argc, char *argv[])
{
    if (argc != 3) // We make sure three arguments are enterd [write_wav_file] [bit_depth] [sample_rate]
    {
        wav_event = -1;
        return -1;
    }
    wav_event = 1;
    num1 = atoi(argv[1]);
    num2 = atoi(argv[2]);
    cli_bit_depth = num1;
    cli_sample_rate = num2;

   // printf("bit depth  = %d, s_rate = %d", num1,num2);
    return 0;
}

/* Private enumerations ----------------------------------------------------------------------------------------------*/

/**
 * Onboard LED colors for the FTHR2 board are represented here, note that blue and green are reversed wrt original FTHR
 */
typedef enum
{
    LED_COLOR_RED = 0,
    LED_COLOR_GREEN,
    LED_COLOR_BLUE,

} LED_Color_t;

/* Private variables -------------------------------------------------------------------------------------------------*/

// a buffer for downsampled audio
static uint8_t downsampled_audio[AUDIO_DMA_LARGEST_BUFF_LEN_IN_BYTES];

/* Private function declarations -------------------------------------------------------------------------------------*/

/**
 * @brief `write_demo_wav_file(a, l)` writes a wav file with attributes `a`, and length in seconds `l`, with a name
 * derived from the attributes. Calling this function starts the ADC/DMA and continuously records audio in blocking
 * fashion until the time is up.
 *
 * @pre initialization is complete for the ADC, DMA, decimation filters, and SD card, the SD card must be mounted
 *
 * @param wav_attr pointer to the wav header attributes structure holding information about sample rate, bit depth, etc
 *
 * @param file_len_secs the length of the audio file to write, in seconds
 *
 * @post this function consumes buffers from the ADC/DMA until the duration of the file length has elapsed and writes
 * the audio data out to a .wav file on the SD card. The wav header for the file is also written in this function.
 */
static void write_demo_wav_file(Wave_Header_Attributes_t *wav_attr, uint32_t file_len_secs);

// the error handler simply rapidly blinks the given LED color forever
static void error_handler(LED_Color_t c);

/* Public function definitions ---------------------------------------------------------------------------------------*/

int main(void)
{
    // blue led on during initialization
    LED_On(LED_COLOR_BLUE);

    gpio_profiling_pin_init();

    if (ad4630_init() != AD4630_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_BLUE);
    }
    if (audio_dma_init() != AUDIO_DMA_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_BLUE);
    }

    if (sd_card_init() != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    if (sd_card_mount() != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    Wave_Header_Attributes_t wav_attr =
        {
            .num_channels = WAVE_HEADER_MONO, // only mono is supported for now, 2 channel might be added later
        };

    LED_Off(LED_COLOR_BLUE);

    // for (uint32_t sr = 0; sr < DEMO_CONFIG_NUM_SAMPLE_RATES_TO_TEST; sr++)
    // {
    //     for (uint32_t bd = 0; bd < DEMO_CONFIG_NUM_BIT_DEPTHS_TO_TEST; bd++)
    //     {
    //         // green led on during recording
    //         LED_On(LED_COLOR_GREEN);
    //         wav_attr.sample_rate = demo_sample_rates_to_test[sr];
    //         wav_attr.bits_per_sample = demo_bit_depths_to_test[bd];
    //         write_demo_wav_file(&wav_attr, DEMO_CONFIG_AUDIO_FILE_LEN_IN_SECONDS);
    //         LED_Off(LED_COLOR_GREEN);

    //         MXC_Delay(500000);
    //     }
    // }

    // if (sd_card_unmount() != SD_CARD_ERROR_ALL_OK)
    // {
    //     error_handler(LED_COLOR_RED);
    // }

    // do a slow green blink to indicate success
    const uint32_t slow_blink = 1000000;

    MXC_CLI_Init(CLI_UART, commands, CUSTOM_COMMANDS_ARRAY_SIZE);
    printf("CLI enter your commands \r\n");
    while (1)
    {
        if (toggle_event != 0)
        {
            if (toggle_event == 1)
            {
                PRINT_LOG("Led toggled");
                LED_Toggle(LED_COLOR_BLUE);
            }
            else
            {
                PRINT_LOG("Led toggle failed");
            }
            toggle_event = 0;
        }

        if (0 != wav_event)
        {
            if (wav_event > 0)
            {
                PRINT_LOG("Wave record started\r\n");
                // green led on during recording
                LED_On(LED_COLOR_GREEN);                
                wav_attr.bits_per_sample = cli_bit_depth;
                wav_attr.sample_rate = cli_sample_rate;
                write_demo_wav_file(&wav_attr, DEMO_CONFIG_AUDIO_FILE_LEN_IN_SECONDS);
                LED_Off(LED_COLOR_GREEN);

                MXC_Delay(500000);

                // if (sd_card_unmount() != SD_CARD_ERROR_ALL_OK)
                // {
                //     error_handler(LED_COLOR_RED);
                // }
                PRINT_LOG("Wave record success");
            }
            else
            {
                PRINT_LOG("Wave recored fail");
            }
            wav_event = 0;
        }

        LED_On(LED_COLOR_GREEN);
        MXC_Delay(slow_blink);
        LED_Off(LED_COLOR_GREEN);
        MXC_Delay(slow_blink);
    }
}

/* Private function definitions --------------------------------------------------------------------------------------*/

void write_demo_wav_file(Wave_Header_Attributes_t *wav_attr, uint32_t file_len_secs)
{
    // a variable to store the number of bytes written to the SD card, can be checked against the intended amount
    static uint32_t bytes_written;

    // a string buffer to write file names into
    static char file_name_buff[64];

    // we'll store the time it takes to write out each block here
    static uint32_t block_write_time_microsecs[4000] = {0}; // 4k is enough if we stay under 70 seconds

    // for misc SD card writing, a buffer to sprintf into
    static char str_buff[64] = {0};

    // there will be some integer truncation here, good enough for this early demo, but improve file-len code eventually
    const uint32_t file_len_in_microsecs = file_len_secs * 1000000;
    const uint32_t num_dma_blocks_in_the_file = file_len_in_microsecs / AUDIO_DMA_CHUNK_READY_PERIOD_IN_MICROSECS;

    // derive the file name from the input parameters
    sprintf(file_name_buff, "demo_%dkHz_%d_bit.wav", wav_attr->sample_rate / 1000, wav_attr->bits_per_sample);

    if (sd_card_fopen(file_name_buff, POSIX_FILE_MODE_WRITE) != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    // seek past the wave header, we'll fill it in later after recording the audio, we'll know the file length then
    if (sd_card_lseek(wav_header_get_header_length()) != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    // 384kHz is a special case where we take 24 bit samples from the DMA, for all other sample rates we want 32 bit samples
    const Audio_DMA_Sample_Width_t sample_width = wav_attr->sample_rate == WAVE_HEADER_SAMPLE_RATE_384kHz ? AUDIO_DMA_SAMPLE_WIDTH_24_BITS : AUDIO_DMA_SAMPLE_WIDTH_32_BITS;
    audio_dma_set_sample_width(sample_width);

    decimation_filter_set_sample_rate(wav_attr->sample_rate);

    ad4630_cont_conversions_start();
    audio_dma_start();

    for (uint32_t num_dma_blocks_written = 0; num_dma_blocks_written < num_dma_blocks_in_the_file;)
    {
        if (audio_dma_overrun_occured())
        {
            error_handler(LED_COLOR_BLUE);
        }

        while (audio_dma_num_buffers_available() > 0)
        {
            MXC_TMR_SW_Start(MXC_TMR1); // for profiling the time it takes to filter and write out the buffer

            if (wav_attr->sample_rate == WAVE_HEADER_SAMPLE_RATE_384kHz)
            {
                if (wav_attr->bits_per_sample == WAVE_HEADER_24_BITS_PER_SAMPLE)
                {
                    if (sd_card_fwrite(audio_dma_consume_buffer(), audio_dma_buffer_size_in_bytes(), &bytes_written) != SD_CARD_ERROR_ALL_OK)
                    {
                        error_handler(LED_COLOR_RED);
                    }
                }
                else // it must be 16 bits
                {
                    const uint32_t len_in_bytes = data_converters_i24_to_q15(audio_dma_consume_buffer(), (q15_t *)downsampled_audio, audio_dma_buffer_size_in_bytes());

                    if (sd_card_fwrite(downsampled_audio, len_in_bytes, &bytes_written) != SD_CARD_ERROR_ALL_OK)
                    {
                        error_handler(LED_COLOR_RED);
                    }
                }
            }
            else // it's not the special case of 384kHz, all other sample rates are filtered
            {
                const uint32_t len_in_samps = decimation_filter_downsample(
                    (q31_t *)audio_dma_consume_buffer(),
                    (q31_t *)downsampled_audio,
                    audio_dma_buffer_size_in_bytes() / 4); // we want num samples, not num bytes

                uint32_t len_in_bytes;

                if (wav_attr->bits_per_sample == WAVE_HEADER_24_BITS_PER_SAMPLE)
                {
                    len_in_bytes = data_converters_q31_to_i24((q31_t *)downsampled_audio, downsampled_audio, len_in_samps);
                }
                else // it's 16 bits
                {
                    len_in_bytes = data_converters_q31_to_q15((q31_t *)downsampled_audio, (q15_t *)downsampled_audio, len_in_samps);
                }

                if (sd_card_fwrite(downsampled_audio, len_in_bytes, &bytes_written) != SD_CARD_ERROR_ALL_OK)
                {
                    error_handler(LED_COLOR_RED);
                }
            }

            const uint32_t time_to_filter_and_write_the_block = MXC_TMR_SW_Stop(MXC_TMR1);
            block_write_time_microsecs[num_dma_blocks_written] = time_to_filter_and_write_the_block;

            num_dma_blocks_written += 1;
        }
    }

    ad4630_cont_conversions_stop();
    audio_dma_stop();

    // back to the top of the file so we can write the wav header now that we can determine the size of the file
    if (sd_card_lseek(0) != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    wav_attr->file_length = sd_card_fsize();
    wav_header_set_attributes(wav_attr);

    if (sd_card_fwrite(wav_header_get_header(), wav_header_get_header_length(), &bytes_written) != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    if (sd_card_fclose() != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    // write a file summary of the time taken to filter and write each DMA block
    if (sd_card_fopen("block_write_times_microsec.csv", POSIX_FILE_MODE_APPEND) != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    const uint32_t len = sprintf(str_buff, "\n%dk-%dbit,", wav_attr->sample_rate / 1000, wav_attr->bits_per_sample);
    if (sd_card_fwrite(str_buff, len, &bytes_written) != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }

    for (uint32_t i = 0; i < num_dma_blocks_in_the_file; i++)
    {
        const uint32_t len = sprintf(str_buff, "%d,", block_write_time_microsecs[i]);

        if (sd_card_fwrite(str_buff, len, &bytes_written) != SD_CARD_ERROR_ALL_OK)
        {
            error_handler(LED_COLOR_RED);
        }
    }

    if (sd_card_fclose() != SD_CARD_ERROR_ALL_OK)
    {
        error_handler(LED_COLOR_RED);
    }
}

void error_handler(LED_Color_t color)
{
    LED_Off(LED_COLOR_RED);
    LED_Off(LED_COLOR_GREEN);
    LED_Off(LED_COLOR_BLUE);

    const uint32_t fast_blink = 100000;
    while (true)
    {
        LED_On(color);
        MXC_Delay(fast_blink);
        LED_Off(color);
        MXC_Delay(fast_blink);
    }
}
