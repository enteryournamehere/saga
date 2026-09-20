#include "legoapi/world/mission.h"

#include "legoapi/characters/core/character.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

#include <string.h>

MISSIONSYS *MissionSys = NULL;

MISSIONSYS *Missions_Configure(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd, MISSIONSAVE *save) {
    nufpar_s *fp;
    i32 i;
    MISSIONSYS *dest;
    i32 charId;
    MISSIONSYS sys;
    MISSIONDATA *buffer;

    fp = NuFParCreate(file);
    if (fp != NULL) {
        memset(&sys, 0, sizeof(sys));

        sys.flags = 1;
        bufferStart->addr = ALIGN(bufferStart->addr, alignof(MISSIONDATA));
        buffer = static_cast<MISSIONDATA *>(bufferStart->void_ptr);
        sys.missions = buffer;
        sys.mission_save = save;

        while (sys.count < 20 && NuFParGetLine(fp) != 0) {
            NuFParGetWord(fp);

            i = NuStrICmp(fp->word_buf, "party");
            if (i == 0) {
                while ((sys.character_count < 8 && (i = NuFParGetWord(fp), i != 0))) {
                    charId = CharIDFromName(fp->word_buf);
                    if (charId != -1) {
                        sys.character_ids[sys.character_count] = (i16)charId;
                        sys.character_count = sys.character_count + 1;
                    }
                }
            } else {
                i = NuStrICmp(fp->word_buf, "pickups");
                if (i == 0) {
                    i = NuFParGetWord(fp);
                    if (i != 0) {
                        i = NuStrICmp(fp->word_buf, "on");
                        if (i == 0) {
                            sys.flags |= 1;
                        } else {
                            i = NuStrICmp(fp->word_buf, "off");
                            if (i == 0) {
                                sys.flags &= 0xfe;
                            }
                        }
                    }
                } else {
                    i = NuStrICmp(fp->word_buf, "mission");
                    if (i == 0) {
                        buffer->find_char = -1;
                        buffer->level = -1;
                        buffer->name_id = -1;
                        buffer->text_id = -1;
                        buffer->bounty = 0x186a0;
                        buffer->bounty2 = 0x186a0;
                        buffer->time = 0xb4;
                        buffer->count = sys.count;
                        while (NuFParGetWord(fp) != 0) {
                            if (NuStrICmp(fp->word_buf, "find") == 0) {
                                if (NuFParGetWord(fp) != 0) {
                                    buffer->find_char = CharIDFromName(fp->word_buf);
                                }
                            } else if (NuStrICmp(fp->word_buf, "in_level") == 0) {
                                if (NuFParGetWord(fp) != 0 && Level_FindByName(fp->word_buf, &charId) != NULL) {
                                    buffer->level = (i16)charId;
                                }
                            } else if (NuStrICmp(fp->word_buf, "time") == 0) {
                                u16 time = (u16)NuFParGetInt(fp);
                                buffer->time = time < 3 ? 3 : time;
                            } else if (NuStrICmp(fp->word_buf, "bounty") == 0) {
                                buffer->bounty = NuFParGetInt(fp);
                                buffer->bounty2 = NuFParGetInt(fp);
                            } else if (NuStrICmp(fp->word_buf, "name_id") == 0) {
                                buffer->name_id = (i16)NuFParGetInt(fp);
                            } else if (NuStrICmp(fp->word_buf, "text_id") == 0) {
                                buffer->text_id = (i16)NuFParGetInt(fp);
                            }
                        }
                        if (buffer->find_char != -1 && buffer->level != -1) {
                            sys.count = sys.count + 1;
                            ++buffer;
                            bufferStart->addr += sizeof(MISSIONDATA);
                        }
                    }
                }
            }
        }

        NuFParDestroy(fp);
        if (sys.count != 0) {
            bufferStart->addr = ALIGN(bufferStart->addr, alignof(MISSIONSYS));
            dest = static_cast<MISSIONSYS *>(bufferStart->void_ptr);
            memmove(dest, &sys, sizeof(sys));
            bufferStart->addr = ALIGN(bufferStart->addr + sizeof(MISSIONSYS), alignof(MISSIONSYS));
            return dest;
        }
    }

    return NULL;
}
