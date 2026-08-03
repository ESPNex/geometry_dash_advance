Hybrid:
- 21 zip = FULL @16384 no clip (already short GBA covers)
- 16 non-zip = CLIPPED @11025, clip to level duration
Clipping method:
 width / BLOCKS_PER_SECOND[Speed] *1000 = estimated ms
 clip_start = source_offset_ms (108000 for Acropolis)
 clip_end = start + estimated + 8000 tail
 ffmpeg -ss start -t duration -> 11025 Hz mono s16le -> GDAA
 Ranges merged across levels using union.
