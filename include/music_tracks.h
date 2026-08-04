#ifndef MUSIC_TRACKS_H
#define MUSIC_TRACKS_H
#include "gdaa_assets.h"
typedef struct {
    GdaaTrackId asset;
    u32 offset_ms;
    u8 loop;
} MusicTrackMapping;
extern const MusicTrackMapping level_music_map[];
extern const u32 level_music_map_count;
GdaaTrackId music_get_by_level_id(u16 level_id, u32* out_offset);
static inline GdaaTrackId music_get_by_key(const char* k){return GDAA_TRACK_INVALID;}
#endif
