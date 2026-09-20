#include "decomp.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/episode.h"
#include "globals.h"

extern i32 LevFlag[4];

i32 KillBoss(i32 character_id, i32 flag_index, float delay) {
    u8 *flags = reinterpret_cast<u8 *>(LevFlag);
    switch (flags[flag_index]) {
    case 0:
        if (BossKilled(character_id) != NULL) {
            if (delay <= 0.0f) {
                flags[flag_index] = 2;
                return 1;
            }
            flags[flag_index] = 1;
        }
        break;
    case 1:
        LevTime[0] += FRAMETIME;
        if (LevTime[0] >= delay) {
            flags[flag_index] = 2;
            return 1;
        }
        break;
    }
    return 0;
}

void KillBossNewLevel(i32 a, i32 b, float c, i32 d) {
    if (KillBoss(a, b, c) != 0) {
        GoToNewLevel(d);
    }
}

i32 KillBossPlayCutScene(i32 a, i32 b, float c, char *name) {
    if (KillBoss(a, b, c) != 0) {
        NewCutScene(NULL, (CUTSYS *)WORLD->cutscene_sys, name, 1);
        return 1;
    }
    return 0;
}

void KillBossCompleteLevel(i32 a, i32 b, float c) {
    if (KillBoss(a, b, c) != 0) {
        CompleteLevel(WORLD);
    }
}
