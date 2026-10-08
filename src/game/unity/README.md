# Original unity units

Recover PiratesIncludeCpp*, FireIncludeCpp* and TempIncludeCpp* as their original
compilation groups. Put recovered included sources in meaningful game/engine
directories and include them from the corresponding unity translation unit.
Do not create separate candidate objects for files that originally shared a unit.
The function inventory retains each contributing original source path.

No artificial definitions or original-byte payloads belong in this scaffold.
Add a group's explicit `implemented_functions` entries only for recovered source.
