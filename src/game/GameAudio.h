#pragma once

// Only the enum tags/32-bit ARM parameter ABI are needed by these entry points.
// Original enumerator names and values have not been recovered. The placeholder
// is a declaration aid, not a recovered sound identifier.
enum AS2D_Type { AS2D_ABIPlaceholder = 0 };
enum AS3D_Type { AS3D_ABIPlaceholder = 0 };

// Init's zero return is observed; bool is a source type hypothesis.
bool GameAudio_Init(const char *path);
void GameAudio_Play(AS2D_Type type, int value);
void GameAudio_PlayIfNoPlaying(AS2D_Type type, int value);
void GameAudio_Play(AS3D_Type type, int value);
void GameAudio_Update();
void GameMusic_Play(AS2D_Type type, int value);
void GameMusic_Stop();
void GameMusic_Stop(AS2D_Type type);
void GameMusic_PlayIfNoPlaying(AS2D_Type type, int value);
