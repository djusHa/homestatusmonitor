#include "HSDHtmlHelper.hpp"

#define SELECTED_STRING (F("selected='selected'"))
#define CHECKED_STRING  (F("checked='checked'")) 
#define EMPTY_STRING    (F(""))

HSDHtmlHelper::HSDHtmlHelper()
{
  
}

String HSDHtmlHelper::getHeader(const char* title, const char* host) const
{
  String header;
  header.reserve(1500);

  header  = F("<!DOCTYPE html>");
  header += F("<html lang='en'>");
  header += F("<head><meta charset='utf-8'>");
  header += F("<meta http-equiv='x-ua-compatible' content='ie=edge' />");
  header += F("<meta name='viewport' content='width=device-width, initial-scale=1' />");
  header += F("<title>");
  header += String(host);
  header += F("</title>");
  header += F("<style>");
  header += F(".hsdcolor {width:15px;height:15px;border: 2px #6C7A89 solid;border-radius: 50%;box-shadow: 3px 3px 3px 0px rgba(0,0,0,0.75);float:left;margin-right:5px;}");
  header += F(".rdark {background-color:#f9f9f9;}");
  header += F(".rlight {background-color:#e5e5e5;}");
  header += F(".container {width: 340px;margin: auto;display: flex;flex-direction: column;justify-content: center;}");
  header += F(".container * {flex-direction: column;display: flex;}");
  header += F(".flex-row {margin-top: 10px;}");
  header += F(".flex-row * {flex-direction: row;}");
  header += F(".button {border: none;cursor: pointer;padding: 15px 70px;margin: 10px 0px;text-transform: uppercase;letter-spacing: 1px;font-weight: 600;background: #3498db;color: #fff;}");
  header += F("input[type=\"text\"], input[type=\"password\"] {padding: 10px 5px;line-height: 28px;}");
  header += F("select {padding: 10px 5px;line-height: 28px;}");
  header += F(".danger {background-color:#CF000F}");
  header += F(".success {background-color:#26A65B}");
  header += F(".warning {background-color:#E08A1E}");
  header += F("hr {margin-left: 0; margin-right: 0;border: 0;height: 1px;background-image: linear-gradient(to right, rgba(108,122,137, 0), rgba(108,122,137, 0.75), rgba(108,122,137, 0));}");
  header += F(".led-item{margin: 5px 0;}");
  header += F(".breadcrumb {display: block;list-style: none;overflow: hidden;margin: 10px 0;padding: 0;}");
  header += F(".breadcrumb li {float: left;}");
  header += F(".breadcrumb li:first-child a {padding-left: 5px;}");
  header += F(".breadcrumb li a{color: white;text-decoration: none; padding: 5px 0 5px 45px;background: #3498db;position: relative;display: block;float: left;width: 100%; text-transform: uppercase; text-shadow: 2px 2px 2px rgba(0, 0, 0, 0.5);}");
  header += F(".breadcrumb li a::after {content: ' ';display: block;width: 0;height: 0;border-top: 50px solid transparent;border-bottom: 50px solid transparent;border-left: 30px solid #3498db;position: absolute;top: 50%;margin-top: -50px;left: 100%;z-index: 2;}");
  header += F(".breadcrumb li a::before {content: ' ';display: block;width: 0;height: 0;border-top: 50px solid transparent;border-bottom: 50px solid transparent;border-left: 30px solid white;position: absolute;top: 50%;margin-top: -50px;margin-left: 1px;left: 100%;z-index: 1;}");
  header += F(".heading{font-weight: 600;padding: 10px 0;}");
  header += F(".heading-underline{text-decoration: underline;font-weight: 600;padding: 10px 0;}");
  header += F(".table {width: 100%;}");
  header += F(".table tr {flex-direction: row;flex-wrap: nowrap;}");
  header += F(".table th, .table td {flex: 1 1 340px;padding: 5px 2px;border-bottom: 1px solid #ccc;align-items: baseline}");
  header += F("</style>");
  header += F("</head>");
  header += F("<body bgcolor='#F2F1EF'>");
  header += F("<div class='container'>");
  header += F("<font size='+3'>");
  header += String(host);
  header += F("</font><hr/>");
  
  header += F("<ul class='breadcrumb'>");

  if(title == "Status") {
    header += F("<li><a href='/'>");
    header += title;
    header += F("</a></li></ul>");
  } else {
    header += F("<li><a href='/'>Home</a></li><li><a href='#'>");
    header += title;
    header += F("</a></li>");
  }

  header += F("</ul>");

  Serial.print(F("Header size: "));
  Serial.println(header.length());
  
  return header;  
}

String HSDHtmlHelper::getNav() const {
  return F("<form><input type='button' class='button' onclick=\"location.href='./'\"  value='Status'>"
  "<input type='button' class='button' onclick=\"location.href='./cfgmain'\"  value='Settings'>"
  "<input type='button' class='button' onclick=\"location.href='./cfgdevicemapping'\"  value='Device mapping'>"
  "<input type='button' class='button' onclick=\"location.href='./cfgcolormapping'\"  value='Color mapping'>"
  "<input type='button' class='button' onclick=\"location.href='./update'\"  value='Update Firmware'>"  
  "<input type='submit' class='button danger' value='Reboot' id='reset' name='reset'></form>");
}

String HSDHtmlHelper::getFooter(const char* version) const
{
  String html;

  html = F("<hr><div style='text-align:center;font-weight: 500'>v");
  html += String(version);
  html += F("</div></div></body></html>");

  return html;
}

String HSDHtmlHelper::getColorMappingTableHeader() const
{
  return F(""
  "<table class='table'>"
  " <tr style='background-color:#828282'>"
  "  <th>Nr</th>"
  "  <th>Message</th>"
  "  <th>Type</th>"
  "  <th>Color</th>"
  "  <th>Behavior</th>"
  " </tr>");
}

String HSDHtmlHelper::getColorMappingTableEntry(int entryNum, const HSDConfig::ColorMapping* mapping) const
{
  String html;
  if(entryNum % 2 == 0)
  {
    html += F("<tr class='rlight'><td>");
  }
  else
  {
    html += F("<tr class='rdark'><td>");
  }
  html += entryNum;
  html += F("</td><td>");
  html += mapping->msg;
  html += F("</td><td>");
  html += type2String(mapping->type);
  html += F("</td><td>");
  html += F("<div class='hsdcolor' style='background-color:");
  html += color2htmlColor(mapping->color);
  html += F(";'></div></td><td>"); 
  html += behavior2String(mapping->behavior);
  html += F("</td></tr>");
  
  return html;
}

String HSDHtmlHelper::getColorMappingTableFooter() const
{
  return F("</table>");
}

String HSDHtmlHelper::getColorMappingTableAddEntryForm(int newEntryNum, bool isFull) const
{
  String html;
  
  html += F("<form><table><tr>");
  html += F("<td><input type='text' id='number' name='i' value='");
  html += isFull ? newEntryNum - 1 : newEntryNum;
  html += F("' size='5' maxlength='3' placeholder='Nr'></td>");
  html += F("<td><input type='text' id='name' name='n' value='' size='20' maxlength='15' placeholder='name'></td>");
  html += F("<td><select name='t'>");
  html += getTypeOptions(HSDConfig::TYPE_WINDOW);
  html += F("</select></td>");
  html += F("<td><select name='c'>");
  html += getColorOptions(HSDConfig::WHITE);
  html += F("</select></td>");
  html += F("<td><select name='b'>");
  html += getBehaviorOptions(HSDConfig::ON);
  html += F("</select></td></tr></table>");  
  html += F("<input type='submit' class='button' value='");
  html += isFull ? F("Edit") : F("Add/Edit");
  html += F("' id='add' name='add'></form>");

  return html;
}

String HSDHtmlHelper::getDeviceMappingTableHeader() const
{
  return F(""
  "<table class='table'>"
  " <tr style='background-color:#828282'>"
  "  <th><b><font size='+1'>Nr</font></b></th>"  
  "  <th><b><font size='+1'>Device</font></b></th>"
  "  <th><b><font size='+1'>Type</font></b></th>"
  "  <th><b><font size='+1'>Led</font></b></th>"
  " </tr>");
}

String HSDHtmlHelper::getDeviceMappingTableEntry(int entryNum, const HSDConfig::DeviceMapping* mapping) const
{
  String html;
    
  if(entryNum % 2 == 0)
  {
    html += F("<tr class='rlight'><td>");
  }
  else
  {
    html += F("<tr class='rdark'><td>");
  }
  html += entryNum;
  html += F("</td><td>");
  html += mapping->name;
  html += F("</td><td>");
  html += type2String(mapping->type);
  html += F("</td><td>");
  html += mapping->ledNumber;
  html += F("</td></tr>");

  return html;
}

String HSDHtmlHelper::getDeviceMappingTableFooter() const
{
  return F("</table>");
}

String HSDHtmlHelper::getDeviceMappingTableAddEntryForm(int newEntryNum, bool isFull) const
{
  String html;
  
  html += F("<form><table><tr>");
  html += F("<td><input type='text' id='number' name='i' value='");
  html += isFull ? newEntryNum - 1 : newEntryNum;
  html += F("' size='5' maxlength='3' placeholder='Nr'></td>");
  html += F("<td><input type='text' id='name' name='n' value='' size='30' maxlength='25' placeholder='name'></td>");
  html += F("<td><select name='t'>");
  html += getTypeOptions(HSDConfig::TYPE_WINDOW);
  html += F("</select></td>");
  html += F("<td><input type='text' id='led' name='l' value='");
  html += isFull ? newEntryNum - 1 : newEntryNum;
  html += F("' size='6' maxlength='3' placeholder='led nr'></td></tr></table>");
  html += F("<input type='submit' class='button' value='");
  html += isFull ? F("Edit") : F("Add/Edit");
  html += F("' id='add' name='add'></form>");

  return html;
}

String HSDHtmlHelper::getDeleteForm() const
{
  String html;
  
  html += F("<form><input type='text' id='number' name='i' value='' size='5' maxlength='3' placeholder='Nr' autocomplete='off'><br/>");
  html += F("<input type='submit' class='button danger' value='Delete' id='delete' name='delete'>");
  html += F("<input type='submit' class='button danger' value='Delete all' id='deleteall' name='deleteall'></form>");
  
  return html;  
}

String HSDHtmlHelper::getSaveForm() const
{
  String html;
  
  html += F("<form><input type='submit' class='button success' value='Save' id='save' name='save'>");   
  html += F("<form><input type='submit' class='button warning' value='Undo' id='undo' name='undo'></form>");   

  return html;
}

String HSDHtmlHelper::getColorOptions(HSDConfig::Color selectedColor) const
{
  String greenSelect  = (selectedColor == HSDConfig::GREEN)  ? SELECTED_STRING : EMPTY_STRING;
  String yellowSelect = (selectedColor == HSDConfig::YELLOW) ? SELECTED_STRING : EMPTY_STRING;
  String orangeSelect = (selectedColor == HSDConfig::ORANGE) ? SELECTED_STRING : EMPTY_STRING;
  String redSelect    = (selectedColor == HSDConfig::RED)    ? SELECTED_STRING : EMPTY_STRING;
  String purpleSelect = (selectedColor == HSDConfig::PURPLE) ? SELECTED_STRING : EMPTY_STRING;
  String blueSelect   = (selectedColor == HSDConfig::BLUE)   ? SELECTED_STRING : EMPTY_STRING;
  String whiteSelect  = (selectedColor == HSDConfig::WHITE)  ? SELECTED_STRING : EMPTY_STRING;

  String html;

  html += F("<option "); html += greenSelect;  html += F(" value='"); html += HSDConfig::color2id(HSDConfig::GREEN);  html += F("'>Green</option>");
  html += F("<option "); html += yellowSelect; html += F(" value='"); html += HSDConfig::color2id(HSDConfig::YELLOW); html += F("'>Yellow</option>");
  html += F("<option "); html += orangeSelect; html += F(" value='"); html += HSDConfig::color2id(HSDConfig::ORANGE); html += F("'>Orange</option>");
  html += F("<option "); html += redSelect;    html += F(" value='"); html += HSDConfig::color2id(HSDConfig::RED);    html += F("'>Red</option>");
  html += F("<option "); html += purpleSelect; html += F(" value='"); html += HSDConfig::color2id(HSDConfig::PURPLE); html += F("'>Purple</option>");
  html += F("<option "); html += blueSelect;   html += F(" value='"); html += HSDConfig::color2id(HSDConfig::BLUE);   html += F("'>Blue</option>");
  html += F("<option "); html += whiteSelect;  html += F(" value='"); html += HSDConfig::color2id(HSDConfig::WHITE);  html += F("'>White</option>");

  return html;
}

String HSDHtmlHelper::getBehaviorOptions(HSDConfig::Behavior selectedBehavior) const
{
  String onSelect       =   (selectedBehavior == HSDConfig::ON)         ? SELECTED_STRING : EMPTY_STRING;
  String blinkingSelect =   (selectedBehavior == HSDConfig::BLINKING)   ? SELECTED_STRING : EMPTY_STRING;
  String flashingSelect =   (selectedBehavior == HSDConfig::FLASHING)   ? SELECTED_STRING : EMPTY_STRING;
  String flickeringSelect = (selectedBehavior == HSDConfig::FLICKERING) ? SELECTED_STRING : EMPTY_STRING;
  
  String html;

  html += F("<option "); html += onSelect;         html += F(" value='"); html += HSDConfig::ON;         html += F("'>On</option>");
  html += F("<option "); html += blinkingSelect;   html += F(" value='"); html += HSDConfig::BLINKING;   html += F("'>Blink</option>");
  html += F("<option "); html += flashingSelect;   html += F(" value='"); html += HSDConfig::FLASHING;   html += F("'>Flash</option>");
  html += F("<option "); html += flickeringSelect; html += F(" value='"); html += HSDConfig::FLICKERING; html += F("'>Flicker</option>");
  
  return html;
}

String HSDHtmlHelper::getTypeOptions(HSDConfig::deviceType selectedType) const
{
  String windowSelect = (selectedType == HSDConfig::TYPE_WINDOW) ? SELECTED_STRING : EMPTY_STRING;
  String doorSelect   = (selectedType == HSDConfig::TYPE_DOOR)   ? SELECTED_STRING : EMPTY_STRING;
  String lightSelect  = (selectedType == HSDConfig::TYPE_LIGHT)  ? SELECTED_STRING : EMPTY_STRING;
  String alarmSelect  = (selectedType == HSDConfig::TYPE_ALARM)  ? SELECTED_STRING : EMPTY_STRING;

  String html;
  
  html += F("<option "); html += windowSelect; html += F("value='"); html += HSDConfig::TYPE_WINDOW; html += F("'>Window</option>");
  html += F("<option "); html += doorSelect;   html += F("value='"); html += HSDConfig::TYPE_DOOR;   html += F("'>Door</option>");
  html += F("<option "); html += lightSelect;  html += F("value='"); html += HSDConfig::TYPE_LIGHT;  html += F("'>Light</option>");
  html += F("<option "); html += alarmSelect;  html += F("value='"); html += HSDConfig::TYPE_ALARM;  html += F("'>Alarm</option>");
  
  return html;
}

String HSDHtmlHelper::ip2String(IPAddress ip) const
{
  char buffer[20];
  memset(buffer, 0, sizeof(buffer));

  sprintf(buffer, "%d.%d.%d.%d", ip[0],ip[1],ip[2],ip[3]);
  
  return String(buffer);
}

String HSDHtmlHelper::color2String(HSDConfig::Color color) const
{
  String colorString = F("none");

  switch(color)
  {
    case HSDConfig::GREEN:  colorString = F("green"); break;
    case HSDConfig::YELLOW: colorString = F("yellow"); break;
    case HSDConfig::ORANGE: colorString = F("orange"); break;
    case HSDConfig::RED:    colorString = F("red"); break;
    case HSDConfig::PURPLE: colorString = F("purple"); break;
    case HSDConfig::BLUE:   colorString = F("blue"); break;
    case HSDConfig::WHITE:  colorString = F("white"); break;
    default: break;
  }

  return colorString;
}

String HSDHtmlHelper::color2htmlColor(HSDConfig::Color color) const
{
  String htmlcolor = F("#000000");

  switch(color)
  {
    case HSDConfig::GREEN:  htmlcolor = F("#00FF00"); break;
    case HSDConfig::YELLOW: htmlcolor = F("#FFFF00"); break;
    case HSDConfig::ORANGE: htmlcolor = F("#FF5500"); break;
    case HSDConfig::RED:    htmlcolor = F("#FF0000"); break;
    case HSDConfig::PURPLE: htmlcolor = F("#FF00FF"); break;
    case HSDConfig::BLUE:   htmlcolor = F("#0000FF"); break;
    case HSDConfig::WHITE:  htmlcolor = F("#FFFFFF"); break;
    default: break;
  }

  return htmlcolor;
}

String HSDHtmlHelper::behavior2String(HSDConfig::Behavior behavior) const
{
  String behaviorString = F("Off");
  
  switch(behavior)
  {
    case HSDConfig::ON:         behaviorString = F("On"); break;
    case HSDConfig::BLINKING:   behaviorString = F("Blink"); break;
    case HSDConfig::FLASHING:   behaviorString = F("Flash"); break;
    case HSDConfig::FLICKERING: behaviorString = F("Flicker"); break;
    default: break;
  }

  return behaviorString;
}

String HSDHtmlHelper::type2String(HSDConfig::deviceType type) const
{
  String typeString = F("Window");

  switch(type)
  {
    case HSDConfig::TYPE_WINDOW: typeString = F("Window"); break;
    case HSDConfig::TYPE_DOOR:   typeString = F("Door"); break;
    case HSDConfig::TYPE_LIGHT:  typeString = F("Light"); break;
    case HSDConfig::TYPE_ALARM:  typeString = F("Alarm"); break;
    default: break;
  }

  return typeString; 
}


String HSDHtmlHelper::minutes2Uptime(unsigned long minutes) const
{
  char buffer[50];
  memset(buffer, 0, sizeof(buffer));
  
  unsigned long days  = minutes / 60 / 24;
  unsigned long hours = (minutes / 60) % 24;
  unsigned long mins  = minutes % 60;

  sprintf(buffer, "%lu days, %lu hours, %lu minutes", days, hours, mins);
  
  return String(buffer);
}

