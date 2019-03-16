//#include "Arduino.h"
#include <ETH.h>
#include <WiFi.h> //24:0A:C4:97:13:58
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
//#include <Preferences.h>
#include "nvs.h"
#include <WiFiScan.h>

//#include <uEEPROMLib.h>
#include <uRTCLib.h> //ds1307
//#include <AT24CX.h> //at24c32
#include <U8g2lib.h> //#define U8G2_16BIT in u8g2.h
#include "time.h"
#include "e1007.h"
#include "MicroMenu.h"

#define RTC_ADDRESS 0x68
#define BASE_FONT u8g2_font_haxrcorp4089_t_cyrillic
#define EDIT_FONT u8g2_font_5x8_t_cyrillic
#define ICON_FONT tm_icons_12x12
#define BASE_SYMBOL_WIDTH 8
#define ICON_WIDTH 12
#define ICON_HEIGHT 12
#define ICON_DIGIT_WIDTH 8
#define CAPTIONS_HEIGHT ICON_HEIGHT
#define MB_MIN_BORDER 2
#define MB_MAX_WIDTH DISP_WIDTH - MB_MIN_BORDER
#define MB_DELIMETERS " "
//#define RFRAME_RADIUS     7

#define ICON_WARNING      33
#define ICON_INFORMATION  34
#define ICON_QUESTION     35
#define ICON_UNCHECK      36
#define ICON_CHECK        37

#define ICON_LEFTRIGHT    38
#define ICON_UPDOWN       39
#define ICON_LOGIN        40
#define ICON_LOGOUT       41
#define ICON_ENTER        42
#define ICON_BACK         43
#define ICON_LEFT         44
#define ICON_RIGHT        45
#define ICON_UP           46
#define ICON_DOWN         47

#define ICON_DIGIT        48
#define ICON_TIME_DELIM   58

#define ICON_NO_WIFI      59
#define ICON_WIFI_0       60
#define ICON_BLUETOOTH    65
#define ICON_BATTERY_0    66
#define ICON_CHARGING     76

#define ICON_DOTS         77
#define ICON_NUMBER       78
#define ICON_STRING       79
#define ICON_HEX_STRING   80
#define ICON_ENUM         81
#define ICON_TIME         82
#define ICON_DATE         83
#define ICON_EDIT         84

#define DISP_WIDTH        u8g2.getDisplayWidth()
#define DISP_HEIGHT       u8g2.getDisplayHeight()

#define BASE_LINE_CHARS   (DISP_WIDTH / BASE_SYMBOL_WIDTH)

#define R_PIN             35
#define L_PIN             32
#define U_PIN             33
#define D_PIN             34
#define ENTER_PIN         36
#define BACK_PIN          15
#define SCAN_LOGIN_PIN    26
#define SCAN_LOGOUT_PIN   25

#define SCAN_LOGIN 1
#define SCAN_LOGOUT 2
#define SCAN_PARAM 3

#define WIFI_RECONNECT_ATTEMPTS 20

#define DEBOUNCE_DELAY          50
#define REPEAT_DELAY            500
#define REPEAT_FREQUENCY        100
#define DISPLAY_UPDATE_INTERVAL 5000
#define HINT_UPDATE_INTERVAL    100
#define CAPTION_UPDATE_INTERVAL HINT_UPDATE_INTERVAL
#define HINT_DELAY              2000
#define CURSOR_INTERVAL         500

#define MAX_STRING_SIZE         60
#define SSID_SIZE               32
#define WIFI_PASS_SIZE          60 //64

enum OperatinalModes{
  omStandby,
  omMenuList,
  omEdit,
  omMenuHint,
  omSelectList,
  omMessageBox
};

enum StandbySubModes{
  ssmNormal,
  ssmMessage
};

/*struct NetworkParams{
  char SSID[SSID_SIZE + 1];
  char Password[WIFI_PASS_SIZE + 1];
};

NetworkParams wifiNetworks[3] = {{"SEROV", "56p4-qdmk-v9kd"}, {"GRAVIS", "zqyv-dx39-7ts6"}, {"OnePlus3", "ssa260579"}};*/
//std::vector <NetworkParams> wifiNetworks;
char PROGMEM string_chars[] = " !\"#$%&\'()*+,-./0123456789:;<=>?[\\]^_`@{|}~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
char PROGMEM hex_chars[] = "0123456789ABCDEF";
char PROGMEM num_chars[] = "0123456789";

enum wifiStatuses{
  wsDisconnected,
  wsConnecting,
  wsConnected
};

////////////////////////// Status //////////////////////////
wifiStatuses wifiStatus = wsDisconnected;
bool wifiConnecting = false;
bool btEnabled = false;
unsigned char batteryStatus = 64;
bool blynk = false;
////////////////////////////////////////////////////////////

unsigned long hint_millis = 0;
int hint_x_offset = 0;
bool hint_scroll_left = true;
bool hint_scrolling = false;

int caption_x_offset = 0;
bool caption_scrolling_left = true;
bool caption_scrolling = false;

byte buttons[] = {U_PIN, D_PIN, R_PIN, L_PIN, ENTER_PIN, BACK_PIN, SCAN_LOGIN_PIN, SCAN_LOGOUT_PIN};
#define NUM_BUTTONS sizeof(buttons)
int buttonState[NUM_BUTTONS];
int lastButtonState[NUM_BUTTONS];
unsigned long lastDebounceTime[NUM_BUTTONS];
unsigned long lastRepeatTime[NUM_BUTTONS];
bool buttonDown[NUM_BUTTONS];

unsigned long last_m = 0;
unsigned long last_m_caption = 0;
char strNULL[] PROGMEM = "";
uint8_t reconnect_attempts;
uint8_t dayOfWeek;
char currSSID[SSID_SIZE+1];

struct Caption
{
  uint16_t icon;
  char text[60];
};

struct CaptionsData
{
  const Caption *Captions;
  size_t Size;
} CurrentCaptions;

#define CHANGE_CURRENT_CAPTIONS(NewCaptions)    \
  {                                             \
    CurrentCaptions.Captions = &NewCaptions[0]; \
    CurrentCaptions.Size = sizeof(NewCaptions); \
  }

const Caption PROGMEM EditCaptions[] = {
    {ICON_LEFTRIGHT, "Курсор"},
    {ICON_UPDOWN, "Символ"},
    {ICON_ENTER, "Сохранить"},
    {ICON_BACK, "Отменить"},
    {ICON_LOGOUT, "Backspace"},
    {ICON_LOGIN, "Сканировать"}};

const Caption PROGMEM StandbyCaptions[] = {
    {ICON_LOGIN, "Вход"},
    {ICON_LOGOUT, "Выход"},
    {ICON_ENTER, "Настройки"}};

struct
{
  param_type Type;
  union {
    size_t Size;
    NumConstraints Constraints;
  };
  char strVal[MAX_STRING_SIZE + 1];
  byte strLen;
  Menu_Item_t *MenuItem;
  OperatinalModes prevMode;
  char *charset;
  int cursor_pos;
  bool invert_cursor;
} CurParam;

enum mbAlign{
  alLeft,
  alCenter,
  alRight
};

struct {
  std::vector<String> message;
  char title[32];
  uint16_t icon;
  u8g2_uint_t width;
  u8g2_uint_t height;
  long timeout;
  u8g2_uint_t line_height;
  uint8_t top_line;
  uint8_t max_lines;
  uint8_t screen_lines;
  mbAlign align;
  OperatinalModes prevMode;
  void (*onClose)(void);
} mbInfo;

OperatinalModes CurMode;

int popcount(int v) {
    v = v - ((v >> 1) & 0x55555555);                // put count of each 2 bits into those 2 bits
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333); // put count of each 4 bits into those 4 bits  
    return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

#include "settings.h"
#include "menuitems.h"
#include "graphics.h"
#include "tm_icons.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif