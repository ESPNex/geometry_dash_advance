#ifndef GDA_ADPCM_STREAM_H
#define GDA_ADPCM_STREAM_H
#include "adpcm_audio.h"
#define GDAA_STREAM_BUFFER_SAMPLES 2048u // increased to reduce underrun buzz
void gdaa_stream_init(void);
u8 gdaa_stream_start(const u8* data,u32 data_size,u32 source_offset_ms,u8 loop);
void gdaa_stream_stop(void);
void gdaa_stream_pause(void);
void gdaa_stream_resume(void);
u8 gdaa_stream_is_playing(void);
void gdaa_stream_timer_irq(void);
#endif
