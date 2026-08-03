#!/usr/bin/env python3
import struct, wave
STEP=(7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,50,55,60,66,73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,371,408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2066,2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,15289,16818,18500,20350,22385,24623,27086,29794,32767)
INDEX=(-1,-1,-1,-1,2,4,6,8,-1,-1,-1,-1,2,4,6,8)
HEADER_SIZE=28
BLOCK_SAMPLES=256
BLOCK_BYTES=132
def read_wav(p):
 import pathlib, wave, struct
 p=str(p)
 with wave.open(p,'rb') as w:
  if (w.getnchannels(),w.getsampwidth(),w.getcomptype())!=(1,2,'NONE'):raise ValueError('need mono 16-bit')
  rate=w.getframerate()
  n=w.getnframes()
  data=w.readframes(n)
  samples=list(struct.unpack(f"<{n}h",data))
  return rate,samples
def _encode_nibble(sample,pred,idx):
 step=STEP[idx]; diff=sample-pred; nib=8 if diff<0 else 0; diff=abs(diff); delta=step>>3
 if diff>=step: nib|=4; diff-=step; delta+=step
 if diff>=step>>1: nib|=2; diff-=step>>1; delta+=step>>1
 if diff>=step>>2: nib|=1; delta+=step>>2
 if nib&8: delta=-delta
 pred=max(-32768,min(32767,pred+delta)); idx=max(0,min(88,idx+INDEX[nib])); return nib,pred,idx
def encode(samples,rate,loop_start=0,loop_end=None):
 import struct
 if loop_end is None: loop_end=len(samples)
 out=bytearray(struct.pack("<4sHHIIIIHH",b"GDAA",1,HEADER_SIZE,rate,len(samples),loop_start,loop_end,256,0))
 for off in range(0,len(samples),256):
  block=samples[off:off+256]; pred=block[0]; idx=0; out+=struct.pack("<hBB",pred,idx,0); nibs=[]
  cur_pred=pred; cur_idx=idx
  for s in block[1:]:
   nib,cur_pred,cur_idx=_encode_nibble(s,cur_pred,cur_idx); nibs.append(nib)
  packed=bytearray()
  for i in range(0,len(nibs),2):
   low=nibs[i]; high=nibs[i+1] if i+1<len(nibs) else 0; packed.append(low|(high<<4))
  packed+=b"\x00"*(128-len(packed))
  out+=packed
 return bytes(out)
def decode(data):
 import struct
 magic,ver,hsize,rate,total,loop_start,loop_end,blk,_=struct.unpack_from("<4sHHIIIIHH",data,0)
 samples=[]; off=HEADER_SIZE
 while off+132<=len(data) and len(samples)<total:
  pred,idx,_=struct.unpack_from("<hBB",data,off); samples.append(pred); cur_pred=pred; cur_idx=idx
  block_data=data[off+4:off+132]
  rem=min(255,total-len(samples))
  for i in range(rem):
   b=block_data[i//2]; nib=(b&0x0F) if i%2==0 else (b>>4); step=STEP[cur_idx]; delta=step>>3
   if nib&4: delta+=step
   if nib&2: delta+=step>>1
   if nib&1: delta+=step>>2
   if nib&8: delta=-delta
   cur_pred=max(-32768,min(32767,cur_pred+delta)); cur_idx=max(0,min(88,cur_idx+INDEX[nib])); samples.append(cur_pred)
  off+=132
 return {"sample_rate":rate,"total":total},samples
