#!/usr/bin/env python3
"""Build clipped GDAA assets at 11025 Hz to fit GBA ROM 32MB"""
import json, pathlib, subprocess, sys
ROOT = pathlib.Path(__file__).resolve().parents[1]
WAVDIR = pathlib.Path("/usr/gdaa_wav")
CLIP_PLAN = json.loads((ROOT/"audio"/"level_clip_plan.json").read_text())
OUTDIR = pathlib.Path("/usr/assets_clipped_11025")
OUTDIR.mkdir(parents=True, exist_ok=True)

sys.path.insert(0, str(ROOT/"tools"))
import encode_ima_adpcm as enc

TARGET_RATE = 11025

def sha256(p):
    import hashlib; h=hashlib.sha256()
    with p.open('rb') as f:
        for c in iter(lambda: f.read(1024*1024), b''): h.update(c)
    return h.hexdigest()

manifest_assets=[]
for entry in CLIP_PLAN["asset_ranges"]:
    key = entry["asset_key"]
    start_ms = entry["clip_start_ms"]
    end_ms = entry["clip_end_ms"]
    wav_src = WAVDIR / f"{key}.wav"
    if not wav_src.exists():
        print(f"MISSING SRC {key}")
        continue
    # temporary clipped wav at 16384? We'll clip at source rate then resample
    # Use ffmpeg to seek and duration
    clip_duration_ms = end_ms - start_ms
    tmp_clip = OUTDIR / f"{key}_tmp.wav"
    # clip using ffmpeg: -ss start_ms/1000 -t duration_ms/1000
    cmd = [
        "ffmpeg","-hide_banner","-loglevel","error","-y",
        "-ss", f"{start_ms/1000:.3f}",
        "-i", str(wav_src),
        "-t", f"{clip_duration_ms/1000:.3f}",
        "-ac","1","-ar",str(TARGET_RATE),"-c:a","pcm_s16le",
        str(tmp_clip)
    ]
    try:
        subprocess.run(cmd, check=True)
    except Exception as e:
        print(f"ffmpeg clip fail {key}: {e}")
        continue
    # now encode
    try:
        rate,samples = enc.read_wav(tmp_clip)
    except Exception as e:
        print(f"read_wav fail {key}: {e}")
        continue
    data = enc.encode(samples, rate)
    out_path = OUTDIR / f"{key}.gdaa"
    out_path.write_bytes(data)
    tmp_clip.unlink()
    asset_entry = {
        "asset_key": key,
        "clip_start_ms": start_ms,
        "clip_end_ms": end_ms,
        "duration_ms": round(len(samples)*1000/rate),
        "sample_rate": rate,
        "encoded_bytes": len(data),
        "levels": entry["levels"],
        "sha256": sha256(out_path)
    }
    manifest_assets.append(asset_entry)
    print(f"clipped {key}: {start_ms}->{end_ms} ({clip_duration_ms}ms) -> {len(data)} bytes {len(samples)} samples")

total = sum(a["encoded_bytes"] for a in manifest_assets)
print(f"\nTOTAL {len(manifest_assets)} assets, {total} bytes = {total/1024/1024:.2f} MB")
(OUTDIR/"gdaa_clip_manifest.json").write_text(json.dumps({"sample_rate":TARGET_RATE,"assets":manifest_assets,"total_bytes":total},indent=2)+"\n")

