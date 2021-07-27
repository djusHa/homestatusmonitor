#include "HSDWebserver.hpp"

HSDWebserver::HSDWebserver(HSDConfig& config, const HSDLeds& leds, const HSDMqtt& mqtt)
:
m_server(80),
m_config(config),
m_leds(leds),
m_mqtt(mqtt),
m_html(),
m_deviceUptimeMinutes(0)
{
  m_updateServer.setup(&m_server);
}

void HSDWebserver::begin()
{
  Serial.println(F(""));
  Serial.println(F("Starting WebServer."));

  m_server.begin();

  m_server.on("/", std::bind(&HSDWebserver::deliverStatusPage, this));
  m_server.on("/cfgmain", std::bind(&HSDWebserver::deliverRootPage, this));
  m_server.on("/cfgcolormapping", std::bind(&HSDWebserver::deliverColorMappingPage, this));
  m_server.on("/cfgdevicemapping", std::bind(&HSDWebserver::deliverDeviceMappingPage, this));
  m_server.onNotFound(std::bind(&HSDWebserver::deliverNotFoundPage, this));
}

void HSDWebserver::handleClient(unsigned long deviceUptime)
{
  m_deviceUptimeMinutes = deviceUptime;
  m_server.handleClient();
}

void HSDWebserver::deliverRootPage()
{
  bool needSave = updateMainConfig();
  
  String html;
  html.reserve(3000);
  
  html = m_html.getHeader("Settings", m_config.getHost());

  html += F("<form><font face='Verdana,Arial,Helvetica'>");
  
  html += F(
  "<table border='0' cellpadding='0' cellspacing='2'>"
  " <tr>"
  "  <td><font size='+1' class='heading-underline'>Common settings:</font></td>"
  "  <td></td>"
  " </tr>"
  " <tr>"
  "  <td>Name</td>");
  
  html += F("  <td><input type='text' id='host' name='host' value='");
  html += String(m_config.getHost());
  html += F("' size='30' maxlength='40' placeholder='host'></td></tr>");
  // hostname
  html += F("</tr><tr><td>Hostname</td>");
  html += F("<td><input type='text' id='wifiHostname' name='wifiHostname' value='");
  html += String(m_config.getWifiHostname());
  html += F("' size='30' maxlength='40' placeholder='Hostname'></td>");
  // hostname end

  html += F(
  " <tr>"
  "  <td><font size='+1' class='heading-underline'>WiFi</font></td>"
  "  <td></td>"
  " </tr>"
  " <tr>"
  "  <td>SSID</td>");
  
  html += F("<td><input type='text' id='wifiSSID' name='wifiSSID' value='");
  html += String(m_config.getWifiSSID());
  html += F("' size='30' maxlength='40' placeholder='SSID'></td>");
  html += F("</tr><tr><td>Password</td>");
  html += F("  <td><input type='password' id='wifiPSK' name='wifiPSK' value='");
  html += String(m_config.getWifiPSK());
  html += F("' size='30' maxlength='40' placeholder='Password'></td></tr>");

  html += F(
  " <tr>"
  "  <td><font size='+1' class='heading-underline'>MQTT</font></td>"
  "  <td></td>"
  " </tr>"
  " <tr>"
  "  <td>Server</td>");
  html += F("  <td><input type='text' id='mqttServer' name='mqttServer' value='");
  html += String(m_config.getMqttServer());
  html += F("' size='30' maxlength='40' placeholder='IP or hostname'></td></tr><tr><td>Username</td>");

  // username + password
  html += F("  <td><input type='text' id='mqttServerUser' name='mqttServerUser' value='");
  html += String(m_config.getMqttServerUser());
  html += F("' size='30' maxlength='40' placeholder='MQTT Username'></td></tr><tr><td>Password</td>");
  
  html += F("  <td><input type='password' id='mqttServerPass' name='mqttServerPass' value='");
  html += String(m_config.getMqttServerPass());
  html += F("' size='30' maxlength='40' placeholder='MQTT PASS'></td></tr><tr><td>Status topic</td>");
  
  html += F("  <td><input type='text' id='mqttStatusTopic' name='mqttStatusTopic' value='");
  html += String(m_config.getMqttStatusTopic());
  html += F("' size='30' maxlength='40' placeholder='#'></td>"
  " </tr>"
  " <tr>"
  "  <td>Test topic</td>"
  "  <td><input type='text' id='mqttTestTopic' name='mqttTestTopic' value='");
  html += String(m_config.getMqttTestTopic());
  html += F("' size='30' maxlength='40' placeholder='#'></td>"
  " </tr>"
  " <tr>"
  "  <td>Will topic</td>"
  "  <td><input type='text' id='mqttWillTopic' name='mqttWillTopic' value='");
  html += String(m_config.getMqttWillTopic());
  html += F("' size='30' maxlength='40' placeholder='#'></td></tr>");

  html += F(""
  " <tr>"
  "  <td><font size='+1' class='heading-underline'>LEDs</font></td>"
  "  <td></td>"
  " </tr>"
  " <tr>"
  "  <td>Number of LEDs</td>");
  html += "  <td><input type='text' id='ledCount' name='ledCount' value='" + String(m_config.getNumberOfLeds()) + "' size='30' maxlength='40' placeholder='0'></td></tr>";
  html += F("<tr><td>Brightness</td>");
  html += F("<td><input type='text' id='ledBrightness' name='ledBrightness' value='");
  html += String(m_config.getLedBrightness());
  html += F("' size='30' maxlength='5' placeholder='0-255'></td></tr></table>"); 
  
  html += F("<input type='submit' class='button' value='Save'>");

  html += F("</form></font></body></html>");
  
  html += F("<input type='button' class='button' onclick=\"location.href='./'\"  value='Back'>");

  Serial.print(F("Page size: "));
  Serial.println(html.length());
  
  m_server.send(200, F("text/html"), html);

  if(needSave)
  {
    Serial.println(F("Main config has changed, storing it."));
    m_config.saveMain();
  }

  checkReboot();

  Serial.print(F("Free RAM: ")); Serial.println(ESP.getFreeHeap());
}

void HSDWebserver::deliverStatusPage()
{
  String html;
  html.reserve(3000);
  
  html = m_html.getHeader("Status", m_config.getHost());

  html += F("<div class='heading'>Device uptime: </div><div>");
  html += m_html.minutes2Uptime(m_deviceUptimeMinutes);
  html += F("</div>");

  if (WiFi.status() == WL_CONNECTED)
  {
    html += F("<div class='heading'>Connected to WLAN: </div><div>");
    html += WiFi.SSID();
    html += F("</div><div class='heading'>Hostname is: </div><div>");
    html += WiFi.hostname();
    html += F("</div><div class='heading'>IP address is </div><div>");
    html += m_html.ip2String(WiFi.localIP());
    html += F("</div>");
  }
  else
  {
    html += F("<div class='heading'>Device is not connected to local network</div>");
  }

  if(m_mqtt.connected())
  {
    html += F("<div class='heading'>Device is connected to  MQTT Broker: </div><div>");
    html += m_config.getMqttServer();
    html += F("</div>");
  }
  else
  {
    html += F("<div div class='heading'>Device is not connected to an MQTT broker</div>");
  }

  if(m_config.getNumberOfLeds() == 0)
  {
    html += F("<div div class='heading'>No LEDs configured yet</div>");
  }
  else
  {
    int ledOnCount = 0;
    
    html += F("<div class='flex-row'>");
    
    for(int ledNr = 0; ledNr < m_config.getNumberOfLeds(); ledNr++)
    {
      HSDConfig::Color color = m_leds.getColor(ledNr);
      HSDConfig::Behavior behavior = m_leds.getBehavior(ledNr);

      if( (HSDConfig::NONE != color) && (HSDConfig::OFF != behavior) )
      {
        html += F("<div class='led-item'><div class='hsdcolor' style='background-color:");
        html += m_html.color2htmlColor(color);
        html += F(";'></div>"); 
        html += F("<div>LED &nbsp;<b>");
        html += ledNr;
        html += F("</b>&nbsp;is&nbsp;<b>");
        html += m_html.behavior2String(behavior);
        html += F("</b>&nbsp;with&nbsp;color&nbsp;<b>");
        html += m_html.color2String(color);      
        html += F("</b></div></div>");

        ledOnCount++;
      }
    }

    if(ledOnCount == 0)
    {
      html += F("<p>All LEDs are&nbsp;<b>off</b><p>");
    }

    html += F("</div>");
  }

  html += m_html.getNav();
  html += m_html.getFooter(m_config.getVersion());

  Serial.print(F("Page size: "));
  Serial.println(html.length());
  
  m_server.send(200, F("text/html"), html);

  checkReboot();
}

void HSDWebserver::deliverColorMappingPage()
{
  if(needUndo())
  {
    Serial.println(F("Need to undo changes to color mapping config"));
    m_config.updateColorMapping();
  }
  else if(needAdd())
  {
    Serial.println(F("Need to add color mapping config entry"));
    addColorMappingEntry();
  }
  else if(needDelete())
  {
    Serial.println(F("Need to delete color mapping config entry"));
    deleteColorMappingEntry();
  }
  else if(needDeleteAll())
  {
    Serial.println(F("Need to delete all color mapping config entries"));
    m_config.deleteAllColorMappingEntries();
  }
  else if(needSave())
  {
    Serial.println(F("Need to save color mapping config"));
    m_config.saveColorMapping();
  }
    
  String html;
  html.reserve(8000);
  
  html = m_html.getHeader("Color mapping", m_config.getHost());

  html += m_html.getColorMappingTableHeader();

  for(uint32_t i = 0; i < m_config.getNumberOfColorMappingEntries(); i++)
  {
    const HSDConfig::ColorMapping* mapping = m_config.getColorMapping(i);
    html += m_html.getColorMappingTableEntry(i, mapping);
  }

  html += m_html.getColorMappingTableFooter();

  if(m_config.isColorMappingFull())
  {
    html += F("<p>Edit entry (add not possible, entry limit reached):</p>");
    html += m_html.getColorMappingTableAddEntryForm(m_config.getNumberOfColorMappingEntries(), true);
  }
  else
  {
    html += F("<p>Add/edit entry:</p>");    
    html += m_html.getColorMappingTableAddEntryForm(m_config.getNumberOfColorMappingEntries(), false);
  }

  html += F("<p>Delete Entry:</p>");
  html += m_html.getDeleteForm();

  if(m_config.isColorMappingDirty())
  {
    html += F("<p style='color:red'>Unsaved changes! Press Save to make them permanent, <br/>or Undo to revert to last saved version!</p>");
    html += m_html.getSaveForm();
  }

  html += F("<input type='button' class='button' onclick=\"location.href='./'\"  value='Back'>");

  html += m_html.getFooter(m_config.getVersion());

  Serial.print(F("Page size: "));
  Serial.println(html.length());
  
  m_server.send(200, F("text/html"), html);

  checkReboot();

  Serial.print(F("Free RAM: ")); Serial.println(ESP.getFreeHeap());
}

bool HSDWebserver::needAdd()
{
   return (m_server.hasArg("add"));
}

bool HSDWebserver::needDelete()
{
   return (m_server.hasArg("delete"));
}

bool HSDWebserver::needDeleteAll()
{
  return (m_server.hasArg("deleteall"));
}

bool HSDWebserver::needSave()
{
   return (m_server.hasArg("save"));
}

bool HSDWebserver::needUndo()
{
   return (m_server.hasArg("undo"));
}

bool HSDWebserver::addColorMappingEntry()
{
  bool success = false;
  
  if(m_server.hasArg("i") && m_server.hasArg("n") && m_server.hasArg("t") && m_server.hasArg("c") && m_server.hasArg("b"))
  {
    if(m_server.arg("n") != "")
    {
      success = m_config.addColorMappingEntry(m_server.arg("i").toInt(),
                                              m_server.arg("n"), 
                                              (HSDConfig::deviceType)(m_server.arg("t").toInt()), 
                                              (HSDConfig::Color)(HSDConfig::id2color(m_server.arg("c").toInt())), 
                                              (HSDConfig::Behavior)(m_server.arg("b").toInt()));
    }
    else
    {
      Serial.print(F("Skipping empty entry"));
    }
  }

  return success;
}

bool HSDWebserver::deleteColorMappingEntry()
{
  bool success = false;
  int entryNum = 0;
  
  if(m_server.hasArg("i"))
  {
    entryNum = m_server.arg("i").toInt();
// TODO check conversion status
    success = m_config.deleteColorMappingEntry(entryNum);                                  
  }

  return success;
}

void HSDWebserver::deliverDeviceMappingPage()
{
  if(needUndo())
  {
    Serial.println(F("Need to undo changes to device mapping config"));
    m_config.updateDeviceMapping();
  }
  else if(needAdd())
  {
    Serial.println(F("Need to add device mapping config entry"));
    addDeviceMappingEntry();
  }
  else if(needDelete())
  {
    Serial.println(F("Need to delete device mapping config entry"));
    deleteDeviceMappingEntry();
  }
  else if(needDeleteAll())
  {
    Serial.println(F("Need to delete all device mapping config entries"));
    m_config.deleteAllDeviceMappingEntries();
  }
  else if(needSave())
  {
    Serial.println(F("Need to save device mapping config"));
    m_config.saveDeviceMapping();
  }

  String html;
  html.reserve(8000);
    
  html = m_html.getHeader("Device mapping", m_config.getHost());

  html += m_html.getDeviceMappingTableHeader();
  
  for(uint32_t i = 0; i < m_config.getNumberOfDeviceMappingEntries(); i++)
  {
    const HSDConfig::DeviceMapping* mapping = m_config.getDeviceMapping(i);
    html += m_html.getDeviceMappingTableEntry(i, mapping);
  }

  html += m_html.getDeviceMappingTableFooter();

  if(m_config.isDeviceMappingFull())
  {
    html += F("<p>Edit entry (add not possible, entry limit reached):</p>");
    html += m_html.getDeviceMappingTableAddEntryForm(m_config.getNumberOfDeviceMappingEntries(), true);
  }
  else
  {
    html += F("<p>Add/edit entry:</p>");    
    html += m_html.getDeviceMappingTableAddEntryForm(m_config.getNumberOfDeviceMappingEntries(), false);
  }

  html += F("<br/>Delete Entry:<br/>");
  html += m_html.getDeleteForm();

  if(m_config.isDeviceMappingDirty())
  {
    html += F("<p style='color:red'>Unsaved changes! Press ""Save"" to make them permanent, or they will be lost on next reboot!</p>");
    html += m_html.getSaveForm();
  }

  html += F("<input type='button' class='button' onclick=\"location.href='./'\"  value='Back'>");

  html += m_html.getFooter(m_config.getVersion());

  Serial.print(F("Page size: "));
  Serial.println(html.length());
  
  m_server.send(200, F("text/html"), html);
  
  checkReboot();

  Serial.print(F("Free RAM: ")); Serial.println(ESP.getFreeHeap());
}

bool HSDWebserver::addDeviceMappingEntry()
{
  bool success = false;

  if(m_server.hasArg("i") && m_server.hasArg("n") && m_server.hasArg("t") && m_server.hasArg("l"))
  {
    if(m_server.arg("n") != "")
    {
      success = m_config.addDeviceMappingEntry(m_server.arg("i").toInt(),
                                               m_server.arg("n"), 
                                               (HSDConfig::deviceType)(m_server.arg("t").toInt()), 
                                               m_server.arg("l").toInt());                                   
    }
    else
    {
      Serial.print(F("Skipping empty entry"));
    }
  }
  
  return success;
}

bool HSDWebserver::deleteDeviceMappingEntry()
{
  bool success = false;
  int entryNum = 0;
  
  if(m_server.hasArg("i"))
  {
    entryNum = m_server.arg("i").toInt();
// TODO check conversion status
    success = m_config.deleteDeviceMappingEntry(entryNum);                                    
  }

  return success;
}

void HSDWebserver::deliverNotFoundPage()
{
  String html = F("File Not Found\n\n");
  html += F("URI: ");
  html += m_server.uri();
  html += F("\nMethod: ");
  html += (m_server.method() == HTTP_GET) ? F("GET") : F("POST");
  html += F("\nArguments: ");
  html += m_server.args();
  html += F("\n");
  
  for (uint8_t i = 0; i < m_server.args(); i++)
  {
    html += " " + m_server.argName(i) + ": " + m_server.arg(i) + "\n";
  }
  
  m_server.send(404, F("text/plain"), html);
}

void HSDWebserver::checkReboot()
{
  if(m_server.hasArg(F("reset"))) 
  {
    Serial.println(F("Rebooting ESP."));
    ESP.restart();
  }
}

bool HSDWebserver::updateMainConfig()
{
  bool needSave = false;

  if (m_server.hasArg(JSON_KEY_HOST))
  {
    needSave |= m_config.setHost(m_server.arg(JSON_KEY_HOST).c_str());
  }
  
  if (m_server.hasArg(JSON_KEY_WIFI_SSID))
  {
    needSave |= m_config.setWifiSSID(m_server.arg(JSON_KEY_WIFI_SSID).c_str());
  }
  
  if (m_server.hasArg(JSON_KEY_WIFI_HOSTNAME))
  {
    needSave |= m_config.setWifiHostname(m_server.arg(JSON_KEY_WIFI_HOSTNAME).c_str());
  }
  
  if (m_server.hasArg(JSON_KEY_WIFI_PSK)) 
  {
    needSave |= m_config.setWifiPSK(m_server.arg(JSON_KEY_WIFI_PSK).c_str());
  }

  if (m_server.hasArg(JSON_KEY_MQTT_SERVER))
  {
    needSave |= m_config.setMqttServer(m_server.arg(JSON_KEY_MQTT_SERVER).c_str());
  }

  if (m_server.hasArg(JSON_KEY_MQTT_USER))
  {
    needSave |= m_config.setMqttServerUser(m_server.arg(JSON_KEY_MQTT_USER).c_str());
  }

  if (m_server.hasArg(JSON_KEY_MQTT_PASS))
  {
    needSave |= m_config.setMqttServerPass(m_server.arg(JSON_KEY_MQTT_PASS).c_str());
  }
  
  if (m_server.hasArg(JSON_KEY_MQTT_STATUS_TOPIC))
  {
    needSave |= m_config.setMqttStatusTopic(m_server.arg(JSON_KEY_MQTT_STATUS_TOPIC).c_str());
  }
  
  if (m_server.hasArg(JSON_KEY_MQTT_TEST_TOPIC)) 
  {
    needSave |= m_config.setMqttTestTopic(m_server.arg(JSON_KEY_MQTT_TEST_TOPIC).c_str());
  }

  if (m_server.hasArg(JSON_KEY_MQTT_WILL_TOPIC)) 
  {
    needSave |= m_config.setMqttWillTopic(m_server.arg(JSON_KEY_MQTT_WILL_TOPIC).c_str());
  }

  if (m_server.hasArg(JSON_KEY_LED_COUNT))
  {
    int ledCount = m_server.arg(JSON_KEY_LED_COUNT).toInt();
    
    if(ledCount > 0)
    {
      needSave |= m_config.setNumberOfLeds(ledCount);
    }
  }

  if (m_server.hasArg(JSON_KEY_LED_BRIGHTNESS)) 
  {
    uint8_t ledBrightness = m_server.arg(JSON_KEY_LED_BRIGHTNESS).toInt();
    
    if(ledBrightness > 0)
    {
      needSave |= m_config.setLedBrightness(ledBrightness);
    }
  }

  return needSave;
}
