// Partial by-value visitor interface; node implementation and traversal stay missing.
class NiAVObject;
struct _SetAlphaTestForNoSorterObjects { unsigned char alpha; };
template<class Visitor> void ForEachNiNode(NiAVObject *, Visitor);
void SetAlphaTestForNoSorterObjects(NiAVObject *node, unsigned char alpha) {
    _SetAlphaTestForNoSorterObjects visitor = {alpha};
    ForEachNiNode(node, visitor);
}
