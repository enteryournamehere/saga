#include "decomp.h"
#include "gamelib_util_types.h"
#include "gamelib/util/Utilities.h"

void NetSmallStats::Draw(float, float, float, float, NetSmallStats::eInfo) const {
    STUBBED();
}

void NetSample::Reset() {
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
}

void NetSample::operator+=(NetSample const &other) {
    values[0] += other.values[0];
    values[1] += other.values[1];
    values[2] += other.values[2];
    values[3] += other.values[3];
}

void NetSample::operator-=(NetSample const &other) {
    values[0] -= other.values[0];
    values[1] -= other.values[1];
    values[2] -= other.values[2];
    values[3] -= other.values[3];
}

void NetSample::Max(NetSample const &other) {
    values[0] = values[0] < other.values[0] ? other.values[0] : values[0];
    values[1] = values[1] < other.values[1] ? other.values[1] : values[1];
    values[2] = values[2] < other.values[2] ? other.values[2] : values[2];
    values[3] = values[3] < other.values[3] ? other.values[3] : values[3];
}

void NetStats::Draw(float, float, float, float, NetSmallStats::eInfo) const {
    STUBBED();
}

void NetStats::Update() {
    if (UtilGetFrameStartTime() - sample_time > 1000) {
        i32 next_sample = sample_index + 1;
        if (next_sample >= 30) {
            next_sample = 0;
        }
        samples[next_sample] = total;
        samples[next_sample] -= previous;
        maximum.Max(samples[next_sample]);
        sample_index = next_sample;
        previous = total;
        sample_time = UtilGetFrameStartTime();
    }
}
