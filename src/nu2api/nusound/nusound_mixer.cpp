#include "decomp.h"
#include "nu2api_nusound_types.h"

u8 NuSoundMixer::sDownmixerChannelMaps[4][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
};

i32 NuSoundMixer::GetOutputIndex(i32 input, i32 output) {
    if (output_layout == OutputLayout::ZERO) {
        return static_cast<i32>(input_config) * output + input;
    }
    return static_cast<i32>(output_config) * input + output;
}

void NuSoundMixer::Mix(float *input, float *output) {
    const f32 *input_matrix =
        routing_table->GetMatrix(input_config, static_cast<NuSoundSystem::ChannelConfig>(8))->matrix;
    const f32 *output_matrix =
        routing_table->GetMatrix(static_cast<NuSoundSystem::ChannelConfig>(8), output_config)->matrix;
    const u8 *downmix_channels = sDownmixerChannelMaps[static_cast<u32>(downmix_type)];

    memset(output, 0, static_cast<i32>(input_config) * static_cast<i32>(output_config) * sizeof(f32));
    for (i32 i = 0; i < static_cast<i32>(input_config); ++i) {
        if (static_cast<i32>(output_config) < static_cast<i32>(input_config) && downmix_channels[i] == 0) {
            continue;
        }
        for (i32 j = 0; j < static_cast<i32>(output_config); ++j) {
            f32 mixed = 0.0f;
            for (i32 k = 0; k < 8; ++k) {
                mixed += input_matrix[i + k * static_cast<i32>(input_config)] * input[k] * output_matrix[j * 8 + k];
            }
            f32 &gain = output[GetOutputIndex(i, j)];
            gain = MAX(0.0f, MIN(mixed, 1.0f));
        }
    }
}

NuSoundMixer::NuSoundMixer(NuSoundSystem::ChannelConfig config, NuSoundSystem::ChannelConfig output,
                           NuSoundMixer::OutputLayout layout, NuSoundSystem::DownmixType downmix,
                           NuSoundRoutingTable *table)
    : input_config(config), output_config(output), output_layout(layout), downmix_type(downmix), routing_table(table) {
}

NuSoundMixer::~NuSoundMixer() {
}
