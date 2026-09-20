# Game-object addons and callbacks

The Android x86 original provides the evidence for these reconstructions.
Addresses below refer to `res/libTTapp.so`.

`AddGameObject` at `0x46d630` initializes the 64-bit collision identity mask,
creates an addon collection, and attaches a `MechEdgeStopAddon`. It also attaches
`MechAutofireAddon` in vehicle areas and for ten specific character IDs. The
autofire allocation is 0x1c bytes, aligned to four bytes, through `NuMemory`.
The edge-stop allocation uses the global `new` operator.

The autofire constructor at `0x45e580` initializes the `MechAddon` base and
stores `GetCharacterObject()` at offset 0x18. The vtable at `0x66b3a0` contains
the inherited addon callbacks. Its deleting destructor at `0x45e520` frees the
allocation through `NuMemory`. These establish the base class, character field,
allocation size, and class-specific delete operation. The canonical declaration
now asserts the 0x1c size and 0x18 character offset.

`OnProcess` at `0x45e240` checks the character, touch controls, and processing
stage before updating the active gesture controller's button bytes. It reads
the controller through `MechSystems` offset 0x20, consistent with the controller
reconstruction. The controller fields remain in their canonical shared types.

The source-specific O2 entry restores a missing optimization setting. The
original has `_GLOBAL__sub_I_MechAutofireAddon.cpp`, and its `OnProcess`,
destructors, and constructor occupy the contiguous region `0x45e240` through
`0x45e62b`. All use optimized frame-free code; the constructor and destructor
inline `MechAddon` and `NuMechPtr` operations. NDK r8e GCC 4.7 experiments at O2
and O3 produce identical code for these reconstructed methods. Both reproduce
the 341-byte `OnProcess` and 81-byte deleting destructor, including their control
flow. O0 instead produces a 477-byte `OnProcess` and out-of-line base operations.
O2 is the narrower setting consistent with this evidence; the original source
build setting cannot be distinguished from O3 using these methods alone.

The game-object source retains its existing O2 setting. Reconstructed callbacks
cover shield transitions, punch sounds and damage, level-specific reflection
planes, and resetting the hub panel when pausing. They use existing character,
animation, world, audio, camera, and input interfaces. `ShieldCode` has an
unresolved local-call ABI difference: the original receives its object in EAX,
while the standalone reconstruction receives it on the stack. No calling-
convention attributes or assembly were added to force that difference away.

Measured with objdiff-cli 3.8.1 against a linked target built from captured
Bazel commands:

| Function | Baseline | Reconstructed |
| --- | ---: | ---: |
| AddGameObject | 24.173% | 96.558% |
| ShieldCode | 4.330% | 93.608% |
| Punch_HitHold | 15.484% | 99.419% |
| Punch_GetDamage_LSW | 6.032% | 93.810% |
| Punch_HitExtraCode_LSW | 5.455% | 99.922% |
| SurfaceInfo_ExtraReflect | 9.333% | 99.822% |
| PauseGame_ExtraCode | 35.000% | 99.875% |
| MechAutofireAddon constructor | 12.383% | 74.383% |
| MechAutofireAddon destructor | 8.056% | 91.167% |
| MechAutofireAddon deleting destructor | 30.842% | 99.947% |
| MechAutofireAddon::OnProcess | 2.472% | 99.876% |

`ClearGameObjects` remains at 99.923%. These are isolated symbol measurements,
not a whole-binary regression audit. The remaining base-file stubs are unchanged.
