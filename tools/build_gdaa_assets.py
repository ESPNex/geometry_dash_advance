#!/usr/bin/env python3
import json, pathlib, hashlib, sys
ROOT=pathlib.Path(__file__).resolve().parents[1]
WAVDIR=pathlib.Path("/usr/gdaa_wav")
OUT=pathlib.Path("/usr/assets")
OUT.mkdir(parents=True, exist_ok=True)
sys.path.insert(0,str(ROOT/"tools"))
import encode_ima_adpcm as enc
def sha256(p):
    import hashlib; h=hashlib.sha256(); 
    with p.open('rb') as f:
        for c in iter(lambda: f.read(1024*1024), b''): h.update(c)
    return h.hexdigest()
custom=json.loads((ROOT/"audio"/"custom_level_source_tracks.json").read_text())
main=json.loads((ROOT/"audio"/"main_level_source_tracks.json").read_text())
assets={}
for lvl in custom["levels"]:
    k=lvl.get("asset_key")
    if k: assets.setdefault(k,{"asset_key":k,"source_song":lvl.get("source_song"),"levels":[]})["levels"].append(lvl["level"])
for a in main["assets"]:
    k=a["asset_key"]; assets.setdefault(k,{"asset_key":k,"source_song":a.get("source_song"),"levels":[]})["levels"].extend(a.get("levels",[]))
for wav in WAVDIR.glob("*.wav"):
    k=wav.stem; assets.setdefault(k,{"asset_key":k,"source_song":k,"levels":[]})
print(f"Total unique assets required: {len(assets)}")
output=[]
missing=[]
for key in sorted(assets.keys()):
    wav=WAVDIR/f"{key}.wav"
    if not wav.exists(): missing.append(key); continue
    rate,samples=enc.read_wav(wav)
    data=enc.encode(samples,rate)
    out_path=OUT/f"{key}.gdaa"
    out_path.write_bytes(data)
    hdr,dec=enc.decode(data)
    entry={"asset_key":key,"source_song":assets[key]["source_song"],"levels":assets[key]["levels"],"generated_file":str(out_path),"generated_sha256":sha256(out_path),"sample_rate":rate,"duration_ms":round(len(samples)*1000/rate),"pcm_samples":len(samples),"encoded_bytes":len(data)}
    output.append(entry)
    print(f"encoded {key}: {len(data)} bytes, {entry['duration_ms']} ms")
manifest={"format":"GDAA v1 IMA ADPCM","sample_rate_target":16384,"assets":output,"missing":missing}
(OUT/"gdaa_assets.json").write_text(json.dumps(manifest,indent=2)+"\n")
print(f"wrote {OUT/'gdaa_assets.json'} missing {missing}")
