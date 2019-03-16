//bool Enabled(Menu_Item_t *current_item){return true;}
//bool Disabled(Menu_Item_t *current_item){return false;}

void doScan(Menu_Item_t *current_item);
void editParam(Menu_Item_t *current_item);
void *readDayOfWeek(Menu_Item_t *current_item);
void writeDayOfWeek(Menu_Item_t *current_item, void* newValue);
void connectWiFi(Menu_Item_t *current_item);
void disconnectWiFi(Menu_Item_t *current_item);
bool canConnectWiFi(Menu_Item_t *current_item);
bool canDisconnectWiFi(Menu_Item_t *current_item);
void doSaveSettings(Menu_Item_t *current_item) { saveSettings(); }

#define MENU_FIRST_ITEM mWifi
// MENU_ITEM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, onEnter, onGetEnabled, onGetVisible)
// MENU_BOOL_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
// MENU_INT_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, MinVal, MaxVal, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible) 
// MENU_DOUBLE_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, MinVal, MaxVal, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible) 
// MENU_STRING_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
// MENU_HEX_STRING_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
// MENU_ENUM_PARAM(Name, Next, Previous, Parent, Child, Text, Hint, Visible, Enabled, ParamVar, EnumItems, onEdit, onParamRead, onParamWrite, onParamWritten, onGetEnabled, onGetVisible)
//
MENU_ITEM(mWifi, mBluetooth, mSaveSettings, NULL_MENU, mConnectWiFi , "WI-FI", "Параметры подключения к WI-FI", NULL, NULL, NULL, NULL, NULL);
  MENU_ITEM(mConnectWiFi, mDisconnectWiFi, NULL_MENU, mWifi, NULL_MENU, "Подключиться", "Подключиться к сохраненной сети WI-FI", NULL, NULL, &connectWiFi, NULL, &canConnectWiFi);
  MENU_ITEM(mDisconnectWiFi, mNetworks, mConnectWiFi, mWifi, NULL_MENU, "Отключиться", "Отключиться от текущей сети WI-FI", NULL, NULL, &disconnectWiFi, NULL, &canDisconnectWiFi);
  MENU_ITEM(mNetworks, mAutoConnect, mDisconnectWiFi, mWifi, NULL_MENU, "Сохраненные сети", "Сохраненные сети Wi-Fi", NULL, NULL, NULL, NULL, NULL);
    // MENU_ITEM(mNetwork0, mNetwork1, NULL_MENU, mNetworks, mSSID0, NULL, NULL, "Сеть #0", "", bTrue, &Enabled);
    //   MENU_STRING_PARAM(mSSID0, mPassword0, NULL_MENU, mNetwork0, NULL_MENU, NULL, &editParam, "SSID", "", bTrue, &Enabled, wifiNetworks[0].SSID, NULL, NULL);
    //   MENU_STRING_PARAM(mPassword0, NULL_MENU, mSSID0, mNetwork0, NULL_MENU, NULL, &editParam, "Пароль", "", bTrue, &Enabled, wifiNetworks[0].Password, NULL, NULL);
    // MENU_ITEM(mNetwork1, mNetwork2, mNetwork0, mNetworks, mSSID1, NULL, NULL, "Сеть #1", "", bTrue, &Enabled);
    //   MENU_STRING_PARAM(mSSID1, mPassword1, NULL_MENU, mNetwork1, NULL_MENU, NULL, &editParam, "SSID", "", bTrue, &Enabled, wifiNetworks[1].SSID, NULL, NULL);
    //   MENU_STRING_PARAM(mPassword1, NULL_MENU, mSSID1, mNetwork1, NULL_MENU, NULL, &editParam, "Пароль", "", bTrue, &Enabled, wifiNetworks[1].Password, NULL, NULL);
    // MENU_ITEM(mNetwork2, NULL_MENU, mNetwork1, mNetworks, mSSID2, NULL, NULL, "Сеть #2", "", bTrue, &Enabled);
    //   MENU_STRING_PARAM(mSSID2, mPassword2, NULL_MENU, mNetwork2, NULL_MENU, NULL, &editParam, "SSID", "", bTrue, &Enabled, wifiNetworks[2].SSID, NULL, NULL);
    //   MENU_STRING_PARAM(mPassword2, NULL_MENU, mSSID2, mNetwork2, NULL_MENU, NULL, &editParam, "Пароль", "", bTrue, &Enabled, wifiNetworks[2].Password, NULL, NULL);
  MENU_BOOL_PARAM(mAutoConnect, mAutoReconnect, mNetworks, mWifi, NULL_MENU, "Автоконнект", "Автоподключение к доступной сохраненной сети при запуске", 
                  NULL, NULL, wifiAutoConnect, &editParam, NULL, NULL, NULL, NULL, NULL);
  MENU_BOOL_PARAM(mAutoReconnect, NULL_MENU, mAutoConnect, mWifi, NULL_MENU, "Переподключение", "Переподключаться при обрыве соединения", 
                  NULL, NULL, wifiAutoReconnect, &editParam, NULL, NULL, NULL, NULL, NULL);
MENU_ITEM(mBluetooth, mScanner, mWifi, NULL_MENU, mVal1 , "Bluetooth", "Параметры Bluetooth", NULL, NULL, NULL, NULL, NULL);
  MENU_INT_PARAM(mVal1, mVal2, NULL_MENU, mBluetooth, NULL_MENU, "Значение1 int", "", NULL, NULL, ival1, 0, INT_MAX, &editParam, NULL, NULL, NULL, NULL, NULL);
  MENU_DOUBLE_PARAM(mVal2, mVal3, mVal1, mBluetooth, NULL_MENU, "Значение2 double", "", NULL, NULL, dval, -5.0, 20000.0, &editParam, NULL, NULL, NULL, NULL, NULL);
  MENU_INT_PARAM(mVal3, NULL_MENU, mVal2, mBluetooth, NULL_MENU, "Значение3 int", "", NULL, NULL, ival3, -50, 100, &editParam, NULL, NULL, NULL, NULL, NULL);
MENU_ITEM(mScanner, mSystem, mBluetooth, NULL_MENU, mApi, "Сканер", "Настройки сканирования билетов", NULL, NULL, NULL, NULL, NULL);
  MENU_ITEM(mApi, mScan, NULL_MENU, mScanner, mApiKey , "Api", "Параметры Api", NULL, NULL, NULL, NULL, NULL);
    MENU_HEX_STRING_PARAM(mApiKey, NULL_MENU, NULL_MENU, mApi, NULL_MENU, "Ключ Api", "", NULL, NULL, ApiKey, &editParam, NULL, NULL, NULL, NULL, NULL);
  MENU_ITEM(mScan, NULL_MENU, mApi, mScanner, NULL_MENU, "Сканировать", "", NULL, NULL, &doScan, NULL, NULL);
MENU_ITEM(mSystem, mInfo, mScanner, NULL_MENU, mDatetime, "Система", "Системные настройки", NULL, NULL, NULL, NULL, NULL);
  MENU_ITEM(mDatetime, NULL_MENU, NULL_MENU, mSystem, mWeekDay, "Дата / Время", "", NULL, NULL, NULL, NULL, NULL);
    MENU_ENUM_PARAM(mWeekDay, NULL_MENU, NULL_MENU, mDatetime, NULL_MENU, "День недели", "", NULL, NULL, nullData, DaysOfWeek, &editParam, &readDayOfWeek, &writeDayOfWeek, NULL, NULL, NULL);
MENU_ITEM(mInfo, mSaveSettings, mSystem, NULL_MENU, NULL_MENU, "Информация", "Общая информация об устройстве", NULL, NULL, NULL, NULL, NULL);
MENU_ITEM(mSaveSettings, mWifi, mInfo, NULL_MENU, NULL_MENU, "Записать настройки", "Записать в память измененные настройки", NULL, NULL, &doSaveSettings, NULL, NULL);