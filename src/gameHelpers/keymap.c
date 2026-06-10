#include "game/keymap.h"
#include "external/parson.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

Keymap* keymaps = NULL;
int totalKeymaps = 0;

static int SearchLUT(const char* string, const StringMapping* lut, int lutSize) {
  for (int i = 0; i < lutSize; i++) {
    if (strcmp(string, lut[i].keyString) == 0) {
      return lut[i].keyValue;
    }
  }
  printf("keymap system: lookup failed, couldn't find: %s", string);
  return -1;
}

void LoadKeymap(const char* filename) {
  JSON_Value *rootValue = json_parse_file(filename);
  if (!rootValue) {
    printf("keymap system: failed to load keymap file: %s\n", filename);
    return;
  }

  JSON_Object *rootObject = json_value_get_object(rootValue);
  JSON_Array *keymapsArray = json_object_get_array(rootObject, "keymaps");

  totalKeymaps = (int)json_array_get_count(keymapsArray);
  keymaps = malloc(sizeof(Keymap) * totalKeymaps);

  int keyLutSize = KeyLUTSize;
  int actionLutSize = ActionLUTSize;

  for (int i = 0; i < totalKeymaps; i++) {
    JSON_Object *mapObj = json_array_get_object(keymapsArray, i);
    const char* keyStr = json_object_get_string(mapObj, "key");
    const char* actionStr = json_object_get_string(mapObj, "action");

    if (keyStr && actionStr) {
      keymaps[i].key = (KeyboardKey)SearchLUT(keyStr, KeyLUT, keyLutSize);
      keymaps[i].action = (KEYBOARD_ACTION)SearchLUT(actionStr, ActionLUT, actionLutSize);
    }
  }

  json_value_free(rootValue);
  printf("keymap system: mapped %d inputs using LUTs\n", totalKeymaps);
}

bool IsActionPressed(KEYBOARD_ACTION action) {
  for (int i = 0; i < totalKeymaps; i++) {
    if (keymaps[i].action == action) {
      if (IsKeyPressed(keymaps[i].key)) {
        return true;
      }
    }
  }
  return false;
}

bool IsActionDown(KEYBOARD_ACTION action) {
  for (int i = 0; i < totalKeymaps; i++) {
    if (keymaps[i].action == action) {
      if (IsKeyDown(keymaps[i].key)) {
        return true;
      }
    }
  }
  return false;
}

void UnloadKeymap(void) {
  if (keymaps != NULL) {
    free(keymaps);
    keymaps = NULL;
    totalKeymaps = 0;
    printf("keymap system: cleared keymaps.\n");
  }
}
