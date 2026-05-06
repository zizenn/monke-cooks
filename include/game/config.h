#ifndef CONFIG_H
#define CONFIG_H

// ====================================
// GAME & MAP CONFIGURATION
// ====================================
#define MAP_ROWS 9
#define MAP_COLS 16
#define TILE_SIZE 64
#define VISIBLE_ROWS 9

#define VIRTUAL_WIDTH (TILE_SIZE * MAP_COLS)   // 1024
#define VIRTUAL_HEIGHT (TILE_SIZE * VISIBLE_ROWS) // 576


// ====================================
// FRIDGE & PANTRY
// ====================================
#define INVENTORY_COLUMNS 8
#define INVENTORY_ITEM_WIDTH 84
#define INVENTORY_ITEM_HEIGHT 84

// Inventory item counts
#define FRIDGE_ITEM_COUNT 11
#define PANTRY_ITEM_COUNT 10

// Inventory UI details
#define INVENTORY_TEXTURE_SCALE 5.25f
#define INVENTORY_QUANTITY_BOX_WIDTH 20
#define INVENTORY_QUANTITY_BOX_HEIGHT 20
#define INVENTORY_QUANTITY_BOX_FONT_SIZE 16

// Inventory item label styling
#define INVENTORY_LABEL_FONT_SIZE 10
#define INVENTORY_LABEL_PADDING 3

// Search box configuration
#define INVENTORY_SEARCH_BOX_MAX_LENGTH 64
#define INVENTORY_CATEGORY_NAME_MAX_LENGTH 65


// ====================================
// TIMING BAR MINIGAME
// ====================================
#define TIMING_BAR_WIDTH 400
#define TIMING_BAR_HEIGHT 20
#define TIMING_BAR_OFFSET_X 200.0f
#define TIMING_BAR_OFFSET_Y 700.0f

// Player indicator
#define TIMING_PLAYER_WIDTH 10
#define TIMING_PLAYER_SPEED 300.0f

// Safe zone configuration
#define TIMING_SAFEZONE_WIDTH 70
#define TIMING_SAFEZONE_MAX_VELOCITY 200.0f
#define TIMING_SAFEZONE_INITIAL_ACCEL 5.0f
#define TIMING_SAFEZONE_INITIAL_VEL 150.0f

// Win/lose conditions
#define TIMING_WIN_TIME 5.0f
#define TIMING_LOSE_TIME 30.0f
#define TIMING_MISS_COOLDOWN 0.5f


// ====================================
// BASKET CATCH MINIGAME
// ====================================
#define BASKET_WIDTH 60
#define BASKET_HEIGHT 20
#define BASKET_OFFSET 40

// Field configuration
#define BASKET_FIELD_X 200
#define BASKET_FIELD_Y 0
#define BASKET_FIELD_WIDTH 200
#define BASKET_FIELD_HEIGHT 300

// Objects (falling items)
#define BASKET_OBJECT_COUNT 30
#define BASKET_OBJECT_SPACING 80.0f
#define BASKET_OBJECT_WIDTH 20
#define BASKET_OBJECT_HEIGHT 20
#define BASKET_INITIAL_OBJECT_SPEED 100.0f
#define BASKET_OBJECT_SPEED_INCREMENT 15.0f

// Game rules
#define BASKET_MAX_MISSES 5
#define BASKET_MISS_COOLDOWN 0.5f


// ====================================
// TARGET GAME MINIGAME
// ====================================

#define TARGET_COUNT 10
#define TARGET_OUTER_RADIUS 100.0f
#define TARGET_PLAYER_RADIUS 40.0f
#define TARGET_RING_SEGMENTS 72

// Game physics
#define TARGET_SPEED 120.0f
#define TARGET_HIT_TOLERANCE 12.0f

// Target spawning
#define TARGET_SPAWN_MIN_OFFSET 20
#define TARGET_SPAWN_MAX_OFFSET 100

// Game rules
#define TARGET_MAX_MISSES 2
#define TARGET_MISS_COOLDOWN 0.5f

// ====================================
// TILE COLORS & STYLING
// ====================================

#define TILE_WALKABLE_COLOR RAYWHITE
#define TILE_COUNTER_COLOR LIGHTGRAY
#define TILE_COUNTER_TEXT_COLOR WHITE

#define TILE_FRIDGE_COLOR DARKBLUE
#define TILE_FRIDGE_TEXT_COLOR WHITE

#define TILE_STOVE_COLOR GREEN
#define TILE_STOVE_TEXT_COLOR BLACK

#define TILE_OVEN_COLOR YELLOW
#define TILE_OVEN_TEXT_COLOR BLACK

#define TILE_DEEP_FRY_COLOR ORANGE
#define TILE_DEEP_FRY_TEXT_COLOR BLACK

#define TILE_GRILL_COLOR RED
#define TILE_GRILL_TEXT_COLOR BLACK

#define TILE_ASSEMBLY_COLOR GRAY
#define TILE_ASSEMBLY_TEXT_COLOR WHITE

#define TILE_SINK_COLOR BLUE
#define TILE_SINK_TEXT_COLOR BLACK

#define TILE_CUTTING_STATION_COLOR DARKPURPLE
#define TILE_CUTTING_STATION_TEXT_COLOR WHITE

#define TILE_TRASH_COLOR BLACK
#define TILE_TRASH_TEXT_COLOR WHITE

#define TILE_PANTRY_COLOR BROWN
#define TILE_PANTRY_TEXT_COLOR WHITE

#define TILE_GRINDING_STATION_COLOR MAROON
#define TILE_GRINDING_STATION_TEXT_COLOR WHITE

// Tile label styling
#define TILE_LABEL_FONT_SIZE 14
#define TILE_LABEL_OFFSET_X 4
#define TILE_LABEL_OFFSET_Y 4

// Grid lines
#define TILE_GRID_COLOR GRAY
#define TILE_GRID_LINE_WIDTH 1

// Inventory UI colors
#define INVENTORY_LABEL_BG_COLOR (Color){200, 200, 200, 255}
#define INVENTORY_LABEL_BORDER_COLOR BLACK

#endif