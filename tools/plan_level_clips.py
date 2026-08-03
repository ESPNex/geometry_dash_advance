#!/usr/bin/env python3
"""Build clip plan from level geometry to fit GBA ROM 32MB"""
from __future__ import annotations
import json, pathlib, xml.etree.ElementTree as ET
ROOT = pathlib.Path(__file__).resolve().parents[1]
LEVELS = ROOT / "levels"
OFFSETS = {x["level"]: x for x in json.loads((ROOT / "audio" / "custom_level_offsets.json").read_text())["levels"]}
CUSTOM = {x["level"]: x for x in json.loads((ROOT / "audio" / "custom_level_source_tracks.json").read_text())["levels"]}
SONG_ASSET = {
    "BACKOT": "BACK_ON_TRACK", "BASEAB": "BASE_AFTER_BASE", "BLASTP": "BLAST_PROCESSING",
    "CANTLG": "CANT_LET_GO", "CLUBSTEP": "CLUBSTEP", "CLTRFNK": "CLUTTERFUNK",
    "CYCLES": "CYCLES", "DRY": "DRY_OUT", "DYNAMIX": "ELECTRODYNAMIX",
    "ELECTRO": "ELECTROMAN_ADVENTURES", "HEXAGON": "HEXAGON_FORCE", "JUMPER": "JUMPER",
    "POLAR": "POLARGEIST", "STEREOMA": "STEREO_MADNESS", "STEREOMA2": "STEREO_MADNESS_2",
    "TIME": "TIME_MACHINE", "TOE": "THEORY_OF_EVERYTHING", "TOE2": "THEORY_OF_EVERYTHING_2",
    "XSTEP": "XSTEP", "MENU": "MENU_THEME", "PRACTICE": "STAY_INSIDE_ME",
}
BLOCKS_PER_SECOND = [8.3720028, 10.38600324, 12.9140044, 15.6000052, 19.2000064]
TAIL_MARGIN_MS = 8000

def props(path: pathlib.Path) -> dict[str, str]:
    root = ET.parse(path).getroot()
    return {x.attrib["name"]: x.attrib.get("value", "") for x in root.findall("./properties/property")}

def main():
    records=[]
    for path in sorted(LEVELS.glob("*.tmx")):
        if path.name.startswith('.'): continue
        p=props(path); key=path.stem
        speed=int(p.get("Speed", "1")); speed=max(0,min(speed,len(BLOCKS_PER_SECOND)-1))
        duration_ms=round(int(ET.parse(path).getroot().attrib["width"]) / BLOCKS_PER_SECOND[speed] * 1000)
        custom=CUSTOM.get(key, {})
        asset=custom.get("asset_key") or SONG_ASSET.get(p.get("Song", ""), p.get("Song", "UNKNOWN"))
        offset=OFFSETS.get(key,{}).get("source_offset_ms",0)
        records.append({
            "level":key, "asset_key":asset, "source_offset_ms":offset,
            "estimated_level_duration_ms":duration_ms,
            "clip_start_ms":offset,
            "clip_end_ms":offset+duration_ms+TAIL_MARGIN_MS,
            "validation":"required: speed portals may change estimate"
        })
    asset_ranges={}
    for r in records:
        a=asset_ranges.setdefault(r["asset_key"],{"asset_key":r["asset_key"],"clip_start_ms":r["clip_start_ms"],"clip_end_ms":r["clip_end_ms"],"levels":[]})
        a["clip_start_ms"]=min(a["clip_start_ms"],r["clip_start_ms"])
        a["clip_end_ms"]=max(a["clip_end_ms"],r["clip_end_ms"])
        a["levels"].append(r["level"])
    out=ROOT/"audio"/"level_clip_plan.json"
    out.write_text(json.dumps({"method":"map-width initial-speed estimate plus 8000ms tail margin","levels":records,"asset_ranges":sorted(asset_ranges.values(),key=lambda x:x['asset_key'])},indent=2)+"\n")
    print(f"wrote {out}: {len(records)} levels, {len(asset_ranges)} asset ranges")

if __name__ == '__main__':
    main()
