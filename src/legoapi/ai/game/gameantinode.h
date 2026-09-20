#pragma once

#include "nu2api/nucore/fixed_width.h"

struct GAMEANTINODE_s;
struct GAMEANTINODESYS_s;
struct GAMEANTINODEDATA_s;
struct nuvec_s;

GAMEANTINODE_s *GameAntinode_RegisterAntiNode(GAMEANTINODESYS_s *system, nuvec_s *position, float radius, float extent_x,
                                              float extent_z, u16 angle, i32 shape, float duration);
void GameAntinode_UnregisterAntiNode(GAMEANTINODESYS_s *system, GAMEANTINODE_s *node);

GAMEANTINODE_s *GameAntinode_RegisterAntiNodeUsingData(GAMEANTINODESYS_s *system, nuvec_s *position, u16 angle,
                                                       GAMEANTINODEDATA_s *data, float duration, i32 disabled);
