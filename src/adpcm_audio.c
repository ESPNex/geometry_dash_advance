#include "adpcm_audio.h"
#define GDAA_MAGIC_0 'G'
#define GDAA_MAGIC_1 'D'
#define GDAA_MAGIC_2 'A'
#define GDAA_MAGIC_3 'A'
#define GDAA_VERSION 1u
static const s16 ima_step_table[89] = {7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,50,55,60,66,73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,371,408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2066,2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,15289,16818,18500,20350,22385,24623,27086,29794,32767};
static const s8 ima_index_table[16] = {-1,-1,-1,-1,2,4,6,8,-1,-1,-1,-1,2,4,6,8};
static u16 read_u16(const u8* s){return (u16)s[0]|((u16)s[1]<<8);}
static u32 read_u32(const u8* s){return (u32)s[0]|((u32)s[1]<<8)|((u32)s[2]<<16)|((u32)s[3]<<24);}
static s16 clamp_s16(s32 v){if(v<-32768)return -32768; if(v>32767)return 32767; return (s16)v;}
static u8 clamp_index(s16 v){if(v<0)return 0; if(v>88)return 88; return (u8)v;}
static void decode_nibble(GdaaDecoder* d,u8 nib){s32 delta; s16 step=ima_step_table[d->step_index]; delta=step>>3; if(nib&4)delta+=step; if(nib&2)delta+=step>>1; if(nib&1)delta+=step>>2; if(nib&8)delta=-delta; d->predictor=clamp_s16((s32)d->predictor+delta); d->step_index=clamp_index((s16)d->step_index+ima_index_table[nib]);}
static u8 load_block(GdaaDecoder* d,u32 bi){u32 bc=(d->total_samples+GDAA_BLOCK_SAMPLES-1)/GDAA_BLOCK_SAMPLES; if(bi>=bc)return FALSE; d->block=d->data+GDAA_HEADER_BYTES+bi*GDAA_BLOCK_BYTES; d->predictor=(s16)read_u16(d->block); d->step_index=d->block[2]; if(d->step_index>88)return FALSE; d->sample_in_block=0; return TRUE;}
u8 gdaa_open(GdaaDecoder* d,const u8* data,u32 sz){u32 bc,es; if(!d||!data||sz<GDAA_HEADER_BYTES)return FALSE; if(data[0]!=GDAA_MAGIC_0||data[1]!=GDAA_MAGIC_1||data[2]!=GDAA_MAGIC_2||data[3]!=GDAA_MAGIC_3)return FALSE; if(read_u16(data+4)!=GDAA_VERSION||read_u16(data+6)!=GDAA_HEADER_BYTES)return FALSE; if(read_u16(data+24)!=GDAA_BLOCK_SAMPLES||read_u16(data+26)!=0)return FALSE; d->data=data; d->sample_rate=read_u32(data+8); d->total_samples=read_u32(data+12); d->loop_start=read_u32(data+16); d->loop_end=read_u32(data+20); d->loop_enabled=FALSE; d->valid=FALSE; if(!d->sample_rate||!d->total_samples||d->loop_start>=d->loop_end||d->loop_end>d->total_samples)return FALSE; bc=(d->total_samples+GDAA_BLOCK_SAMPLES-1)/GDAA_BLOCK_SAMPLES; es=GDAA_HEADER_BYTES+bc*GDAA_BLOCK_BYTES; if(sz<es)return FALSE; d->valid=TRUE; return gdaa_seek(d,0);}
u8 gdaa_seek(GdaaDecoder* d,u32 samp){u32 bi; u16 disc; if(!d||!d->valid||samp>=d->total_samples)return FALSE; bi=samp/GDAA_BLOCK_SAMPLES; disc=(u16)(samp%GDAA_BLOCK_SAMPLES); if(!load_block(d,bi))return FALSE; d->sample_position=bi*GDAA_BLOCK_SAMPLES; while(disc--){s16 ign; if(!gdaa_next_sample(d,&ign))return FALSE;} return TRUE;}
u8 gdaa_next_sample(GdaaDecoder* d,s16* out){u32 bs; u16 np; u8 pack; if(!d||!out||!d->valid)return FALSE; if(d->sample_position>=d->total_samples||(d->loop_enabled&&d->sample_position>=d->loop_end)){if(!d->loop_enabled)return FALSE; if(!gdaa_seek(d,d->loop_start))return FALSE;} if(d->sample_in_block!=0){np=d->sample_in_block-1; pack=d->block[GDAA_BLOCK_HEADER_BYTES+(np>>1)]; decode_nibble(d,(np&1)?(pack>>4):(pack&15));} *out=d->predictor; d->sample_position++; d->sample_in_block++; bs=d->total_samples-((d->sample_position-1)/GDAA_BLOCK_SAMPLES)*GDAA_BLOCK_SAMPLES; if(bs>GDAA_BLOCK_SAMPLES)bs=GDAA_BLOCK_SAMPLES; if(d->sample_in_block>=bs&&d->sample_position<d->total_samples){if(!load_block(d,d->sample_position/GDAA_BLOCK_SAMPLES)){d->valid=FALSE; return FALSE;}} return TRUE;}
u32 gdaa_decode(GdaaDecoder* d,s16* out,u32 cnt){u32 w=0; while(w<cnt&&gdaa_next_sample(d,out+w))w++; return w;}
s8 gdaa_pcm16_to_pcm8(s16 s){return (s8)(s>>8);}
