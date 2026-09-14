#pragma once

struct LEVELDATA_s;

void HostLoadPermImmediately(void (*load_perm)());
void HostEnterLevel(const LEVELDATA_s &level);
