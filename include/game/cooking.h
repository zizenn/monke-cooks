#ifndef COOKING
#define COOKING

typedef enum{
  NULL,
  PAN,
  DEEP_FRY,
  OVEN,
  GRILL
} COOK_TYPE;

typedef enum {
  PREP_NONE,
  CRACK,
  WASH,
  CUT,
  GROUND
} PREP_TYPE;

extern COOK_TYPE currentCookType;
extern PREP_TYPE currentPrepType;

#endif
