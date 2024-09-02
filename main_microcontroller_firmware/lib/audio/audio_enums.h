
#ifndef AUDIO_ENUMS_H__
#define AUDIO_ENUMS_H__

/* Public types ------------------------------------------------------------------------------------------------------*/

/**
 * @brief enumerated options for number of chanels are represented here.
 */
typedef enum
{
    AUDIO_NUM_CHANNELS_1 = 1,
    AUDIO_NUM_CHANNELS_2 = 2,
} Audio_Num_Channels_t;

/**
 * @brief enumerated audio channels are represented here.
 */
typedef enum
{
    AUDIO_CHANNEL_0 = 0,
    AUDIO_CHANNEL_1 = 1,
} Audio_Channel_t;

/**
 * @brief Enumerated options for bits per sample are represented here.
 */
typedef enum
{
    AUDIO_16_BITS_PER_SAMPLE = 16,
    AUDIO_24_BITS_PER_SAMPLE = 24,
} Audio_Bits_Per_Sample_t;

/**
 * @brief Enumerated audio sample rates are represented here.
 */
typedef enum
{
    AUDIO_SAMPLE_RATE_24kHz = 24000,
    AUDIO_SAMPLE_RATE_48kHz = 48000,
    AUDIO_SAMPLE_RATE_96kHz = 96000,
    AUDIO_SAMPLE_RATE_192kHz = 192000,
    AUDIO_SAMPLE_RATE_384kHz = 384000,
} Audio_Sample_Rate_t;

#endif
