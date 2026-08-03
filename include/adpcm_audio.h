#ifndef GDA_ADPCM_AUDIO_H
#define GDA_ADPCM_AUDIO_H
#include <tonc.h>
#define GDAA_SAMPLE_RATE       16384u
#define GDAA_SAMPLE_RATE_CLIPPED 11025u
#define GDAA_HEADER_BYTES      28u
#define GDAA_BLOCK_SAMPLES     256u
#define GDAA_BLOCK_HEADER_BYTES 4u
#define GDAA_BLOCK_BYTES       132u
typedef struct {
    const u8* data;
    const u8* block;
    u32 sample_rate;
    u32 total_samples;
    u32 loop_start;
    u32 loop_end;
    u32 sample_position;
    u16 sample_in_block;
    s16 predictor;
    u8 step_index;
    u8 loop_enabled;
    u8 valid;
} GdaaDecoder;
u8 gdaa_open(GdaaDecoder* decoder, const u8* data, u32 data_size);
u8 gdaa_seek(GdaaDecoder* decoder, u32 source_sample);
u8 gdaa_next_sample(GdaaDecoder* decoder, s16* output);
u32 gdaa_decode(GdaaDecoder* decoder, s16* output, u32 sample_count);
s8 gdaa_pcm16_to_pcm8(s16 sample);
#endif
