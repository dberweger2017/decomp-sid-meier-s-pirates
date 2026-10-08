# Inventory and matching semantics

## Original inventory

The reader accepts thin, little-endian 32-bit ARMv7 Mach-O executable/object files. It checks load-command, section, symbol-string, relocation and data ranges before accessing them. Fat, ELF, ARM64, encrypted and malformed inputs are rejected explicitly.

The linked executable’s named `N_FUN` STABS entries define the inventory. Their ending `N_FUN` records supply sizes; `N_OSO` entries define the 268 original object groups. `N_SO` and `N_SOL` source/header paths remain debug-source hints. Unity files such as `FireIncludeCpp.o` remain one original compilation group. Groups with no function records remain present in reports and the unfiltered tree.

Function IDs hash the object-group ID, exact Darwin symbol name, and original address. Objective-C colons and block symbol names are preserved. Repeated identical tuples get explicit record suffixes and ambiguous status. Missing sizes, overlapping records, unknown mode, invalid sections/ranges, and unaligned mode metadata are recorded as ambiguities rather than dropped. ARM/Thumb mode comes from the associated Mach-O symbol’s `N_ARM_THUMB_DEF` metadata, not guessed from disassembly.

The current verified input recovers all 9,177 records with explicit sizes and mode metadata. The generated inventory is deterministic and checked against `config/inventory-lock.json`. A change in its digest fails configuration until the changed records or boundaries are investigated. The lock is evidence, not permission to adjust counts to silence a failure.

## Byte verification

A candidate is a compiled C, C++, or Objective-C translation unit associated with its original object group. Assembly files and original-byte fallbacks are not game source candidates. Missing symbols remain missing. The candidate boundary is conservatively taken from its next non-local named code symbol or section end; aliases and duplicate candidate definitions require explicit disambiguation. Padding is included rather than silently trimmed to obtain a match. Inferred spans often produce size differences; this can reject an otherwise promising candidate without ever accepting fewer bytes than the full candidate span.

The matching core applies relocations at their exact sites, using proven original addresses. It never masks address bytes. Supported cases:

- 32-bit absolute `ARM_RELOC_VANILLA` pointers, including literal pools.
- ARM `ARM_RELOC_BR24` B/BL and Thumb-2 `ARM_THUMB_RELOC_BR22` B.W/BL, with signed offsets and PC bias.
- `ARM_RELOC_HALF` MOVW/MOVT with following PAIR for both ARM and Thumb encodings.
- Scattered `ARM_RELOC_SECTDIFF` / `ARM_RELOC_LOCAL_SECTDIFF`, and `ARM_RELOC_HALF_SECTDIFF`, with a proven subtractor pair.

Named original definitions resolve exactly; local function placement is tied to its stable original function address. Other local section addresses require exact known symbols or explicit placement maps. Section-name equality alone does not prove equivalent data layout. Duplicate original names resolve within the original object group where possible; unresolved duplicates are not guessed.

Imported ARM branch targets can resolve to a unique original 12-byte absolute symbol stub: `ldr ip, [pc]`, `ldr pc, [ip]`, followed by the lazy-pointer address. The reader checks `LC_DYSYMTAB` indirect indices, section stride/ranges and agreement between the stub and referenced lazy-pointer symbol. These addresses apply only to zero-addend branch relocations; they do not establish runtime imported function-pointer values. Duplicate stubs, unknown stub encodings (including Thumb), absent indirect tables, and mismatched lazy-pointer names remain unresolved. The table layout follows Apple's [Mach-O loader definitions](https://github.com/apple-oss-distributions/cctools/blob/main/include/mach-o/loader.h).

Unnamed constants, veneers, interworking BLX transformations, PC-relative VANILLA, obsolete/prebound relocation forms, malformed pairs, unsupported widths/encodings, and unknown placement remain unresolved.

The formulas follow Apple’s [ARM relocation format](https://github.com/apple-oss-distributions/cctools/blob/main/include/mach-o/arm/reloc.h) and [ld64 ARM relocation interpretation](https://github.com/apple-oss-distributions/ld64/blob/main/src/ld/parsers/macho_relocatable_file.cpp). Synthetic tests include independent MOVW/MOVT objects produced by modern Clang and fixed expected ARM/Thumb instruction words.

`matched` requires complete relocated byte equality, equal mode, unambiguous original/candidate ranges, all relocations resolved, a current source/object/dependency fingerprint, and the compiler validation gate. A raw byte agreement with unresolved relocation semantics stays `unresolved`. A candidate compilation failure deletes its stale object and yields `compile_error`. An original-byte or disassembly view without candidate source stays `missing`.

## Assembly differences and progress

Pinned Capstone 5.0.3 disassembles ARM and Thumb-2. Listings retain every byte, including invalid instructions, padding, and pools. PC-relative literal-load detection is a display heuristic; it does not contribute evidence to byte equality. A pool changed by one byte can never be a verified match.

Rows align mnemonic/operand text without broad address normalization. The browser/terminal show instruction, operand, register, byte and relocation-target differences. Similarity is an instruction/operand sequence score with addresses left visible; it is separate from exact byte verification. It may reach 100% for a comparison whose relocation semantics remain unresolved. Per-function native statuses and aggregate verified measures remain authoritative.

Matched byte counts cover only verified function ranges and avoid double-counting overlaps. Function counts refer to recovered records, including ambiguous records. Missing source and unresolved comparisons have separate counts. Global similarity includes missing candidates as zero; compared similarity averages only candidates with listings. Data allocation matching and conservative image linking gates are implemented separately; the recovered CPVRTString npos allocation verifies at four bytes while full-game linking completion remains zero. See [progress and linking](progress.md).

The objdiff adapter uses the v2 protobuf JSON schema pinned in `toolchain/sources.lock.json`, including string-valued uint64 fields and section-relative item addresses. Its per-item fuzzy percentage carries assembly similarity, not byte evidence. The schema cannot encode all native statuses, relocation reasons, or boundary provenance; use the native report for verification. It is a progress export, not a stock objdiff Mach-O comparison backend.
