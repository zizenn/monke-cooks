#ifndef COOKING
#define COOKING

typedef enum{
  NULL,
  PAN,
  DEEP_FRY,
  OVEN,
  GRILL
} COOK_TYPE;

extern COOK_TYPE currentCookType;

#endif
