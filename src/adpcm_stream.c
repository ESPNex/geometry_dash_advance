#include <tonc.h>
#include "adpcm_stream.h"
#define DS_MASTER_ENABLE  0x0080u
#define DS_B_VOLUME_100   0x0008u
#define DS_B_RIGHT        0x1000u
#define DS_B_LEFT         0x2000u
#define DS_B_TIMER1       0x4000u
#define DS_B_FIFO_RESET   0x8000u
#define DMA_ENABLE        0x80000000u
#define DMA_START_FIFO    0x30000000u
#define DMA_REPEAT        0x02000000u
#define DMA_32BIT         0x04000000u
#define DMA_DEST_FIXED    0x00400000u
#define TIMER_ENABLE      0x0080u
#define TIMER_CASCADE     0x0004u
#define TIMER_IRQ         0x0040u
#define FIFO_AHEAD_SAMPLES 16u
#define TIMER_RELOAD(samples) ((u16)(0x10000u - (samples)))
static EWRAM_DATA ALIGN4 s8 stream_buffers[2][GDAA_STREAM_BUFFER_SAMPLES];
static GdaaDecoder stream_decoder;
static volatile u8 stream_playing;
static volatile u8 stream_paused;
static u8 dma_buffer_index;
static void decode_buffer(u8 index){u32 s; s16 pcm; for(s=0;s<GDAA_STREAM_BUFFER_SAMPLES;s++){if(gdaa_next_sample(&stream_decoder,&pcm)){stream_buffers[index][s]=gdaa_pcm16_to_pcm8(pcm);}else{stream_buffers[index][s]=0;}}}
static void arm_dma2(u8 index){REG_DMA2CNT=0; REG_DMA2SAD=(u32)stream_buffers[index]; REG_DMA2DAD=(u32)&REG_FIFOB; REG_DMA2CNT=DMA_ENABLE|DMA_START_FIFO|DMA_REPEAT|DMA_32BIT|DMA_DEST_FIXED;}
void gdaa_stream_timer_irq(void){u8 next,rec; if(!stream_playing||stream_paused)return; next=dma_buffer_index^1; rec=dma_buffer_index; arm_dma2(next); dma_buffer_index=next; decode_buffer(rec); REG_TM2D=TIMER_RELOAD(GDAA_STREAM_BUFFER_SAMPLES);}
void gdaa_stream_init(void){REG_SNDSTAT|=DS_MASTER_ENABLE; REG_SNDDSCNT|=DS_B_VOLUME_100|DS_B_RIGHT|DS_B_LEFT|DS_B_TIMER1|DS_B_FIFO_RESET; REG_DMA2CNT=0; REG_TM1CNT=0; REG_TM2CNT=0; irq_add(II_TM2,gdaa_stream_timer_irq); irq_enable(II_TM2);}
u8 gdaa_stream_start(const u8* data,u32 sz,u32 off_ms,u8 loop){u32 samp; gdaa_stream_stop(); if(!gdaa_open(&stream_decoder,data,sz))return FALSE; if(stream_decoder.sample_rate!=GDAA_SAMPLE_RATE)return FALSE; samp=(off_ms*stream_decoder.sample_rate)/1000u; if(samp>=stream_decoder.total_samples||!gdaa_seek(&stream_decoder,samp))return FALSE; stream_decoder.loop_enabled=loop?TRUE:FALSE; decode_buffer(0); decode_buffer(1); dma_buffer_index=0; stream_paused=FALSE; stream_playing=TRUE; REG_SNDDSCNT|=DS_B_FIFO_RESET; arm_dma2(0); REG_TM2D=TIMER_RELOAD(GDAA_STREAM_BUFFER_SAMPLES-FIFO_AHEAD_SAMPLES); REG_TM2CNT=TIMER_ENABLE|TIMER_CASCADE|TIMER_IRQ; REG_TM1D=(u16)(0x10000u-(0x1000000u/GDAA_SAMPLE_RATE)); REG_TM1CNT=TIMER_ENABLE; return TRUE;}
void gdaa_stream_stop(void){REG_TM2CNT=0; REG_TM1CNT=0; REG_DMA2CNT=0; REG_SNDDSCNT|=DS_B_FIFO_RESET; stream_playing=FALSE; stream_paused=FALSE;}
void gdaa_stream_pause(void){if(!stream_playing||stream_paused)return; REG_TM2CNT=0; REG_TM1CNT=0; REG_DMA2CNT=0; stream_paused=TRUE;}
void gdaa_stream_resume(void){if(!stream_playing||!stream_paused)return; decode_buffer(dma_buffer_index); arm_dma2(dma_buffer_index); REG_TM2D=TIMER_RELOAD(GDAA_STREAM_BUFFER_SAMPLES-FIFO_AHEAD_SAMPLES); REG_TM2CNT=TIMER_ENABLE|TIMER_CASCADE|TIMER_IRQ; REG_TM1CNT=TIMER_ENABLE; stream_paused=FALSE;}
u8 gdaa_stream_is_playing(void){return stream_playing&&!stream_paused;}
