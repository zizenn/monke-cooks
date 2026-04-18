#ifndef ITEMS
#define ITEMS

typedef enum {
  EMPTY,
  BURRITO,
  BAGUETTE,
  EGG
} ITEM;

typedef struct{
  ITEM item;
  int amnt;
} inventorySlot;

extern inventorySlot stockedItems[];

#endif
