#pragma once

#include "HSDConfigFile.hpp"
#include "PreAllocatedLinkedList.hpp"

#define JSON_KEY_HOST                  (F("host"))
#define JSON_KEY_WIFI_SSID             (F("wifiSSID"))
#define JSON_KEY_WIFI_HOSTNAME         (F("wifiHostname"))
#define JSON_KEY_WIFI_PSK              (F("wifiPSK"))
#define JSON_KEY_MQTT_SERVER           (F("mqttServer"))
#define JSON_KEY_MQTT_USER             (F("mqttServerUser"))
#define JSON_KEY_MQTT_PASS             (F("mqttServerPass"))
#define JSON_KEY_MQTT_STATUS_TOPIC     (F("mqttStatusTopic"))
#define JSON_KEY_MQTT_TEST_TOPIC       (F("mqttTestTopic"))
#define JSON_KEY_MQTT_WILL_TOPIC       (F("mqttWillTopic"))
#define JSON_KEY_LED_COUNT             (F("ledCount"))
#define JSON_KEY_LED_BRIGHTNESS        (F("ledBrightness"))
#define JSON_KEY_COLORMAPPING_MSG      (F("m"))
#define JSON_KEY_COLORMAPPING_TYPE     (F("t"))
#define JSON_KEY_COLORMAPPING_COLOR    (F("c"))
#define JSON_KEY_COLORMAPPING_BEHAVIOR (F("b"))
#define JSON_KEY_DEVICEMAPPING_NAME    (F("n"))
#define JSON_KEY_DEVICEMAPPING_TYPE    (F("t"))
#define JSON_KEY_DEVICEMAPPING_LED     (F("l"))

class HSDConfig
{
  
public:

  static const int MAX_DEVICE_MAPPING_NAME_LEN = 25;
  static const int MAX_COLOR_MAPPING_MSG_LEN = 15;
  
  /*
   * Enum which defines the types of devices which can send messages.
   * If the same message (e.g. "on") can be received from different types
   * of devices (e.g. light and alarm), different reaction can be done.
   */
  enum deviceType
  {
    TYPE_WINDOW,
    TYPE_DOOR,
    TYPE_LIGHT,
    TYPE_ALARM,
    TYPE_UNKNOWN
  };

  enum Behavior
  {
    OFF,
    ON,
    BLINKING,
    FLASHING,
    FLICKERING
  };

  enum Color
  {
    NONE   = 0x000000,
    GREEN  = 0x00FF00,
    YELLOW = 0xFFFF00,
    ORANGE = 0xFF5500,
    RED    = 0xFF0000,
    PURPLE = 0xFF00FF,
    BLUE   = 0x0000FF,
    WHITE  = 0xFFFFFF
  };

  struct ColorTranslator
  {
    Color color;
    uint32_t id; 
  };

  /*
   * This struct is used for mapping a device of a specific device type 
   * to a led number, that means a specific position on the led stripe
   */
  struct DeviceMapping
  { 
    DeviceMapping()
    {
      memset(name, 0, MAX_DEVICE_MAPPING_NAME_LEN);
      type = TYPE_UNKNOWN;
      ledNumber = 0;       
    }

    DeviceMapping(String n, deviceType t, int l)
    {
      strncpy(name, n.c_str(), MAX_DEVICE_MAPPING_NAME_LEN);
      name[MAX_DEVICE_MAPPING_NAME_LEN] = '\0';
      type = t;
      ledNumber = l;   
    }
    
    char name[MAX_DEVICE_MAPPING_NAME_LEN]; // name of the device
    deviceType type;                        // type of the device
    int ledNumber;                          // led number on which reactions for this device are displayed
  };

  /*
   * This struct is used for mapping a message for a specific device
   * type to a led behavior (see LedSwitcher::ledState).
   */
  struct ColorMapping
  {
    ColorMapping()
    {
      memset(msg, 0, MAX_COLOR_MAPPING_MSG_LEN);
      type = TYPE_UNKNOWN;
      color = NONE;
      behavior = OFF;
    }
    
    ColorMapping(String m, deviceType t, Color c, Behavior b)
    {
      strncpy(msg, m.c_str(), MAX_COLOR_MAPPING_MSG_LEN);
      msg[MAX_COLOR_MAPPING_MSG_LEN] = '\0';
      type = t;
      color = c;
      behavior = b;
    }
    
    char msg[MAX_COLOR_MAPPING_MSG_LEN+1];  // message 
    deviceType type;                        // type of the device
    Color color;                            // led color for message from device type
    Behavior behavior;                      // led behavior for message from device type
  };

  HSDConfig();

  void begin(const char* version, const char* defaultIdentifier);

  void saveMain();
  
  void saveColorMapping();
  void updateColorMapping();
  
  void saveDeviceMapping();
  void updateDeviceMapping();

  const char* getVersion() const;
  bool setVersion(const char* version);

  const char* getHost() const;
  bool setHost(const char* host);

  const char* getWifiSSID() const;
  bool setWifiSSID(const char* ssid);
  
  const char* getWifiHostname() const;
  bool setWifiHostname(const char* hostname);

  const char* getWifiPSK() const;
  bool setWifiPSK(const char* psk);

  const char* getMqttServer() const;
  bool setMqttServer(const char* ip);

  const char* getMqttServerUser() const;
  bool setMqttServerUser(const char* mqttuser);

  const char* getMqttServerPass() const;
  bool setMqttServerPass(const char* mqttpass);

  const char* getMqttStatusTopic() const;
  bool setMqttStatusTopic(const char* topic);

  const char* getMqttTestTopic() const;
  bool setMqttTestTopic(const char* topic);

  const char* getMqttWillTopic() const;
  bool setMqttWillTopic(const char* topic);

  int getNumberOfLeds() const;
  bool setNumberOfLeds(uint32_t numberOfLeds);

  uint8_t getLedBrightness() const;
  bool setLedBrightness(uint8_t brightness);

  void resetMainConfigData();
  void resetDeviceMappingConfigData();
  void resetColorMappingConfigData();

  int getNumberOfDeviceMappingEntries() const;
  int getNumberOfColorMappingEntries();
  
  bool addDeviceMappingEntry(int entryNum, String name, deviceType type, int ledNumber);
  bool deleteColorMappingEntry(int entryNum);
  bool deleteAllDeviceMappingEntries();
  bool isDeviceMappingDirty() const;
  bool isDeviceMappingFull() const;
  
  bool addColorMappingEntry(int entryNum, String msg, deviceType type, Color color, Behavior behavior);
  bool deleteDeviceMappingEntry(int entryNum);
  bool deleteAllColorMappingEntries();
  bool isColorMappingDirty() const;
  bool isColorMappingFull() const;
  
  const DeviceMapping* getDeviceMapping(int index) const;
  const ColorMapping* getColorMapping(int index); 
    
  int getLedNumber(String device, deviceType type);
  int getColorMapIndex(deviceType deviceType, String msg);
  Behavior getLedBehavior(int colorMapIndex);
  Color getLedColor(int colorMapIndex);

  static uint32_t color2id(Color color)
  {
    for(uint32_t index = 0; index < 8; index++)
    {
      if(colorTranslator[index].color == color)
      {
        return colorTranslator[index].id;
      }
    }
    return 0;
  }

  static Color id2color(uint32_t id)
  {
    for(uint32_t index = 0; index < 8; index++)
    {
      if(colorTranslator[index].id == id)
      {
        return colorTranslator[index].color;
      }
    }
    return NONE;
  }

private:

  static const constexpr ColorTranslator colorTranslator[8] =
  {  
    {NONE,   0}, 
    {GREEN,  1},
    {YELLOW, 2},
    {ORANGE, 3},
    {RED,    4},
    {PURPLE, 5},
    {BLUE,   6},
    {WHITE,  7},
  };

  bool readMainConfigFile();
  void printMainConfigFile(JsonObject& json);
  void writeMainConfigFile();

  bool readColorMappingConfigFile();
  void writeColorMappingConfigFile();

  bool readDeviceMappingConfigFile();
  void writeDeviceMappingConfigFile();

  void onFileWriteError();

  static const int MAX_VERSION_LEN           = 20;
  static const int MAX_HOST_LEN              = 30;
  static const int MAX_WIFI_SSID_LEN         = 30;
  static const int MAX_WIFI_HOSTNAME_LEN     = 30;
  static const int MAX_WIFI_PSK_LEN          = 30;
  static const int MAX_MQTT_SERVER_LEN       = 20;
  static const int MAX_MQTT_SERVER_USER_LEN  = 20;
  static const int MAX_MQTT_SERVER_PASS_LEN  = 20;
  static const int MAX_MQTT_STATUS_TOPIC_LEN = 50;
  static const int MAX_MQTT_TEST_TOPIC_LEN   = 50;
  static const int MAX_MQTT_WILL_TOPIC_LEN   = 50;

  static const int MAX_COLOR_MAP_ENTRIES  = 30;
  static const int MAX_DEVICE_MAP_ENTRIES = 35;

  PreAllocatedLinkedList<ColorMapping> m_cfgColorMapping;
  bool m_cfgColorMappingDirty;

  PreAllocatedLinkedList<DeviceMapping> m_cfgDeviceMapping;
  bool m_cfgDeviceMappingDirty;
  
  char m_cfgVersion[MAX_VERSION_LEN + 1];
  char m_cfgHost[MAX_HOST_LEN + 1];
  char m_cfgWifiSSID[MAX_WIFI_SSID_LEN + 1];
  char m_cfgWifiHostname[MAX_WIFI_HOSTNAME_LEN + 1];
  char m_cfgWifiPSK[MAX_WIFI_PSK_LEN + 1];
  char m_cfgMqttServer[MAX_MQTT_SERVER_LEN + 1];
  char m_cfgMqttServerUser[MAX_MQTT_SERVER_USER_LEN + 1];
  char m_cfgMqttServerPass[MAX_MQTT_SERVER_PASS_LEN + 1];
  char m_cfgMqttStatusTopic[MAX_MQTT_STATUS_TOPIC_LEN + 1];
  char m_cfgMqttTestTopic[MAX_MQTT_TEST_TOPIC_LEN + 1];
  char m_cfgMqttWillTopic[MAX_MQTT_WILL_TOPIC_LEN + 1];
  
  int m_cfgNumberOfLeds;
  uint8_t m_cfgLedBrightness;

  HSDConfigFile m_mainConfigFile;
  HSDConfigFile m_colorMappingConfigFile;
  HSDConfigFile m_deviceMappingConfigFile;
};
