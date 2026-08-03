#include "gdaa_assets.h"
#include "music_tracks.h"

#define LEVEL_COUNT 54

const MusicTrackMapping level_music_map[LEVEL_COUNT] = {
    [0] = { .asset = GDAA_TRACK_STEREO_MADNESS, .offset_ms = 0, .loop = 0 }, // id 0
    [1] = { .asset = GDAA_TRACK_BACK_ON_TRACK, .offset_ms = 0, .loop = 0 }, // id 1
    [2] = { .asset = GDAA_TRACK_POLARGEIST, .offset_ms = 0, .loop = 0 }, // id 2
    [3] = { .asset = GDAA_TRACK_DRY_OUT, .offset_ms = 0, .loop = 0 }, // id 3
    [4] = { .asset = GDAA_TRACK_BASE_AFTER_BASE, .offset_ms = 0, .loop = 0 }, // id 4
    [5] = { .asset = GDAA_TRACK_CANT_LET_GO, .offset_ms = 0, .loop = 0 }, // id 5
    [6] = { .asset = GDAA_TRACK_JUMPER, .offset_ms = 0, .loop = 0 }, // id 6
    [7] = { .asset = GDAA_TRACK_TIME_MACHINE, .offset_ms = 0, .loop = 0 }, // id 7
    [8] = { .asset = GDAA_TRACK_CYCLES, .offset_ms = 0, .loop = 0 }, // id 8
    [9] = { .asset = GDAA_TRACK_XSTEP, .offset_ms = 0, .loop = 0 }, // id 9
    [10] = { .asset = GDAA_TRACK_CLUTTERFUNK, .offset_ms = 0, .loop = 0 }, // id 10
    [11] = { .asset = GDAA_TRACK_THEORY_OF_EVERYTHING, .offset_ms = 0, .loop = 0 }, // id 11
    [12] = { .asset = GDAA_TRACK_ELECTROMAN_ADVENTURES, .offset_ms = 0, .loop = 0 }, // id 12
    [13] = { .asset = GDAA_TRACK_CLUBSTEP, .offset_ms = 0, .loop = 0 }, // id 13
    [14] = { .asset = GDAA_TRACK_ELECTRODYNAMIX, .offset_ms = 0, .loop = 0 }, // id 14
    [15] = { .asset = GDAA_TRACK_HEXAGON_FORCE, .offset_ms = 0, .loop = 0 }, // id 15
    [16] = { .asset = GDAA_TRACK_BLAST_PROCESSING, .offset_ms = 0, .loop = 0 }, // id 16
    [17] = { .asset = GDAA_TRACK_THEORY_OF_EVERYTHING_2, .offset_ms = 0, .loop = 0 }, // id 17
    [18] = { .asset = GDAA_TRACK_CYCLES, .offset_ms = 0, .loop = 0 }, // id 18
    [19] = { .asset = GDAA_TRACK_STEREO_MADNESS_2, .offset_ms = 0, .loop = 0 }, // id 19
    [20] = { .asset = GDAA_TRACK_GOLDEN_HAZE, .offset_ms = 0, .loop = 0 }, // id 20
    [21] = { .asset = GDAA_TRACK_BACK_ON_TRACK, .offset_ms = 0, .loop = 0 }, // id 21
    [22] = { .asset = GDAA_TRACK_STEREO_MADNESS, .offset_ms = 0, .loop = 0 }, // id 22
    [23] = { .asset = GDAA_TRACK_ELECTROMAN_ADVENTURES, .offset_ms = 0, .loop = 0 }, // id 23
    [24] = { .asset = GDAA_TRACK_BASH, .offset_ms = 0, .loop = 0 }, // id 24
    [25] = { .asset = GDAA_TRACK_GOLDEN_HAZE, .offset_ms = 0, .loop = 0 }, // id 25
    [26] = { .asset = GDAA_TRACK_CLOWNPARTY_REMIX, .offset_ms = 0, .loop = 0 }, // id 26
    [27] = { .asset = GDAA_TRACK_CLUBSTEP, .offset_ms = 0, .loop = 0 }, // id 27
    [28] = { .asset = GDAA_TRACK_STEREO_MADNESS, .offset_ms = 0, .loop = 0 }, // id 28
    [29] = { .asset = GDAA_TRACK_SELECT_PAYMENT_TYPE, .offset_ms = 0, .loop = 0 }, // id 29
    [30] = { .asset = GDAA_TRACK_DEADLOCKED, .offset_ms = 0, .loop = 0 }, // id 30
    [31] = { .asset = GDAA_TRACK_CHAOZ_IMPACT, .offset_ms = 0, .loop = 0 }, // id 31
    [32] = { .asset = GDAA_TRACK_ENIGMA, .offset_ms = 0, .loop = 0 }, // id 32
    [33] = { .asset = GDAA_TRACK_BACK_ON_TRACK, .offset_ms = 0, .loop = 0 }, // id 33
    [34] = { .asset = GDAA_TRACK_JUMPER, .offset_ms = 0, .loop = 0 }, // id 34
    [35] = { .asset = GDAA_TRACK_ULTIMATE_DESTRUCTION, .offset_ms = 0, .loop = 0 }, // id 35
    [36] = { .asset = GDAA_TRACK_STAY_INSIDE_ME, .offset_ms = 0, .loop = 0 }, // id 36
    [37] = { .asset = GDAA_TRACK_CANT_LET_GO, .offset_ms = 0, .loop = 0 }, // id 37
    [38] = { .asset = GDAA_TRACK_JUMPER, .offset_ms = 0, .loop = 0 }, // id 38
    [39] = { .asset = GDAA_TRACK_STEREO_MADNESS, .offset_ms = 0, .loop = 0 }, // id 39
    [40] = { .asset = GDAA_TRACK_INFERNOPLEX, .offset_ms = 0, .loop = 0 }, // id 40
    [41] = { .asset = GDAA_TRACK_FIRE_AURA, .offset_ms = 0, .loop = 0 }, // id 41
    [42] = { .asset = GDAA_TRACK_FIRE_AURA, .offset_ms = 0, .loop = 0 }, // id 42
    [43] = { .asset = GDAA_TRACK_POLARGEIST, .offset_ms = 0, .loop = 0 }, // id 43
    [44] = { .asset = GDAA_TRACK_JUMPER, .offset_ms = 0, .loop = 0 }, // id 44
    [45] = { .asset = GDAA_TRACK_ROSE, .offset_ms = 0, .loop = 0 }, // id 45
    [46] = { .asset = GDAA_TRACK_PROBLEMATIC, .offset_ms = 0, .loop = 0 }, // id 46
    [47] = { .asset = GDAA_TRACK_DEATH_MOON, .offset_ms = 0, .loop = 0 }, // id 47
    [48] = { .asset = GDAA_TRACK_GHOST_OF_JUPITER, .offset_ms = 0, .loop = 0 }, // id 48
    [49] = { .asset = GDAA_TRACK_FINAL_BATTLE, .offset_ms = 0, .loop = 0 }, // id 49
    [50] = { .asset = GDAA_TRACK_EVERY_END, .offset_ms = 0, .loop = 0 }, // id 50
    [51] = { .asset = GDAA_TRACK_STAY_INSIDE_ME, .offset_ms = 0, .loop = 0 }, // id 51
    [52] = { .asset = GDAA_TRACK_STAY_INSIDE_ME, .offset_ms = 0, .loop = 0 }, // id 52
    [53] = { .asset = GDAA_TRACK_STAY_INSIDE_ME, .offset_ms = 0, .loop = 0 }, // id 53
};
const u32 level_music_map_count = LEVEL_COUNT;

GdaaTrackId music_get_by_level_id(u16 level_id, u32* out_offset) {
    if(level_id >= LEVEL_COUNT) { if(out_offset) *out_offset=0; return GDAA_TRACK_INVALID; }
    if(out_offset) *out_offset = level_music_map[level_id].offset_ms;
    return level_music_map[level_id].asset;
}
