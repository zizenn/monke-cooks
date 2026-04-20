#include "game/items.h"
#include "game/cooking.h"
#include "minigames/minigame.h"

Rectangle totalArea = (Rectangle){640-346.5, 360-178.5, 693, 357};
Rectangle panelBounds = (Rectangle){640-346.5, 360-178.5, 693, 341};

Foods allIngredients[] = {
  { "RAW_EGG", PAN, 3.50, "assets/food/ingredients/eggs/egg_orange.png" },
  { "RAW_RICE", OVEN, 5.00, "assets/food/ingredients/grains/rice.png" },
  { "RAW_SHIITAKE_MUSHROOM", PAN, 5.50, "assets/food/ingredients/mushrooms/shiitake.png" },
};

Foods allFoods[] = {

};
