#ifndef _MICRO_MENU_H_
#define _MICRO_MENU_H_

#include <stddef.h>
#include <stdint.h>

#define MENU_ITEM_STORAGE PROGMEM

#define MENU_ITEM_READ_POINTER(Addr) *(Addr)

#define MENU_TEXT_SIZE 40
#define MENU_HINT_SIZE 128

enum param_type
{
  ptNone,
  ptBool,
  ptInt,
  ptDouble,
  ptString,
  ptHexString,
  ptDate,
  ptTime,
  ptEnum
};

bool bTrue = true;
bool bFalse = false;
#define DEF_ENABLED true
#define DEF_VISIBLE true

struct NumConstraints
{
  double minVal;
  double maxVal;
};

#define IS_SIGNED(param) ((param.Type != ptInt) && (param.Type != ptDouble) ? false : param.Constraints.minVal < 0 || param.Constraints.maxVal < 0)

char nullData = 0;

typedef const char EnumItem[MENU_TEXT_SIZE];

struct EnumStruct
{
  uint8_t count;
  const EnumItem *EnumItems;
};

struct Param_Data
{
  param_type Type;
  union {
    size_t Size;
    NumConstraints Constraints;
  };
  void *Pointer;
};

struct Menu_Item;

typedef struct Param_Struct{
  Param_Data Data;
  void *(*onRead)(Menu_Item *);
  void (*onWrite)(Menu_Item *, void *);
  void (*onWritten)(Menu_Item *);
} Param_Struct_t;

typedef struct Menu_Callbacks{
  void (*onEnter)(Menu_Item *); 
  bool (*getEnabled)(Menu_Item *);
  bool (*getVisible)(Menu_Item *);
} Menu_Callbacks_t;

typedef struct Menu_Item
{
  struct Menu_Item *Next;            
  struct Menu_Item *Previous;        
  struct Menu_Item *Parent;          
  struct Menu_Item *Child;            
  char Text[MENU_TEXT_SIZE];          
  char Hint[MENU_HINT_SIZE];
  bool *Visible;
  bool *Enabled;
  Param_Struct Param;
  Menu_Callbacks callbacks;
} Menu_Item_t;


#define MENU_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamType, ParamSize, ParamVar,  \
                   onEnter, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)                  \
  extern Menu_Item_t MENU_ITEM_STORAGE Next;                                                                                              \
  extern Menu_Item_t MENU_ITEM_STORAGE Previous;                                                                                          \
  extern Menu_Item_t MENU_ITEM_STORAGE Parent;                                                                                            \
  extern Menu_Item_t MENU_ITEM_STORAGE Child;                                                                                             \
  Menu_Item MENU_ITEM_STORAGE Name = {&Next, &Previous, &Parent, &Child, Text, Hint, Visible, Enabled, {{ParamType, ParamSize, &ParamVar},  \
                                      onParamRead, onParamWrite, onParamWritten}, {onEnter, onGetEnabled, onGetVisible}}

#define MENU_ITEM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, \
                  onEnter, onGetEnabled, onGetVisible) \
  MENU_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ptNone, 0, nullData, \
             onEnter, NULL, NULL, NULL, onGetEnabled, onGetVisible)

#define CONSTRAINTS(MinVal, MaxVal) \
  {                                 \
    Constraints:                    \
    {                               \
      MinVal, MaxVal                \
    }                               \
  }

#define MENU_BOOL_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible) \
  MENU_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ptBool, sizeof(bool), ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
#define MENU_INT_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, MinVal, MaxVal, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible) \
  MENU_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ptInt, CONSTRAINTS(MinVal, MaxVal), ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
#define MENU_DOUBLE_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, MinVal, MaxVal, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible) \
  MENU_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ptDouble, CONSTRAINTS(MinVal, MaxVal), ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
#define MENU_STRING_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible) \
  MENU_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ptString, sizeof(ParamVar), ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
#define MENU_HEX_STRING_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible) \
  MENU_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ptString, sizeof(ParamVar), ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
#define MENU_ENUM_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, EnumItems, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible) \
  MENU_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ptEnum, (size_t)&EnumItems, ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define FOO(...) GET_MACRO(__VA_ARGS__, FOO3, FOO2)(__VA_ARGS__)
#define FOO2(a, b) a+b;
#define FOO3(a, b, c) a+b+c;

/*int a=FOO(1, 2)
int b=FOO(1, 2, 3)*/

Menu_Item_t NULL_MENU = {0};

/*Menu_Item_t *AddMenuItem(Menu_Item_t *parent, char *text, char *hint, void (*onEnter)(Menu_Item *), bool *visible = NULL, bool *enabled = NULL, bool (*onGetVisible)(Menu_Item *) = NULL, bool (*onGetEnabled)(Menu_Item *) = NULL, Param_Struct param)
{
  if(!parent) return NULL;
  Menu_Item_t *newItem = new Menu_Item_t;
  Menu_Item_t *lastItem; // = &NULL_MENU;

  newItem->Next = &NULL_MENU;
  lastItem = parent->Child;
  if (lastItem == &NULL_MENU)
  { // если это первый элемент в меню
    parent->Child = newItem;
    newItem->Previous = &NULL_MENU;
  }
  else // иначе ищем последний
  { 
    while (lastItem->Next != &NULL_MENU)
      lastItem = lastItem->Next;
    newItem->Previous = lastItem;
    lastItem->Next = newItem;
  }
  newItem->Parent = parent;
  newItem->Child = &NULL_MENU;
  strncpy(newItem->Text, text, sizeof(newItem->Text));
  strncpy(newItem->Hint, hint, sizeof(newItem->Hint));
  newItem->Param = param;
  newItem->Visible = visible;
  newItem->Enabled = enabled;
  newItem->callbacks.onEnter = onEnter;
  newItem->callbacks.getEnabled = onGetEnabled;
  newItem->callbacks.getVisible = onGetVisible;
  //TODO: Дописать все поля
  return newItem;
}*/

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu parent. */
#define MENU_PARENT MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Parent)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu child. */
#define MENU_CHILD MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Child)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
#define MENU_NEXT MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Next)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the previous linked menu item. */
#define MENU_PREVIOUS MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Previous)

/** Null menu entry, used in \ref MENU_ITEM() definitions where no menu link is to be made. */

/** Retrieves the currently selected meny item.
	 *
	 *  \return Pointer to the currently selected meny item.
	 */
Menu_Item_t *Menu_GetCurrentMenu(void);

/** Navigates to an absolute or relative menu entry.
	 *
	 * \param[in] NewMenu  Pointer to the absolute menu item to select, or one of \ref MENU_PARENT,
	 *                     \ref MENU_CHILD, \ref MENU_NEXT or \ref MENU_PREVIOUS for relative navigation.
	 */
void Menu_Navigate(Menu_Item_t *const NewMenu);

/** Configures the menu text write callback function, fired for all menu items. Within this callback
	 *  function the user should implement code to display the current menu text stored in \ref MENU_ITEM_STORAGE
	 *  memory space.
	 *
	 *  \ref WriteFunc  Pointer to a callback function to execute for each selected menu item.
	 */
void Menu_SetGenericWriteCallback(void (*WriteFunc)(const char *Text));

/** Enters the currently selected menu item, running its configured callback function (if any). */
void Menu_EnterCurrentItem(void);

/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t *CurrentMenuItem = &NULL_MENU;

Menu_Item_t *Menu_GetCurrentMenu(void)
{
  return CurrentMenuItem;
}

void Menu_Navigate(Menu_Item_t *const NewMenu)
{
  if ((NewMenu == &NULL_MENU) || (NewMenu == NULL))
    return;
  CurrentMenuItem = NewMenu;
}

void Menu_EnterCurrentItem(void)
{
  if ((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
    return;

  void (*onEnter)(Menu_Item *) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->callbacks.onEnter);

  if (onEnter)
    onEnter((Menu_Item *)Menu_GetCurrentMenu());
}

bool Menu_isVisible(Menu_Item *menuitem){
  bool (*getVisible)(Menu_Item *) = MENU_ITEM_READ_POINTER(&menuitem->callbacks.getVisible);
  if(getVisible){
    return getVisible(menuitem);
  }
  else
  {
    if(menuitem->Visible != NULL) return *menuitem->Visible;
    else return DEF_VISIBLE;
  }
}

bool Menu_isEnabled(Menu_Item *menuitem){
  bool (*getEnabled)(Menu_Item *) = MENU_ITEM_READ_POINTER(&menuitem->callbacks.getEnabled);
  if(getEnabled){
    return getEnabled(menuitem);
  }
  else
  {
    if(menuitem->Enabled != NULL) return *menuitem->Enabled;
    else return DEF_ENABLED;
  }
}

void Menu_WriteParam(Menu_Item *menuitem, void * newValue){
    if ((menuitem == &NULL_MENU) || (menuitem == NULL))
    return;

  //Serial.printf("WriteParam: MenuItem.Text=%s, newValue=%d\n", CurrentMenuItem->Text, *(char*)newValue);

  void (*onWrite)(Menu_Item *, void*) = MENU_ITEM_READ_POINTER(&menuitem->Param.onWrite);

  if (onWrite)
    onWrite((Menu_Item *)menuitem, newValue);
  else{
    switch(menuitem->Param.Data.Type){
      case ptBool:
          *(bool*)menuitem->Param.Data.Pointer = *(bool*)newValue;
        break;
      case ptInt:
          *(int*)menuitem->Param.Data.Pointer = *(int*)newValue;
          //Serial.printf("ptInt=%d\n", *(int*)newValue);
        break;
      case ptDouble:
          *(double*)menuitem->Param.Data.Pointer = *(double*)newValue;
          //Serial.printf("ptDouble=%f\n", *(double*)newValue);
        break;
      case ptString:
      case ptHexString: {
          //size_t strsize = _min(strlen((char*) newValue), menuitem->Param.Data.Size-1);
          size_t strsize = menuitem->Param.Data.Size;
          memcpy(menuitem->Param.Data.Pointer, newValue, strsize);
          //Serial.printf("ptString=%s\n", (char*)newValue);
          ((char*)menuitem->Param.Data.Pointer)[strsize] = 0;
      }
        break;
      // case ptDate:
      //     *(bool*)menuitem->Param.Data.Pointer = *(bool*)newValue;
      //   break;
      // case ptTime:
      //     *(bool*)menuitem->Param.Data.Pointer = *(bool*)newValue;
      //   break;
      case ptEnum:
          *(uint8_t*)menuitem->Param.Data.Pointer = *(uint8_t*)newValue;
        break;
    }
  }
  void (*onWritten)(Menu_Item *) = MENU_ITEM_READ_POINTER(&menuitem->Param.onWritten);
  if (onWritten)
    onWritten((Menu_Item *)menuitem);
}

void * Menu_ReadParam(Menu_Item *menuitem){
    if ((menuitem == &NULL_MENU) || (menuitem == NULL))
    return NULL;
//Serial.printf("ReadParam: MenuItem.Text=%s\n", CurrentMenuItem->Text);
  void *(*onRead)(Menu_Item *) = MENU_ITEM_READ_POINTER(menuitem->Param.onRead);

  if (onRead)
    return onRead((Menu_Item *)menuitem);
  else {
    //Serial.printf("ReadParam: =%d\n", *(bool*)CurrentMenuItem->Param.Data.Pointer);
    return menuitem->Param.Data.Pointer;}
}

Menu_Item_t *menuNextVisible(Menu_Item_t *menu)
{
  Menu_Item_t *tempMenu = menu;
  if ((tempMenu == NULL) || (tempMenu == &NULL_MENU))
    return &NULL_MENU;
  while (!Menu_isVisible(tempMenu))
  {
    if ((tempMenu->Next == NULL) || (tempMenu->Next == &NULL_MENU))
    {
      return &NULL_MENU;
    }
    tempMenu = (Menu_Item_t *)(tempMenu->Next);
    if ((tempMenu == NULL) || (tempMenu == &NULL_MENU))
      return &NULL_MENU;
  };
  return tempMenu;
}

Menu_Item_t *menuPrevVisible(Menu_Item_t *menu)
{
  Menu_Item_t *tempMenu = menu;
  if ((tempMenu == NULL) || (tempMenu == &NULL_MENU))
    return &NULL_MENU;
  while (!Menu_isVisible(tempMenu))
  {
    if ((tempMenu->Previous == NULL) || (tempMenu->Previous == &NULL_MENU))
    {
      return &NULL_MENU;
    }
    tempMenu = (Menu_Item_t *)(tempMenu->Previous);
    if ((tempMenu == NULL) || (tempMenu == &NULL_MENU))
      return &NULL_MENU;
  };
  return tempMenu;
}
#endif
