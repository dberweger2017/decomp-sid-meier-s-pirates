// Partial method-only interfaces; class hierarchy, ivars and runtime metadata
// are not recovered or credited. Do not instantiate these partial classes.
// Method encodings below are independently read from the original runtime lists.
typedef signed char PiratesBOOL;

@interface AchievementView
// f-53c73be93d5e99c73017 — original encoding c12@0:4i8
- (PiratesBOOL)shouldAutorotateToInterfaceOrientation:(int)orientation;
@end

@implementation AchievementView
- (PiratesBOOL)shouldAutorotateToInterfaceOrientation:(int)orientation { return 1; }
@end
