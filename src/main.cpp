//TODO: WPS support
//TODO: Частичное обновление экрана вместо полного
#include "tmaster.h"

void onScanDone(char *barcode, long mode);
//HardwareSerial Serial2(2);
e1007 scanner(&Serial2, &onScanDone, 9600);
U8G2_HX1230_96X68_F_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/12, /* cs=*/14, /* reset=*/27);
//U8G2_SSD1327_MIDAS_128X128_F_HW_I2C u8g2(U8G2_R0);
#define scl 22
#define sda 21
//U8G2_SSD1327_EA_W128128_F_SW_I2C u8g2(U8G2_R0, scl, sda);
//uRTCLib rtc(RTC_ADDRESS);

/*String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}*/

void redrawScreen();

void clearScreenEx()
{
  u8g2.setMaxClipWindow();
  if (CurrentCaptions.Captions == NULL)
    u8g2.clearBuffer();
  else
  {
    uint8_t prevColor = u8g2.getDrawColor();
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, DISP_WIDTH, DISP_HEIGHT - CAPTIONS_HEIGHT);
    u8g2.setDrawColor(prevColor);
  }
}

void changeMode(OperatinalModes new_mode)
{
  CurrentCaptions.Captions = NULL;
  switch (new_mode)
  {
  case omStandby:
    CHANGE_CURRENT_CAPTIONS(StandbyCaptions)
    caption_x_offset = 0;
    caption_scrolling_left = true;
    break;
  case omEdit:
    CHANGE_CURRENT_CAPTIONS(EditCaptions)
    caption_x_offset = 0;
    caption_scrolling_left = true;
    break;
  case omMenuList:

    break;
  case omMenuHint:
    hint_x_offset = 0;
    hint_scrolling = false;
    hint_scroll_left = true;
    break;
  }
  CurMode = new_mode;
}

void drawMessageBox(){
  u8g2_uint_t box_x = (DISP_WIDTH - mbInfo.width) / 2;
  u8g2_uint_t box_y = (DISP_HEIGHT - mbInfo.height) / 2;
  u8g2_uint_t offset_x = box_x;
  uint8_t right_offset = 0;

  u8g2.setFont(BASE_FONT);
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1); 
  u8g2.setAutoPageClear(0);
  u8g2.firstPage();
  do {
    u8g2.drawBox(box_x, box_y, mbInfo.width, ICON_HEIGHT + 2); // инвертируем заголовок
    u8g2.setDrawColor(0);
    if(mbInfo.icon){ //если нужна иконка
      u8g2.setFont(ICON_FONT);
      u8g2.drawGlyph(offset_x + 1, box_y + ICON_HEIGHT + 1, mbInfo.icon);
      offset_x += ICON_WIDTH + 3;
      u8g2.setFont(BASE_FONT);
    }
    u8g2.setClipWindow(offset_x, box_y + 1, DISP_WIDTH - box_x, box_y + ICON_HEIGHT); //ограничиваем зону текста заголовка
    u8g2.drawUTF8(offset_x, box_y + 1 + (ICON_HEIGHT + mbInfo.line_height + u8g2.getDescent()) / 2 , mbInfo.title); //выводим текст заголовка
    u8g2.setMaxClipWindow();
    u8g2.drawBox(box_x + 1, box_y + ICON_HEIGHT + 2, mbInfo.width - 2, mbInfo.height - ICON_HEIGHT - 2); // очищаем зону для сообщения
    u8g2.drawFrame(box_x - 1, box_y - 1, mbInfo.width + 2, mbInfo.height + 2);
    u8g2.setDrawColor(1);
    u8g2.drawFrame(box_x, box_y, mbInfo.width, mbInfo.height); // рисуем общую рамку
    if(mbInfo.message.size() > mbInfo.max_lines){ // рисуем прокрутку
      right_offset = 2;
      u8g2.drawVLine(box_x + mbInfo.width - 1 - right_offset, box_y + ICON_HEIGHT + 2, mbInfo.height - ICON_HEIGHT - 3);
      float piece =  (float)(mbInfo.height - ICON_HEIGHT - 5) / mbInfo.message.size();
      u8g2.drawVLine(box_x + mbInfo.width - 1 - right_offset + 1, box_y + ICON_HEIGHT + 2 + mbInfo.top_line * piece + 1, mbInfo.screen_lines * piece + 1);
    }
    u8g2.setClipWindow(box_x + 2, box_y + ICON_HEIGHT + 3, box_x + mbInfo.width - 2 - right_offset, box_y + mbInfo.height - 2);
    u8g2_uint_t msg_y = box_y + ICON_HEIGHT + 2 + mbInfo.line_height;
    //Serial.println("=====");
    u8g2_uint_t msg_y_offset = (mbInfo.height - ICON_HEIGHT - 6 - mbInfo.screen_lines * mbInfo.line_height) / 2;
    Serial.println(mbInfo.height - ICON_HEIGHT);
    for(int i = mbInfo.top_line; i < mbInfo.top_line + mbInfo.screen_lines; i++){
      u8g2_uint_t line_offset_x = 0;
      u8g2_uint_t line_width = u8g2.getUTF8Width(mbInfo.message[i].c_str());
      switch(mbInfo.align){
        case alLeft: 
            line_offset_x = box_x + 2;
          break;
        case alCenter: 
            line_offset_x = box_x + 2 + (mbInfo.width - 2 - right_offset - line_width) / 2 - 1;
          break;
        case alRight: 
            line_offset_x = box_x + (mbInfo.width - 2 - right_offset - line_width);
            Serial.printf("box_x=%d, mbInfo.width=%d, right_offset=%d, line_width=%d, msg=%s\n", box_x, mbInfo.width, right_offset, line_width, mbInfo.message[i].c_str());
          break;
      }
      u8g2.drawUTF8(line_offset_x,  msg_y + msg_y_offset, mbInfo.message[i].c_str());
      //Serial.println(mbInfo.message[i].c_str());
      msg_y += mbInfo.line_height;
    }
  } while ( u8g2.nextPage() );
  u8g2.setAutoPageClear(1);
}

//TODO: Оптимизировать
std::vector<String> splitLines(const char *text, const uint8_t *font, u8g2_uint_t &width){
  u8g2_uint_t tmp_w, newWidth = 0;
  Serial.println(width);
  std::vector<String> lines;
  std::vector<String> words;
  char *buf = strdup(text);
  char *word;
  for(word = strtok(buf, MB_DELIMETERS); word != NULL; word = strtok(NULL, MB_DELIMETERS)){
    words.push_back(word);
  }
  //return words;
  /*for(int k=0; k<words.size(); k++){
    Serial.println(words[k]);
  }*/
  
  int idx = 0;
  int cnt;
  String s;// = words[idx];
  u8g2.setFont(font);
  s = words[0];
  //Serial.printf("init s=%s\n", s.c_str());
  idx = 1;
  while(idx <= words.size()){
    //Serial.printf("idx=%d\n", idx);
    cnt = 0;
    do{
      s = s.substring(cnt);
      //Serial.printf("---------->s=%s(%d)\n", s.c_str(), s.length());
      cnt = s.length();
      while((tmp_w = u8g2.getUTF8Width(s.substring(0, cnt-1).c_str())) > width) cnt--;
      //Serial.printf("---------->cnt=%d\n", cnt);
      if(s.length() > cnt){
        lines.push_back(s.substring(0, cnt));
        //Serial.printf("w=%d\n", u8g2.getUTF8Width(lines.back().c_str()));
        newWidth = _max(newWidth, u8g2.getUTF8Width(lines.back().c_str()));
        //Serial.printf("***push=%s\n", lines.back().c_str());
      }
    } while(s.length() > cnt);

    if(idx == words.size()){
      lines.push_back(s);
      //Serial.printf("w=%d\n", u8g2.getUTF8Width(lines.back().c_str()));
      newWidth = _max(newWidth, u8g2.getUTF8Width(lines.back().c_str()));
      //Serial.printf("end push=%s\n", lines.back().c_str());
    } else {
      String tmp = s + " " + words[idx];
      if((tmp_w = u8g2.getUTF8Width(tmp.c_str())) > width){
        lines.push_back(s);
        //Serial.printf("w=%d\n", u8g2.getUTF8Width(lines.back().c_str()));
        newWidth = _max(newWidth, u8g2.getUTF8Width(lines.back().c_str()));
        //Serial.printf("push=%s\n", lines.back().c_str());
        s = words[idx];
      }
      else s = tmp;
    }  
    idx++;
  }
  //Serial.println("======done=========");

  free(buf);
  width = _min(newWidth, width);
  return lines;
}

void messageBox(const char *title, const char *message, uint16_t icon = 0, mbAlign align = alCenter, long timeout = 0, void (*onClose)(void) = NULL){
  mbInfo.icon = icon;
  mbInfo.align = align;
  mbInfo.top_line = 0;
  if(onClose)
    mbInfo.onClose = onClose;
  
  if(timeout) mbInfo.timeout = millis() + timeout;
  else mbInfo.timeout = 0;

  strncpy(mbInfo.title, title, sizeof(mbInfo.title)-1);
  mbInfo.title[sizeof(mbInfo.title)-1] = '\0';
  u8g2.setFont(BASE_FONT);
  u8g2_uint_t title_width = _min(u8g2.getUTF8Width(title) + (icon ? ICON_WIDTH : 0) + 1, MB_MAX_WIDTH - 4);
  u8g2_uint_t msg_width = MB_MAX_WIDTH - 4;
  mbInfo.line_height = u8g2.getAscent() - u8g2.getDescent();
  mbInfo.max_lines = (DISP_HEIGHT - MB_MIN_BORDER * 2 - ICON_HEIGHT - 6) / mbInfo.line_height;
  mbInfo.message.clear();
  //Serial.println("############### FIRST PASS");
  mbInfo.message = splitLines(message, BASE_FONT, msg_width);
  if(mbInfo.message.size() > mbInfo.max_lines){
    msg_width = _max(title_width, msg_width - 2);
    mbInfo.message.clear();
    //Serial.println("SECOND PASS #################");
    mbInfo.message = splitLines(message, BASE_FONT, msg_width);
    msg_width += 2;
    //msg_width = _min(msg_width + 2, MB_MAX_WIDTH - 4);
  }
  //mbInfo.width = msg_width + 4;
  mbInfo.width = _min(_max(msg_width, title_width) + 4, MB_MAX_WIDTH);
  mbInfo.screen_lines = _min(mbInfo.max_lines, mbInfo.message.size());
  u8g2_uint_t msg_height = mbInfo.line_height * _max(mbInfo.screen_lines, 3);
  mbInfo.height = ICON_HEIGHT + 6 + msg_height;
//Serial.printf("line_height=%d, max_lines=%d, size()=%d, screen_lines=%d, msg_height=%d", mbInfo.line_height, mbInfo.max_lines, mbInfo.message.size(), mbInfo.screen_lines, msg_height);
  if(CurMode != omMessageBox){
    mbInfo.prevMode = CurMode;
    changeMode(omMessageBox);
  }
}

void editParam(Menu_Item_t *current_item)
{
  void *Value = Menu_ReadParam(current_item);
  CurParam.Type = current_item->Param.Data.Type;
  CurParam.Size = current_item->Param.Data.Size;
  CurParam.Constraints = current_item->Param.Data.Constraints;
  CurParam.MenuItem = current_item;
  char tmp[16];
  switch (CurParam.Type)
  {
  case ptBool: 
    bool temp_bool;
    temp_bool = *(bool *)Value;
    temp_bool = !temp_bool;
    Menu_WriteParam(current_item, &temp_bool);
    redrawScreen();
    return;
    break;
  case ptInt:
    CurParam.charset = num_chars;
    CurParam.strLen = sprintf(CurParam.strVal, "%d", (int)_max(abs(CurParam.Constraints.minVal), abs(CurParam.Constraints.maxVal))) + (IS_SIGNED(CurParam) ? 1 : 0);
    sprintf(tmp, "%%0%s%dd", IS_SIGNED(CurParam) ? "+" : "", CurParam.strLen);
    sprintf(CurParam.strVal, tmp, *(int *)Value);
    redrawScreen();
    break;
  case ptDouble:
    CurParam.charset = num_chars;
    CurParam.strLen = sprintf(CurParam.strVal, "%d", (int)ceil(_max(abs(CurParam.Constraints.minVal), abs(CurParam.Constraints.maxVal)))) + (IS_SIGNED(CurParam) ? 1 : 0) + 4;
    sprintf(tmp, "%%%s0*.3f", IS_SIGNED(CurParam) ? "+" : "");
    sprintf(CurParam.strVal, tmp, CurParam.strLen, *(double *)Value);
    redrawScreen();
    break;
  case ptString:
    CurParam.charset = string_chars;
    CurParam.strLen = CurParam.Size - 1;
    memcpy((void *)CurParam.strVal, Value, CurParam.Size);
    break;
  case ptHexString:
    CurParam.charset = hex_chars;
    CurParam.strLen = CurParam.Size - 1;
    memcpy((void *)CurParam.strVal, Value, CurParam.Size);
    break;
  case ptEnum:
    EnumStruct *enumData = (EnumStruct *)(CurParam.Size);
    char *itemsBuf = new char[sizeof(EnumItem) * enumData->count + 1];
    int pos = 0;
    for (int i = 0; i < enumData->count; i++)
    {
      pos += sprintf(itemsBuf + pos, "%s\n", enumData->EnumItems[i]);
    }
    itemsBuf[pos - 1] = 0;
    uint8_t res = *(uint8_t *)Value + 1;
    //Serial.println(itemsBuf);
    delay(REPEAT_FREQUENCY);
    res = u8g2.userInterfaceSelectionList(CurParam.MenuItem->Text, res, itemsBuf);
    delete itemsBuf;
    if (res){
      res--;
      Menu_WriteParam(current_item, &res);
    }
      //*(uint8_t *)CurParam.Value->Pointer = res - 1;
    return;
    break;
  }
  CurParam.prevMode = CurMode;
  CurParam.cursor_pos = 0;
  changeMode(omEdit); 
}

void saveParam()
{
  switch (CurParam.Type)
  {
  case ptInt: {
      int val = strtol(CurParam.strVal, NULL, 10);
      Menu_WriteParam(CurParam.MenuItem, &val);
    }
    break;
  case ptDouble: {
      double val = strtof(CurParam.strVal, NULL);
      Menu_WriteParam(CurParam.MenuItem, &val);
    }
    break;
  case ptString:
  case ptHexString:
      Menu_WriteParam(CurParam.MenuItem, &CurParam.strVal);
    break;
  default:
    u8g2.userInterfaceMessage("Параметр НЕ сохранен", "", "", "OK");
    break;
  }
}

void *readDayOfWeek(Menu_Item_t *current_item){
  dayOfWeek = 0/*rtc.dayOfWeek()*/;
  return &dayOfWeek;
}

void writeDayOfWeek(Menu_Item_t *current_item, void* newValue){
  dayOfWeek = *(uint8_t*)newValue;
  //rtc.set(rtc.second(), rtc.minute(), rtc.hour(), dayOfWeek, rtc.day(), rtc.month(), rtc.year());
}

bool canConnectWiFi(Menu_Item_t *current_item)
{
  return wifiStatus != wsConnected;
}

bool canDisconnectWiFi(Menu_Item_t *current_item)
{
  return wifiStatus == wsConnected;
}

bool reconnectWiFi(){
  if(reconnect_attempts < WIFI_RECONNECT_ATTEMPTS){
    wifiStatus = wsConnecting;
    //Serial.printf("...attempt to reconnect #%d\n", reconnect_attempts);
    WiFi.disconnect(true);
    WiFi.begin();
    reconnect_attempts++;
    return true;
  }
  else return false;
}

void connectToAP(){
  Serial.printf("SSID=%s\n", currSSID);
  WiFi.begin(currSSID, CurParam.strVal);
}

void connectWiFi(Menu_Item_t *current_item)
{
  WiFi.disconnect(true);
  /*CurParam.Type = ptString;
  CurParam.Size = current_item->Param.Data.Size;
  CurParam.Constraints = current_item->Param.Data.Constraints;
  CurParam.MenuItem = NULL;*/

  uint8_t num = WiFi.scanNetworks();
  String sNetworks;
  for(int i=0; i < num; i++){
    sNetworks += WiFi.SSID(i);
    if(i < num - 1) sNetworks += "\n";
  }
  uint8_t sel = u8g2.userInterfaceSelectionList("Доступные сети", 1, sNetworks.c_str());
  if(sel){
    if(WiFi.encryptionType(sel - 1) == WIFI_AUTH_OPEN)
      WiFi.begin(WiFi.SSID(sel - 1).c_str());
    else {

    }
  }
  
  //WiFi.begin(wifiNetworks[2].SSID, wifiNetworks[2].Password);
  //Serial.printf("Connecting WiFi (SSID=%s, Password=%s)", wifiNetworks[2].SSID, wifiNetworks[2].Password);
  /*int counter = 5;
  while ((WiFi.status() != WL_CONNECTED) && counter--)
  {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("OK");
    
  }
  else
    Serial.println("TIMEOUT");*/
  //wifiConnected = (WiFi.status() == WL_CONNECTED);
  //redrawScreen();
}

void disconnectWiFi(Menu_Item_t *current_item)
{
  WiFi.disconnect(true);
  wifiStatus = wsDisconnected;
  redrawScreen();
}

void onConnectWiFi(system_event_id_t event_id){
  reconnect_attempts = 0;
  wifiStatus = wsConnected;
  //redrawScreen();
}

void onDisconnectWiFi(system_event_t *event){
  //redrawScreen();
  //Serial.print("WIFi disconnected");
  uint8_t reason = event->event_info.disconnected.reason;
  if(((reason == WIFI_REASON_AUTH_EXPIRE) ||
            (reason >= WIFI_REASON_BEACON_TIMEOUT && reason != WIFI_REASON_AUTH_FAIL))){
    if (!reconnectWiFi()) {
      wifiStatus = wsDisconnected;
      redrawScreen();
    }
  }
  else wifiStatus = wsDisconnected;
}

void setup()
{
  Serial.begin(115200);
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    pinMode(buttons[i], INPUT);
    lastButtonState[i] = HIGH;
  }
  pinMode(SCAN_LOGIN_PIN, INPUT_PULLUP);
  pinMode(SCAN_LOGOUT_PIN, INPUT_PULLUP);
  pinMode(BACK_PIN, INPUT_PULLUP);
  //u8g2.setI2CAddress(0x78);
  Wire.begin();
  //Wire.begin( 21, 22, 800000 );
  u8g2.begin(ENTER_PIN, R_PIN, L_PIN, U_PIN, D_PIN, BACK_PIN);
  u8g2.setBusClock(800000);
  u8g2.setContrast(112);
  u8g2.enableUTF8Print();
  u8g2.firstPage(); 
  do {
    u8g2.setFont(BASE_FONT);
    u8g2.drawXBM(0, 0, tm_logo_width, tm_logo_height, tm_logo_bits);
    u8g2.setFontMode(1);
    u8g2.drawUTF8(DISP_WIDTH - u8g2.getUTF8Width(VERSION), tm_logo_height - 2, VERSION);
  } while ( u8g2.nextPage() );
  u8g2.setAutoPageClear(0);
  //delay(500);
  //u8g2.sendBuffer();
  if ((digitalRead(SCAN_LOGIN_PIN) == LOW) && (digitalRead(SCAN_LOGOUT_PIN) == LOW)){
    u8g2.userInterfaceMessage(PSTR("Загружаем настройки"), PSTR("по умолчанию!"), "", " OK ");
  } else
    loadSettings();
  /*uint8_t num = WiFi.scanNetworks();
  for(int i=0; i < num; i++){
    char text[MENU_TEXT_SIZE+1];
    char hint[MENU_HINT_SIZE+1];
    sprintf(text, "%s%s", (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*", WiFi.SSID(i).c_str());
    sprintf(hint, "RSSI = %d", WiFi.RSSI(i));
    Serial.println(text);
    //AddMenuSubItem(&mInfo, text, hint, NULL, NULL, &mTrue, Enabled, {ptNone, 0, NULL});
  }*/
  //redrawScreen();
  //WiFi.disconnect(true);
  WiFi.setAutoReconnect(false);
  WiFi.onEvent(onConnectWiFi, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(onDisconnectWiFi, SYSTEM_EVENT_STA_DISCONNECTED);
  //WiFi.mode(WIFI_MODE_STA);
  changeMode(omStandby);
  //messageBox("Информация", "Сообщение", ICON_INFORMATION, alCenter);
  redrawScreen();
}

/*byte buttonOfPin(byte pin){
  for(byte i=0;i<NUM_BUTTONS;i++){
    if(buttons[i]==pin) return i;
  }
  return -1;
}*/

char *menuText(int8_t menuShift, uint16_t &icon, bool &is_disabled)
{
  int8_t i;
  Menu_Item_t *tempMenu = Menu_GetCurrentMenu();
  icon = 0;
  if ((void *) tempMenu == (void *)&NULL_MENU)
    return strNULL;

  Menu_Item_t *visibleMenu = menuPrevVisible(tempMenu);
  if ((void *)visibleMenu == (void *)&NULL_MENU){
    visibleMenu = menuNextVisible(tempMenu);
    if ((void *)visibleMenu == (void *)&NULL_MENU){
      return strNULL;
    }
  }
  if(tempMenu != visibleMenu) Menu_Navigate(visibleMenu);

  i = menuShift;

  if (i > 0)
  {
    while (i != 0)
    {
      if ((void *)visibleMenu != (void *)&NULL_MENU)
      {
        visibleMenu = menuNextVisible((Menu_Item_t *)(visibleMenu->Next));
      }
      i--;
    }
  }
  else
  {
    while (i != 0)
    {
      if ((void *)visibleMenu != (void *)&NULL_MENU)
      {
        visibleMenu = menuPrevVisible((Menu_Item_t *)visibleMenu->Previous);
      }
      i++;
    }
  }

  //Serial.printf("Menu_isVisible(visibleMenu)=%d\n", Menu_isVisible(visibleMenu));

  if ((void *)visibleMenu == (void *)&NULL_MENU)
  {
    return strNULL;
  }
  else
  {
    if(!Menu_isVisible(visibleMenu)) return strNULL;
    switch (visibleMenu->Param.Data.Type)
    {
    case ptNone:
      break;
    case ptBool:
      icon = *(bool *)visibleMenu->Param.Data.Pointer ? ICON_CHECK : ICON_UNCHECK;
      break;
    case ptInt:
    case ptDouble:
      icon = ICON_NUMBER;
      break;
    case ptString:
      icon = ICON_STRING;
      break;
    case ptHexString:
      icon = ICON_HEX_STRING;
      break;
    case ptEnum:
      icon = ICON_ENUM;
      break;
    case ptDate:
      icon = ICON_DATE;
      break;
    case ptTime:
      icon = ICON_TIME;
      break;
    default:
      icon = ICON_EDIT;
    }
    is_disabled = !Menu_isEnabled(visibleMenu);
    return ((char *)visibleMenu->Text);
  }
}

void drawCenterLine(u8g2_uint_t y, const char *s, uint16_t icon = 0, bool is_disabled = false, bool is_invert = false)
{
  u8g2_uint_t str_width, disp_width, x_offset = 0;
  u8g2_uint_t /*glyph_height = 0, */glyph_width = 0;
  int8_t line_height;
  //char glyph_buf[]="00\n";

  u8g2.setDrawColor(1);
  if (icon)
  {
    u8g2.setFont(ICON_FONT);
    //glyph_height = u8g2.getAscent() - u8g2.getDescent();
    glyph_width = ICON_WIDTH+1; //u8g2.getUTF8Width(glyph_buf);
  }
  u8g2.setFont(BASE_FONT);
  line_height = _max(u8g2.getAscent() - u8g2.getDescent(), 0 /*glyph_height*/);
  str_width = u8g2.getUTF8Width(s) + glyph_width;
  disp_width = u8g2.getDisplayWidth();

  if (str_width < disp_width)
  {
    x_offset = (disp_width - str_width) / 2;
  }
  if (is_disabled && is_invert)
  {
    for (int tx = 0; tx < disp_width / grid_width; tx++)
    {
      u8g2.drawXBM(tx * grid_width, y - line_height, grid_width, grid_height, grid_bits);
    }
    u8g2.setDrawColor(0);
  }
  else
    u8g2.setDrawColor(2);

  if (icon)
  {
    u8g2.setFont(ICON_FONT);
    u8g2.drawGlyph(x_offset, y, icon);
  }
  u8g2.setFont(BASE_FONT);
  u8g2.drawUTF8(x_offset + glyph_width, y + u8g2.getDescent(), s);

  if (!is_invert && is_disabled)
  {
    u8g2.setDrawColor(0);
    for (int ty = 0; ty < line_height; ty += 2)
    {
      u8g2.drawHLine(0, y - ty, u8g2.getDisplayWidth());
    }
  }
  if (is_invert && !is_disabled)
  {
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, y - line_height, disp_width, line_height);
  }
}

//TODO: Переделать вывод пунктов меню в цикл и по количеству помещающихся на экран !
void drawMenuScreen()
{
  u8g2.firstPage();
  do {
    u8g2.setFontRefHeightAll();
    u8g2.setFontMode(1);
    u8g2.setFont(u8g2_font_haxrcorp4089_t_cyrillic);
    int8_t line_height = u8g2.getAscent() - u8g2.getDescent();
    u8g2_uint_t y_offset = u8g2.getDisplayHeight() - line_height * 5; //(u8g2.getDisplayHeight() - line_height * 5 ) / 2;
    Menu_Item_t *tempMenu = MENU_PARENT;
    clearScreenEx(); //u8g2.clearBuffer();
    //char menuTitle[MENU_TEXT_SIZE];
    if (tempMenu != &NULL_MENU)
    {
      drawCenterLine(line_height, tempMenu->Text);
    }
    else
      drawCenterLine(line_height, PSTR("НАСТРОЙКИ"));

    u8g2.drawHLine(0, y_offset - 1, u8g2.getDisplayWidth());

    //char buf[25];

    char *text;
    uint16_t icon;
    bool disabled;

    text = menuText(0, icon, disabled);
    if (text)
      drawCenterLine(line_height * 3 + y_offset, text, icon, disabled, true);

    text = menuText(-2, icon, disabled);
    if (text)
      drawCenterLine(line_height + y_offset, text, icon, disabled);

    text = menuText(-1, icon, disabled);
    if (text)
      drawCenterLine(line_height * 2 + y_offset, text, icon, disabled);

    text = menuText(1, icon, disabled);
    if (text)
      drawCenterLine(line_height * 4 + y_offset, text, icon, disabled);

    text = menuText(2, icon, disabled);
    if (text)
      drawCenterLine(line_height * 5 + y_offset, text, icon, disabled);
  } while ( u8g2.nextPage() );
  //u8g2.sendBuffer();
}

void doScan(Menu_Item_t *current_item)
{
  delay(100);
  //u8g2.userInterfaceMessage("Сканирование...", "", "", "OK");
  struct tm timeinfo;
  /*
    tm_sec      seconds after the minute - [ 0 to 59 ]
    tm_min      minutes after the hour - [ 0 to 59 ]
    tm_hour     hours since midnight - [ 0 to 23 ]
    tm_mday     day of the month - [ 1 to 31 ]
    tm_wday     days since Sunday - [ 0 to 6 ]
    tm_mon      months since January - [ 0 to 11 ]
    tm_year     years since 1900
    tm_yday     days since January 1 - [ 0 to 365 ]
    tm_isdst    Daylight Saving Time flag *
  */
  const char *ntp_server = "pool.ntp.org";
  Serial.println("0");
  configTime(2 * 3600, 3600, ntp_server);
  Serial.println("1");
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println("2");
  //  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
  //rtc.set(timeinfo.tm_sec, timeinfo.tm_min, timeinfo.tm_hour, timeinfo.tm_wday, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void printDigits(u8g2_uint_t x, u8g2_uint_t y, unsigned long number, byte num_digits, bool lead_zero = false)
{
  char buf[num_digits + 1];
  char fmt_str[6] = "";

  sprintf_P(fmt_str, "%%%s%dd", lead_zero ? "0" : "", num_digits);
  int num_chars = sprintf_P(buf, fmt_str, number);
  for (int i = 0; i < num_chars; i++)
  {
    u8g2.drawGlyph(x + i * ICON_DIGIT_WIDTH, y, ICON_DIGIT + buf[i] - '0');
  }
}

uint8_t mapRSSI(int8_t rssi){
  if((rssi < -30) && (rssi >= -52)){
    return 4;
  }else if((rssi < -52) && (rssi >= -60)){
    return 3;
  }else if((rssi < -60) && (rssi >= -67)){
    return 2;
  }else if((rssi < -67) && (rssi >= -75)){
    return 1;
  }else{
    return 0;
  }
}

void drawStandbyScreen()
{
  //rtc.refresh();
  u8g2.setAutoPageClear(0);
  u8g2.firstPage();
  do {
    u8g2.setFontMode(0);
    u8g2.setDrawColor(1);
    u8g2.setFont(ICON_FONT);
    clearScreenEx(); 
    printDigits(0, ICON_HEIGHT, /*rtc.hour()*/0, 2, true);
    u8g2.drawGlyph(ICON_DIGIT_WIDTH * 2 - 2, ICON_HEIGHT, ICON_TIME_DELIM);
    printDigits(ICON_DIGIT_WIDTH * 2 + 4, ICON_HEIGHT, /*rtc.minute()*/0, 2, true);
    u8g2.setFont(ICON_FONT);
    uint8_t icon; 
    u8g2_uint_t x_offset = DISP_WIDTH - ICON_WIDTH;
    icon = map(batteryStatus, 0, 99, 0, 9) + ICON_BATTERY_0;
    u8g2.drawGlyph(x_offset, ICON_HEIGHT, icon); 
    x_offset -= ICON_WIDTH + 2;

    switch(wifiStatus){
      case wsDisconnected:
          icon = ICON_NO_WIFI;
        break;
      case wsConnecting:
          if(blynk) icon = ICON_NO_WIFI;
          else icon = 0;
          blynk = !blynk;
        break;
      case wsConnected:
          icon = mapRSSI((long)WiFi.RSSI()) + ICON_WIFI_0;
        break;
    }
    if(icon) 
      u8g2.drawGlyph(x_offset, ICON_HEIGHT, icon); 
    x_offset -= ICON_WIDTH + 2;

    if(true) icon = ICON_BLUETOOTH;
    u8g2.drawGlyph(x_offset, ICON_HEIGHT, icon); 
    x_offset -= ICON_WIDTH + 2;

    u8g2.drawHLine(0, ICON_HEIGHT, DISP_WIDTH);
    u8g2_uint_t line_height = u8g2.getAscent() - u8g2.getDescent();
    //drawCenterLine(DISP_HEIGHT / 2 + line_height / 2, DaysOfWeek.EnumItems[rtc.dayOfWeek()]);
    //u8g2.drawHLine(0, DISP_HEIGHT - 11, DISP_WIDTH);
    //drawCenterLine(DISP_HEIGHT + 2 , "НАСТРОЙКИ", ICON_ENTER);
  } while ( u8g2.nextPage() );
  u8g2.setAutoPageClear(1);
  //u8g2.sendBuffer();
}

char *getParamStringValue(Menu_Item *menuitem, char *buf)
{
  switch (menuitem->Param.Data.Type)
  {
  case ptString:
  case ptHexString:
    return (char *)menuitem->Param.Data.Pointer;
    break;
  case ptInt:
    sprintf(buf, "%d", *(int *)menuitem->Param.Data.Pointer);
    break;
  case ptDouble:
    sprintf(buf, "%.3f", *(double *)menuitem->Param.Data.Pointer);
    break;
  case ptEnum:
  {
    EnumStruct *enumData = (EnumStruct *)(menuitem->Param.Data.Size);
    uint8_t curVal = *(uint8_t*)Menu_ReadParam(menuitem);
    return (char *)&enumData->EnumItems[curVal];
  }
  break;
  default:
    break;
  }
  return buf;
}

void drawMenuScreenHint()
{
  Menu_Item_t *m = Menu_GetCurrentMenu();
  const char *hint_text;
  char buf[32];

  if ((m->Param.Data.Type != ptNone) && (m->Param.Data.Type != ptBool))
  {
    hint_text = getParamStringValue(m, &buf[0]);
  }
  else
    hint_text = &m->Hint[0];

  if (hint_text[0] == 0)
    return;

  u8g2_uint_t frame_offset = 5;
  u8g2_uint_t disp_width = u8g2.getDisplayWidth();
  u8g2_uint_t line_height = u8g2.getAscent() - u8g2.getDescent();
  u8g2_uint_t hint_width = u8g2.getUTF8Width(hint_text);
  u8g2_uint_t hint_clip_width = disp_width - frame_offset * 4;
  u8g2_uint_t frame_width = disp_width - frame_offset * 2;
  u8g2_uint_t frame_height = line_height + frame_offset * 2;
  u8g2_uint_t hint_x = frame_offset * 2;
  u8g2_uint_t hint_y = frame_offset * 2 + line_height + u8g2.getDescent();
  u8g2.setAutoPageClear(0);
  u8g2.firstPage(); 
  do {
    u8g2.setFont(BASE_FONT);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(0);
    if (hint_scrolling)
      u8g2.drawBox(hint_x, frame_offset * 2, hint_clip_width, line_height);
    else
      u8g2.drawRBox(frame_offset, frame_offset, frame_width, frame_height, 7);
    u8g2.setDrawColor(1);
    if (!hint_scrolling)
      u8g2.drawRFrame(frame_offset, frame_offset, frame_width, frame_height, 7);
    u8g2.setClipWindow(hint_x, hint_y - line_height, hint_x + hint_clip_width, hint_y - u8g2.getDescent());
    u8g2.drawUTF8(hint_x + hint_x_offset, hint_y, hint_text);
    u8g2.setMaxClipWindow();
  } while ( u8g2.nextPage() );
  u8g2.setAutoPageClear(1);
  //u8g2.sendBuffer();
  if (hint_width > hint_clip_width)
  {
    hint_scrolling = true;
    if (hint_scroll_left)
    {
      hint_x_offset--;
      if (hint_width + hint_x_offset == hint_clip_width - 2)
        hint_scroll_left = !hint_scroll_left;
    }
    else
    {
      hint_x_offset++;
      if (hint_x_offset == 2)
        hint_scroll_left = !hint_scroll_left;
    }
  }
  else
    hint_scrolling = false;
}

void drawCaptions()
{
  u8g2.setFont(BASE_FONT);
  int count = CurrentCaptions.Size / sizeof(Caption);
  if (CurMode == omEdit)
  {
    if ((CurParam.Type != ptString) && (CurParam.Type != ptHexString))
      count--;
  }
  u8g2_uint_t caption_width = count * (ICON_WIDTH + 2);
  for (int i = 0; i < count; i++)
  {
    caption_width += u8g2.getUTF8Width(CurrentCaptions.Captions[i].text);
  }

  u8g2_uint_t line_height = u8g2.getAscent() - u8g2.getDescent();

  /*u8g2.setAutoPageClear(0);
  u8g2.firstPage(); 
  do {*/
    u8g2.setFontMode(0);
    u8g2.setDrawColor(0);
    if (caption_scrolling)
      u8g2.drawBox(0, DISP_HEIGHT - CAPTIONS_HEIGHT - 1, DISP_WIDTH, CAPTIONS_HEIGHT + 1);
    u8g2.setDrawColor(1);
    u8g2.drawHLine(0, DISP_HEIGHT - CAPTIONS_HEIGHT - 2, DISP_WIDTH);
    //u8g2.setClipWindow(0, DISP_HEIGHT - CAPTIONS_HEIGHT - 1, DISP_WIDTH, DISP_HEIGHT + 1);
    int x_offset = 0;
    for (int i = 0; i < count; i++)
    {
      u8g2.setFont(ICON_FONT);
      u8g2.drawGlyph(caption_x_offset + x_offset, DISP_HEIGHT, CurrentCaptions.Captions[i].icon);
      x_offset += ICON_WIDTH+1;
      u8g2.setFont(BASE_FONT);
      x_offset += u8g2.drawUTF8(caption_x_offset + x_offset, DISP_HEIGHT - (CAPTIONS_HEIGHT - line_height) / 2 - 2/*+ u8g2.getDescent()*/ , CurrentCaptions.Captions[i].text) + 3;
    }
    //u8g2.setMaxClipWindow();
  //} while ( u8g2.nextPage() );
  //u8g2.setAutoPageClear(1);
  ////u8g2.sendBuffer();
  uint8_t tile_y = (DISP_HEIGHT - CAPTIONS_HEIGHT - 2) / 8;
  uint8_t tile_height = u8g2.getBufferTileHeight() - tile_y;
  u8g2.updateDisplayArea(0, tile_y, u8g2.getBufferTileWidth(), tile_height);
  if (caption_width > DISP_WIDTH)
  {
    caption_scrolling = true;
    if (caption_scrolling_left)
    {
      caption_x_offset--;
      if (caption_width + caption_x_offset == DISP_WIDTH - 2)
        caption_scrolling_left = !caption_scrolling_left;
    }
    else
    {
      caption_x_offset++;
      if (caption_x_offset == 2)
        caption_scrolling_left = !caption_scrolling_left;
    }
  }
  else
    caption_scrolling = false;
}

void drawEditScreen()
//TODO: Для чисел выводить границы
{
  u8g2.firstPage(); 
  do {
    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
    u8g2.setFont(BASE_FONT);
    u8g2_uint_t line_height = u8g2.getAscent() - u8g2.getDescent();
    //clearScreenEx(); 
    drawCenterLine(line_height, CurParam.MenuItem->Text, ICON_EDIT);
    u8g2_uint_t y_offset = line_height * 2;
    u8g2.setFont(EDIT_FONT);
    line_height = u8g2.getAscent() - u8g2.getDescent();
    switch (CurParam.Type)
    {
    case ptString:
    case ptHexString:
    case ptInt:
    case ptDouble:
      char tmp[2];
      tmp[1] = 0;
      byte val_full_len = CurParam.strLen;
      byte val_len = strlen(CurParam.strVal);
      byte end_cols = val_full_len % BASE_LINE_CHARS;
      byte rows = val_full_len / BASE_LINE_CHARS + (end_cols ? 1 : 0);
      //Serial.printf("CurParam.strVal=%s, val_full_size=%d, val_len=%d, rows=%d, end_cols=%d, BASE_LINE_CHARS=%d\n", CurParam.strVal, val_full_len, val_len, rows, end_cols, BASE_LINE_CHARS);
      for (byte y = 0; y < rows; y++)
      {
        byte max_pos = (y == rows - 1 ? (end_cols ? end_cols : BASE_LINE_CHARS) : BASE_LINE_CHARS);
        for (byte x = 0; x < max_pos; x++)
        {
          byte cur_char = y * BASE_LINE_CHARS + x;
          if (cur_char < val_len)
          {
            tmp[0] = CurParam.strVal[cur_char];
            u8g2.drawUTF8(x * BASE_SYMBOL_WIDTH + 1, y_offset + y * line_height - 1, tmp);
          }
          else
          {
            for (int z = 0; z < 3; z++)
              u8g2.drawPixel(x * BASE_SYMBOL_WIDTH + z * 2 + 1, y_offset + y * line_height);
          }
          if (CurParam.invert_cursor && cur_char == CurParam.cursor_pos)
          {
            u8g2.setDrawColor(2);
            u8g2.drawBox(x * BASE_SYMBOL_WIDTH, y_offset + (y - 1) * line_height, BASE_SYMBOL_WIDTH, line_height);
          }
        }
      }
      break;
    }
  } while ( u8g2.nextPage() );
  //u8g2.sendBuffer();
  CurParam.invert_cursor = !CurParam.invert_cursor;
}

void redrawScreen()
{
  switch (CurMode)
  {
    case omStandby:
        drawStandbyScreen();
      break;
    case omMenuList:
        drawMenuScreen();
      break;
    case omMenuHint:
        drawMenuScreenHint();
      break;
    case omEdit:
        drawEditScreen();
      break;
    case omMessageBox:
        drawMessageBox();
      break;
  }
}

void resetHintDelay()
{
  hint_millis = millis() + HINT_DELAY;
}

bool checkConstraints()
{
  int iTmp;
  double dTmp;
  errno = 0;
  switch (CurParam.Type)
  {
  case ptInt:
    iTmp = strtol(CurParam.strVal, NULL, 10);
    //Serial.printf("ptInt: strVal=%s, iTmp=%d, min=%d, max=%d\n", CurParam.strVal, iTmp, (int)CurParam.Value->Constraints.minVal, (int)CurParam.Value->Constraints.maxVal);
    return (iTmp >= (int)CurParam.Constraints.minVal && iTmp <= (int)CurParam.Constraints.maxVal && errno != ERANGE);
    break;
  case ptDouble:
    dTmp = strtof(CurParam.strVal, NULL);
    return (dTmp >= CurParam.Constraints.minVal && dTmp <= CurParam.Constraints.maxVal && errno != ERANGE);
    break;
  }
  return true;
}

void buttonPressed(byte button, bool repeated = false)
{
  Serial.print(button); if(repeated) Serial.print(" repeated");Serial.print(", CurMode=");Serial.println(CurMode);
  switch (CurMode)
  {
  case omStandby:
    switch (button)
    {
    case ENTER_PIN:
      changeMode(omMenuList); //CurMode = omMenuList;
      Menu_Navigate(&MENU_FIRST_ITEM);
      resetHintDelay();
      redrawScreen();
      break;
    case SCAN_LOGIN_PIN:
      Serial.println("Сканирование входа");
      if (!scanner.startScan(SCAN_LOGIN))
      {
        u8g2.userInterfaceMessage("Ошибка сканирования", "", "", " OK ");
      }
      break;
    case SCAN_LOGOUT_PIN:
      Serial.println("Сканирование выхода");
      if (!scanner.startScan(SCAN_LOGOUT))
      {
        u8g2.userInterfaceMessage("Ошибка сканирования", "", "", " OK ");
      }
      break;
    }
    break;
  case omMenuList:
    Menu_Item_t *tmp_menu;
    switch (button)
    {
    case L_PIN:
    case BACK_PIN:
      tmp_menu = MENU_PARENT;
      if ((tmp_menu == &NULL_MENU) || (tmp_menu == NULL))
        changeMode(omStandby); 
      else
        Menu_Navigate(tmp_menu);
      break;
    case U_PIN:
      Menu_Navigate(menuPrevVisible(MENU_PREVIOUS));
      break;
    case D_PIN:
      Menu_Navigate(menuNextVisible(MENU_NEXT));
      break;
    case ENTER_PIN:
    case R_PIN:
      if (!Menu_isEnabled(Menu_GetCurrentMenu()))
        return;
      tmp_menu = MENU_CHILD;
      if ((tmp_menu == &NULL_MENU) || (tmp_menu == NULL))
        Menu_EnterCurrentItem();
      else
        Menu_Navigate(menuNextVisible(tmp_menu));
      break;
    }
    resetHintDelay();
    break;
  case omMenuHint:
    changeMode(omMenuList); //CurMode = omMenuList;
    redrawScreen();
    buttonPressed(button, repeated);
    break;
  case omEdit: {
    CurParam.invert_cursor = true;
    byte val_len = strlen(CurParam.strVal);
    char prevChar = CurParam.strVal[CurParam.cursor_pos];
    bool emptyChar = !prevChar;
    int charsetPos = 0;
    if (!emptyChar)
      charsetPos = strchr(CurParam.charset, prevChar) - CurParam.charset;
    //Serial.printf("charset=%s\ncharsetPos=%d, CurParam.cursor_pos=%d, CurParam.strVal[CurParam.cursor_pos]=%c, emptyChar=%d\n", CurParam.charset, charsetPos, CurParam.cursor_pos, CurParam.strVal[CurParam.cursor_pos], emptyChar);
    switch (button)
    {
      case SCAN_LOGOUT_PIN:
        val_len = _max(0, val_len - 1);
        Serial.printf("val_len=%d, CurParam.Size=%d\n", val_len, CurParam.Size);
        memset(&CurParam.strVal[val_len], 0, CurParam.Size-val_len);
        CurParam.cursor_pos = val_len;
        break;
      case SCAN_LOGIN_PIN:
        if (!scanner.startScan(SCAN_PARAM))
        {
          u8g2.userInterfaceMessage("Ошибка сканирования", "", "", " OK ");
        }
        break;
      case R_PIN:
        CurParam.cursor_pos++;
        if (CurParam.Type == ptDouble)
        {
          if (CurParam.cursor_pos == CurParam.strLen - 4)
            CurParam.cursor_pos++;
        }
        if (CurParam.cursor_pos > _min(val_len, CurParam.strLen - 1))
        {
          CurParam.cursor_pos = 0;
        }
        break;
      case L_PIN:
        CurParam.cursor_pos--;
        if (CurParam.Type == ptDouble)
        {
          if (CurParam.cursor_pos == CurParam.strLen - 4)
            CurParam.cursor_pos--;
        }
        if (CurParam.cursor_pos < 0)
        {
          CurParam.cursor_pos = val_len - 1;
        }
        break;
      case U_PIN:
        if (IS_SIGNED(CurParam) && (CurParam.cursor_pos == 0))
        {
          CurParam.strVal[CurParam.cursor_pos] = (prevChar == '-' ? '+' : '-');
        }
        else
        {
          if (emptyChar)
            charsetPos = 0;
          else
            charsetPos++;
          if (charsetPos == strlen(CurParam.charset))
            charsetPos = 0;
          CurParam.strVal[CurParam.cursor_pos] = CurParam.charset[charsetPos];
        }
        break;
      case D_PIN:
        if (IS_SIGNED(CurParam) && (CurParam.cursor_pos == 0))
        {
          CurParam.strVal[CurParam.cursor_pos] = (prevChar == '-' ? '+' : '-');
        }
        else
        {
          if (emptyChar)
            charsetPos = -1;
          else
            charsetPos--;
          if (charsetPos < 0)
            charsetPos = strlen(CurParam.charset) - 1;
          CurParam.strVal[CurParam.cursor_pos] = CurParam.charset[charsetPos];
        }
        break;
      case ENTER_PIN:
        saveParam();
        changeMode(CurParam.prevMode);
        redrawScreen();
        return;
      case BACK_PIN:
        changeMode(CurParam.prevMode); 
        break;
      }
      if (!checkConstraints())
        CurParam.strVal[CurParam.cursor_pos] = prevChar;
    }
    break;
  case omMessageBox:
    switch (button){
      case ENTER_PIN:
          mbInfo.message.clear();
          changeMode(mbInfo.prevMode);
        break;
      case U_PIN:
          if(mbInfo.top_line > 0) 
            mbInfo.top_line--;
          //redrawScreen();
        break;
      case D_PIN:
          if(mbInfo.top_line < mbInfo.message.size() - mbInfo.screen_lines) 
            mbInfo.top_line++;
          //redrawScreen();
        break;
    }
    break;
  case omSelectList:

    break;
  }
  redrawScreen();
}

void checkKeyPress()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    int reading = digitalRead(buttons[i]);
    if (reading == HIGH)
    {
      buttonDown[i] = false;
      lastRepeatTime[i] = 0;
    }
    if (reading != lastButtonState[i])
    {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY)
    {
      if (reading != buttonState[i])
      {
        buttonState[i] = reading;

        if (buttonState[i] == LOW)
        {
          buttonDown[i] = true;
          lastRepeatTime[i] = millis();
          buttonPressed(buttons[i]);
        }
      }
      if (((CurMode != omMenuList) || (i < 2)) && ((millis() - lastRepeatTime[i]) > REPEAT_DELAY) && (buttonDown[i]))
      {
        buttonPressed(buttons[i], true);
        lastRepeatTime[i] = lastRepeatTime[i] + REPEAT_FREQUENCY;
      }
    }
    lastButtonState[i] = reading;
  }
}

void onScanDone(char *barcode, long mode)
{
  char mode_str[20];
  switch (mode)
  {
  case SCAN_LOGIN:
    strcpy(mode_str, "для ВХОДА");
    break;
  case SCAN_LOGOUT:
    strcpy(mode_str, "для ВЫХОДА");
    break;
  }
  u8g2.userInterfaceMessage("Получен код", mode_str, barcode, "OK");
}

void loop()
{
  unsigned long m = millis();
  unsigned long interval = DISPLAY_UPDATE_INTERVAL;
  switch (CurMode)
  {
    case omMenuHint:
      if (hint_scrolling)
        interval = HINT_UPDATE_INTERVAL;
      break;
    case omEdit:
      interval = CURSOR_INTERVAL;
      break;
    case omStandby:
        if(wifiStatus == wsConnecting) interval = CURSOR_INTERVAL;
      break;
  }
  if (m - last_m > interval)
  {
    last_m = m;
    redrawScreen();
  }
  if ((m > hint_millis) && (CurMode == omMenuList))
  {
    changeMode(omMenuHint); 
    redrawScreen();
  }
  if ((m - last_m_caption > CAPTION_UPDATE_INTERVAL) && (CurrentCaptions.Captions != NULL))
  {
    last_m_caption = m;
    drawCaptions();
  }
  checkKeyPress();
  scanner.loop();
}