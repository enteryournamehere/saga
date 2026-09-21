# Matching implementation synthesis

The comparison starts at main commit
`73c5d2e8eaf0370839a71c3704e15977777632c4`. Both binaries use the pinned
Android x86 NDK r8e GCC 4.7 build and the documented objdiff fork at
`08fd60d328046baf5f8246d4a6fb2cb0553aacb7`, with its default comparison settings.
The original ELF SHA256 is
`d864055b1db5cc2ee2c16f7968ed68965b69f262ace6b6bfe43558296981c967`.

| Whole-binary measure | Main | Reconstructed |
| --- | ---: | ---: |
| Overall matching | 46.241400% | 50.109623% |
| Exact functions | 4,796 | 5,292 |
| Exact code bytes | 405,562 | 446,570 |
| Original functions | 13,459 | 13,459 |
| Original code bytes | 4,722,419 | 4,722,419 |

The overall increase is 3.868223 percentage points. Comparisons preserve each
original function's address as well as its name, including the 26 repeated
names. The generated `matching.json` contains the complete comparison and
source ownership map. Individual isolated worker scores can differ from the
integrated binary because shared declarations and compiler-generated helpers
change code generation.

The implementation covers gameplay movement and object callbacks, level logic,
terrain and debris queries, renderer and font routines, audio, touch tasks,
editor menus and serialization, and network lifecycle and replication.
Recovered widths, offsets, inheritance, and managed-pointer lifetimes belong in
the canonical declarations and have target ABI assertions. No handwritten
assembly or score-only compiler attributes were introduced.

Integration also recovers the original `ClassEditor` constructor at `0x53b7a0`,
its 0x70-byte global at `0x134ff20`, and the 0x6c-byte default manipulator at
`0x134ed20`. The constructor's GOT reference identifies that manipulator.
`aidata_version` at `0x664f40` is initialized to 20. The existing shared
`TURNTIME` definition supplies both movement and trench callbacks.

The typed default particle rotation bounds preserve the previous bytes:
`c3b40000` and `43b40000` are -360.0f and 360.0f. All 1,064 bytes of
`nulleffecttype` are identical to the first integrated baseline.

Validation uses target compilation/linking, target clang-tidy, all four
repository tests, and the original-symbol check with LLVM nm. All 13,425
required original text symbols are present, with no extra symbols. The legacy
NDK nm reports the linker's init/fini array boundary symbols as text on both
main and the reconstructed binary; LLVM nm correctly reports them as data.
No symbol exclusions or matching settings were changed for that difference.
No native or WebAssembly execution was used.

This is an implementation and first matching pass, not a claim of complete
matching. Some recovered callbacks still have low scores under their existing
translation-unit settings; their original ownership and registration groups
remain to be reconstructed. The measured DookuC_Reset decline from 99.326% to
83.093% is an instruction-order/register-allocation difference audited against
the original behavior and field offsets.

## Regression follow-up

The follow-up raises the initial synthesis from 50.090286% to 50.109623%:
three original functions improve, with no additional regressions.
`GizmoBlowUpOpponent` rises from 0% to 64.756%, exceeding main's 55.184%.
The original pointer walk, missing-array return, filter ordering, and ordered
distance comparison are restored while retaining the managed forced target.

`theNetwork` belongs to `Network.cpp`: the original
`_GLOBAL__sub_I_Network.cpp` at `0xe2920` loads GOT slot `0x616564`, whose
value is `theNetwork` at `0x133d140`, then constructs it and registers its
destructor. Moving its definition from `globals.cpp` restores that ownership
without changing compilation settings. The affected anonymous initializer
recovers from 87.2% to 99.7%; network initialization rises from 20.870% to
93.565%.

The remaining anonymous-initializer decline, 99.7% to 93.125%, pairs original
`numath_includes.c` code at `0x29098c` with the reconstructed
`MechJumpAutopilotAddon.cpp` helper. Its extra constructor initializes
`s_hashId`, as required by the original optimized Jump initializer at
`0xdf9a0`: the string is at `0x573c1c`, and GOT slot `0x615ef0` identifies
`s_hashId` at `0x12684f0`. This is duplicate pairing and translation-unit
reconstruction debt, not missing original initialization behavior.

`Push_AddGizmos` is the only function declining from exactly 100%, to
99.651%. Its initial count load/test uses EDX instead of EAX; the remaining
instructions match. Natural equivalent source forms did not recover this
register choice or Dooku's instruction scheduling. The correct field layout
and original reload behavior are preserved. Apart from Dooku and the duplicate
initializer pairing, every decline from a previous score of at least 50% is
under 1.8 percentage points.
