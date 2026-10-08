#pragma once

class FStringA;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FStringTable {
public:
    void OnGenerateHashTable(const FStringA *strings);
    void OnLoadStrings(const FStringA *strings);
    void OnWriteStrings(const FStringA *strings, unsigned int count);
};
