# First source batch

The batch adds 14 verified functions (544 bytes) to the initial two, for 16 functions and 628 bytes. It also keeps five differing candidates visible for flag and scheduling investigation. All candidates are readable C++ with minimal ABI declarations; original bytes and assembly are not source fallbacks. Original compilation groups remain separate and partial.

The five PVRShell callbacks return true. Quaternion identity writes (0, 0, 0, 1). The vector and matrix constructors copy the original float fields. Rotation wrappers call the original named matrix rotation functions; their branch relocations are resolved to the original addresses before byte equality. The Mat3 wrappers copy the upper 3x3 after rotation. Constructor entries refer to the separately emitted original constructor variants selected by the manifest.

All groups use investigation flags `-O2 -marm`. The PVRTVector group additionally uses `-mfloat-abi=soft`, which makes float copies use integer load/store instructions matching this unit. This is evidence for these functions, not proof of the original flags throughout the unit or game. `softfp`, Cortex-A8 tuning, NEON selection, disabling scheduling, and O3 were investigated; none verified the five remaining candidates. The matrix multiplication wrapper still has a scheduling difference. Translation/scaling and vector interpolation remain different; no register/address masking grants them equality.

| ID | Original symbol | Bytes | Verified status |
|---|---|---:|---|
| `f-62cd60b098a1a0381e31` | `__ZN12PVRShellInit6ApiSetE15prefNameIntEnumi` | 8 | matched |
| `f-f9f0d05fcf913b8814d3` | `__ZN8PVRShell15InitApplicationEv` | 8 | matched |
| `f-73b9a5b8e4e3bc76fa64` | `__ZN8PVRShell15QuitApplicationEv` | 8 | matched |
| `f-67a9f7b44c4b3574d114` | `__ZN8PVRShell8InitViewEv` | 8 | matched |
| `f-3056ddf4a0547696fde0` | `__ZN8PVRShell11ReleaseViewEv` | 8 | matched |
| `f-fac033c0e8e10d664bde` | `__ZN8PVRShell11RenderSceneEv` | 8 | matched |
| `f-bdec2b4851a7bb623546` | `__Z29PVRTMatrixQuaternionIdentityFR15PVRTQUATERNIONf` | 28 | matched |
| `f-da79232dc420a34aba1b` | `__Z19PVRTMatrixIdentityFR11PVRTMATRIXf` | 76 | matched |
| `f-248465017e5b190a6081` | `__Z22PVRTMatrixTranslationFR11PVRTMATRIXffff` | 76 | different |
| `f-a95d54daa8058c95939d` | `__Z18PVRTMatrixScalingFR11PVRTMATRIXffff` | 76 | different |
| `f-1024ba705cf4e96f3a55` | `__Z19PVRTMatrixVec3LerpFR12PVRTVECTOR3fRKS_S2_f` | 84 | different |
| `f-8594ffda5a7ad6b10e37` | `__Z19PVRTMatrixMultiplyFR11PVRTMATRIXfRKS_S2_` | 20 | different |
| `f-a84e36effead63a93467` | `__ZN8PVRTVec3C1ERK8PVRTVec4` | 28 | matched |
| `f-0c2386105dbe1e990fd8` | `__ZN8PVRTMat3C1ERK8PVRTMat4` | 76 | matched |
| `f-f89e4e0ef9c4d7cf9d12` | `__ZNK8PVRTMat4mlERKS_` | 28 | different |
| `f-b5deb57ef9de286dea43` | `__ZN8PVRTMat49RotationZEf` | 16 | matched |
| `f-a59fafdae59489dbb624` | `__ZN8PVRTMat39RotationZEf` | 108 | matched |
| `f-f7a0c585d45fe03d53ce` | `__ZN8PVRTMat49RotationYEf` | 16 | matched |
| `f-67421388b6ffd3a3aada` | `__ZN8PVRTMat39RotationYEf` | 108 | matched |
| `f-bbd56f8df54ca10f3092` | `__ZN8PVRTMat49RotationXEf` | 16 | matched |
| `f-6626bdd50217f9d3e16e` | `__ZN8PVRTMat39RotationXEf` | 108 | matched |

Reproduce with `python configure.py`, `ninja`, and `python tools/dev.py diff <id>`. These functions contain no newly recovered global data. Full-game linking progress remains independent.
