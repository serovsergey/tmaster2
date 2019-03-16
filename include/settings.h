// TODO: Хранение настроек в NVS
// TODO: Хранение параметров WiFi-сетев в NVS
// TODO: Различные хранилища настроек
//#include "nvs_func.cpp"
/*
-30 dBm	Maximum signal strength, you are probably standing right next to the access point.	
-50 dBm	Anything down to this level can be considered excellent signal strength.	
-60 dBm	Good, reliable signal strength.	
-67 dBm	Reliable signal strength.	The minimum for any service depending on a reliable connection and signal strength, such as voice over Wi-Fi and non-HD video streaming.
-70 dBm	Not a strong signal.	Light browsing and email.
-80 dBm	Unreliable signal strength, will not suffice for most services.	Connecting to the network.
-90 dBm	The chances of even connecting are very low at this level.	
*/
#define APP_NAME "TicketMaster"
#define VERSION PSTR("Ver 1.0")

#define EEPROM_ADDRESS 0x50

extern U8G2_HX1230_96X68_F_3W_SW_SPI u8g2;
//extern U8G2_SSD1327_MIDAS_128X128_F_HW_I2C u8g2;
//uEEPROMLib eeprom(EEPROM_ADDRESS);
#define API_KEY_SIZE 36
///////////////////////// Settings /////////////////////////
bool wifiAutoConnect = true;
bool wifiAutoReconnect = true;
double dval = 4589.234;
long int ival1 = 45;
long int ival2 = 16;
long int ival3 = 6;
bool timeStartupSync = false;
char ApiKey[API_KEY_SIZE + 1] = "";
////////////////////////////////////////////////////////////
#define _COMMA ,
#define DEFINE_ENUM(Name, Items)             \
  const EnumItem PROGMEM ei##Name[] = Items; \
  EnumStruct PROGMEM Name = {sizeof(ei##Name) / sizeof(EnumItem), &ei##Name[0]};
////////////////////// Enums ///////////////////////////////
DEFINE_ENUM(DaysOfWeek, {"Воскресенье" _COMMA
                       "Понедельник" _COMMA
                       "Вторник" _COMMA
                       "Среда" _COMMA
                       "Четверг" _COMMA
                       "Пятница" _COMMA
                       "Суббота"})
////////////////////////////////////////////////////////////

#define MAKE_STORED_PARAM(name, type, param) \
  {                               \
    name, {type, sizeof(param), &param}   \
  }

struct StoredParam {
  char name[16];
  Param_Data data;
};

struct WiFiAuth{
  char SSID[SSID_SIZE + 1];
  char Password[WIFI_PASS_SIZE + 1];
};

typedef std::vector<WiFiAuth> WiFiAuthList;

const StoredParam PROGMEM params[] = {
    MAKE_STORED_PARAM("AutoConnect", ptBool, wifiAutoConnect),
    MAKE_STORED_PARAM("AutoReconnect", ptBool, wifiAutoReconnect),
    MAKE_STORED_PARAM("timeStartupSync", ptBool, timeStartupSync),
    MAKE_STORED_PARAM("ival1", ptInt, ival1),
    MAKE_STORED_PARAM("ival2", ptInt, ival2),
    MAKE_STORED_PARAM("ival3", ptInt, ival3),
    MAKE_STORED_PARAM("dval", ptDouble, dval),
    MAKE_STORED_PARAM("ApiKey", ptHexString, ApiKey)};

int saveSettings()
{
  int errors = 0;
  nvs_handle hSettings;
	esp_err_t err = nvs_open("settings", NVS_READWRITE, &hSettings);
	if (err != ESP_OK) return -1;
  for (int i = 0; i < sizeof(params) / sizeof(StoredParam); i++)
  {
    err = nvs_set_blob(hSettings, params[i].name, params[i].data.Pointer, params[i].data.Size);
    if(err != ESP_OK) {
      errors++;
      u8g2.userInterfaceMessage("Параметр", params[i].name, "не записан!"," OK ");
    }
  }
  nvs_commit(hSettings);
  nvs_close(hSettings);
  u8g2.userInterfaceMessage("Настройки", "записаны", "", " OK ");
  return errors;
}

int loadSettings()
{
  int errors = 0;
  size_t blob_size;
  u8g2.setCursor(0, u8g2.getDisplayHeight());
  //u8g2.setFont(BASE_FONT);
  nvs_handle hSettings;
	esp_err_t err = nvs_open("settings", NVS_READONLY, &hSettings);
	if (err != ESP_OK) return -1;
  for (int i = 0; i < sizeof(params) / sizeof(StoredParam); i++)
  {
    blob_size = params[i].data.Size;
    err = nvs_get_blob(hSettings, params[i].name, params[i].data.Pointer, &blob_size);
    if(err != ESP_OK) {
      Serial.println(err);
      errors++;
      u8g2.userInterfaceMessage("Параметр", params[i].name, "не загружен!"," OK ");
    }
    else if(params[i].data.Size != blob_size){
      errors++;
      u8g2.userInterfaceMessage("Неверный размер", "параметра", params[i].name," OK ");
    }
    u8g2.print(".");
    u8g2.sendBuffer();
  }
  nvs_close(hSettings);
  return errors;
}

esp_err_t writeAuthInfo(WiFiAuthList *AuthList){
  nvs_handle hAuthData;
	esp_err_t err = nvs_open("wifi_auth", NVS_READWRITE, &hAuthData);
	if (err != ESP_OK) return err;
  nvs_erase_all(hAuthData);
  char name[5];
  for(int i=0; i < AuthList->size(); i++){
    snprintf(name, 5, "_%d", i);
    err = nvs_set_blob(hAuthData, name, &AuthList[i], sizeof(AuthList[i]));
    if(err != ESP_OK) return err;
  }
  nvs_set_i16(hAuthData, "size", AuthList->size());
  nvs_commit(hAuthData);
  nvs_close(hAuthData); 
  return ESP_OK;
}
//ESP_ERR_NVS_NOT_FOUND
esp_err_t readAuthInfo(WiFiAuthList *AuthList){
  nvs_handle hAuthData;
	esp_err_t err = nvs_open("wifi_auth", NVS_READONLY, &hAuthData);
	if (err != ESP_OK) return err;
  int16_t size;
  err = nvs_get_i16(hAuthData, "", &size);
  if (err != ESP_OK) return err;
  if(size){
    char name[5];
    WiFiAuth auth_data;
    AuthList->clear();
    for(int i=0; i < size; i++){
      snprintf(name, 5, "_%d", i);
      size_t size;
      err = nvs_get_blob(hAuthData, name, &auth_data, &size);
      if (err != ESP_OK) return err;
      if (size != sizeof(auth_data)) return -1;
      AuthList->push_back(auth_data);
    }
  }
  return ESP_OK;
}