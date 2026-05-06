#ifndef CORE_ENUMS_H
#define CORE_ENUMS_H

// Cooking methods
typedef enum {
  COOK_NONE = 0,
  PAN,
  DEEP_FRY,
  OVEN,
  GRILL
} COOK_TYPE;

// Preparation methods
typedef enum {
  PREP_NONE,
  CRACK,
  WASH,
  CUT,
  GROUND
} PREP_TYPE;

// Item origin (where item came from)
typedef enum {
  FROM_FRIDGE,
  FROM_PANTRY,
  FROM_NONE
} ItemOrigin;

// Which array/collection the item belongs to
typedef enum {
  ARRAY_FOOD,
  ARRAY_EQUIPMENT,
  ARRAY_UI
} ItemArray;

// Menu types
typedef enum {
  NONE,
  FRIDGE_MENU,
  PANTRY_MENU,
  STOVE_MENU,
  OVEN_MENU,
  DEEP_FRY_MENU,
  GRILL_MENU
} MENU_TYPE;

// Game screens
typedef enum {
  MAIN_MENU,
  SETTINGS_MENU,
  CAMPAIGN_MENU,
  MULTIPLAYER_MENU,
  ADVANCEMENTS_MENU,
  GAME,
  FRIDGE_SCREEN,
  PANTRY_SCREEN,
  STOVE_SCREEN,
  OVEN_SCREEN,
  DEEP_FRY_SCREEN,
  GRILL_SCREEN,
} GameScreen;

// Notification types
typedef enum {
  INFO,
  WARNING,
  ERROR,
  SUCCESS
} NOTIF_TYPE;

// Cooking result types
typedef enum {
  TIMING_BAR_RUNNING,
  TIMING_BAR_WIN,
  TIMING_BAR_LOSE
} TimingBarResult;

typedef enum {
  TARGET_GAME_RUNNING,
  TARGET_GAME_WIN,
  TARGET_GAME_LOSE
} TargetGameResult;

typedef enum {
  BASKET_CATCH_RUNNING,
  BASKET_CATCH_WIN,
  BASKET_CATCH_LOSE
} BasketCatchResult;

#endif
