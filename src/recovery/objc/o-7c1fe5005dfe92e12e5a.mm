// Partial method-only interfaces; class hierarchy, ivars and runtime metadata
// are not recovered or credited. Do not instantiate these partial classes.
// Method encodings below are independently read from the original runtime lists.
typedef signed char PiratesBOOL;

@interface MyOwnView
// f-95f7b310f2254465dead — original encoding v16@0:4i8i12
- (void)setPriorityOfView:(int)view withPriority:(int)priority;
// f-bdd94f4b5e2387d80566 — original encoding c12@0:4i8
- (PiratesBOOL)shouldAutorotateToInterfaceOrientation:(int)orientation;
@end

@implementation MyOwnView
- (void)setPriorityOfView:(int)view withPriority:(int)priority {  }
- (PiratesBOOL)shouldAutorotateToInterfaceOrientation:(int)orientation { return 1; }
@end
