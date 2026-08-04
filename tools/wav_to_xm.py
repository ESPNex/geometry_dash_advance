#!/usr/bin/env python3
"""
WAV to XM converter ported from AutoIt WAV2XM
- Input: PCM WAV 8/16/24/32 mono/stereo
- Output: XM 1.04, 1 instrument, 1 sample 44100Hz 16-bit mono, delta encoded
- Relative note 29 => 8363*2^(29/12) ~= 44100
- Patterns: 125 BPM, 6 tempo, 0.12s per row, multiple patterns if needed
"""
import wave, struct, math, pathlib, sys

TARGET_RATE = 44100
XM_TEMPO = 6
XM_BPM = 125
XM_CHANNELS = 2
XM_MAX_ROWS = 256

def le16(v):
    return struct.pack("<H", v & 0xFFFF)
def le32(v):
    return struct.pack("<I", v & 0xFFFFFFFF)
def le16_signed(v):
    if v<0:
        v+=65536
    return struct.pack("<H", v & 0xFFFF)

def fixed_string(s, length):
    b=s.encode('ascii','ignore')[:length]
    return b + b'\x00'*(length-len(b))

def zeros(n):
    return b'\x00'*n

def read_wav(wav_path):
    with wave.open(str(wav_path),'rb') as w:
        rate=w.getframerate()
        ch=w.getnchannels()
        sampwidth=w.getsampwidth()
        n=w.getnframes()
        data=w.readframes(n)
        if sampwidth==1:
            samples=[]
            for i in range(n*ch):
                b=data[i]
                s=(b-128)*256
                samples.append(s)
            if ch==2:
                mono=[]
                for i in range(0,len(samples),2):
                    mono.append(int((samples[i]+samples[i+1])/2))
                samples=mono
        elif sampwidth==2:
            fmt=f"<{n*ch}h"
            samples=list(struct.unpack(fmt, data))
            if ch==2:
                mono=[]
                for i in range(0,len(samples),2):
                    mono.append(int((samples[i]+samples[i+1])/2))
                samples=mono
        elif sampwidth==3:
            samples=[]
            for i in range(n):
                frame=[]
                for c in range(ch):
                    off=(i*ch+c)*3
                    b0=data[off]
                    b1=data[off+1]
                    b2=data[off+2]
                    val=b0 | (b1<<8) | (b2<<16)
                    if val & 0x800000:
                        val-=0x1000000
                    val=int(val/256)
                    frame.append(val)
                if ch==2:
                    samples.append(int((frame[0]+frame[1])/2))
                else:
                    samples.append(frame[0])
        elif sampwidth==4:
            fmt=f"<{n*ch}i"
            raw=list(struct.unpack(fmt, data))
            samples=[]
            for i in range(n):
                if ch==2:
                    samples.append(int((raw[i*2]+raw[i*2+1])/2/256))
                else:
                    samples.append(int(raw[i]/256))
        else:
            raise ValueError(f"Unsupported sampwidth {sampwidth}")
        return rate, samples

def wav_to_xm(wav_path, xm_path):
    wav_path=pathlib.Path(wav_path)
    xm_path=pathlib.Path(xm_path)
    rate, samples = read_wav(wav_path)
    source_frames=len(samples)
    duration=source_frames/rate
    if rate==TARGET_RATE:
        output_frames=source_frames
        resampled=samples
    else:
        output_frames=int((source_frames-1)*TARGET_RATE/rate)+1 if source_frames>1 else 1
        step=rate/TARGET_RATE
        pos=0.0
        resampled=[]
        for _ in range(output_frames):
            idx=int(pos)
            if idx>=source_frames:
                idx=source_frames-1
            resampled.append(samples[idx])
            pos+=step
    row_seconds=(2.5/XM_BPM)*XM_TEMPO
    output_duration=output_frames/TARGET_RATE
    required_rows=math.ceil(output_duration/row_seconds)
    if required_rows<1:
        required_rows=1
    pattern_count=math.ceil(required_rows/XM_MAX_ROWS)
    if pattern_count>256:
        raise ValueError("Source too long")
    print(f"Source {rate}Hz {source_frames} frames {duration:.2f}s -> {TARGET_RATE}Hz {output_frames} frames {output_duration:.2f}s, rows {required_rows}, patterns {pattern_count}")

    with open(xm_path,'wb') as out:
        out.write(b'Extended Module: ')
        out.write(fixed_string(wav_path.stem,20))
        out.write(b'\x1a')
        out.write(fixed_string("AutoIt WAV2XM",20))
        out.write(le16(0x0104))
        out.write(le32(276))
        out.write(le16(pattern_count))
        out.write(le16(0))
        out.write(le16(XM_CHANNELS))
        out.write(le16(pattern_count))
        out.write(le16(1))
        out.write(le16(1))
        out.write(le16(XM_TEMPO))
        out.write(le16(XM_BPM))
        for i in range(256):
            if i<pattern_count:
                out.write(bytes([i]))
            else:
                out.write(b'\x00')
        remaining=required_rows
        for pat in range(pattern_count):
            rows=min(XM_MAX_ROWS, remaining)
            if rows<1:
                rows=1
            packed_size=rows*XM_CHANNELS*5
            out.write(le32(9))
            out.write(bytes([0]))
            out.write(le16(rows))
            out.write(le16(packed_size))
            for r in range(rows):
                for ch in range(XM_CHANNELS):
                    if pat==0 and r==0 and ch==0:
                        out.write(bytes([49,1,0,0,0]))
                    else:
                        out.write(zeros(5))
            remaining-=rows
        out.write(le32(263))
        out.write(fixed_string("WAV Audio Instrument",22))
        out.write(bytes([0]))
        out.write(le16(1))
        out.write(le32(40))
        out.write(zeros(96))
        out.write(zeros(48))
        out.write(zeros(48))
        out.write(bytes([0,0,0,0,0,0,0,0,0,0,0,0]))
        out.write(le16(0))
        out.write(zeros(22))
        sample_bytes=output_frames*2
        out.write(le32(sample_bytes))
        out.write(le32(0))
        out.write(le32(0))
        out.write(bytes([64]))
        out.write(bytes([0]))
        out.write(bytes([16]))
        out.write(bytes([128]))
        out.write(bytes([29]))
        out.write(bytes([0]))
        out.write(fixed_string("44100Hz 16-bit WAV",22))
        prev=0
        for s in resampled:
            if s<-32768:
                s=-32768
            if s>32767:
                s=32767
            delta=s-prev
            if delta<-32768:
                delta=-32768
            if delta>32767:
                delta=32767
            out.write(le16_signed(delta))
            prev=s
    print(f"Wrote {xm_path} {xm_path.stat().st_size} bytes")

if __name__=="__main__":
    if len(sys.argv)<3:
        print("Usage: wav_to_xm.py input.wav output.xm")
        sys.exit(1)
    wav_to_xm(sys.argv[1], sys.argv[2])
