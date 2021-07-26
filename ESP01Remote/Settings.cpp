#include "Settings.h";

Settings::Settings(){
  
}

void Settings::SaveSetting(ESP8266WebServer &server)
{
  eepromWrite(adrAPSSID, (char*)server.arg("APSSID").c_str());
  eepromWrite(adrAPPassword, (char*)server.arg("APPassword").c_str());
  eepromWrite(adrAPIpAddress, (char*)server.arg("APIpAddress").c_str());
  eepromWrite(adrClientSSID, (char*)server.arg("ClientSSID").c_str());
  eepromWrite(adrClientPassword, (char*)server.arg("password").c_str());
  eepromWrite(adrTShift, (char*)server.arg("APSSID").c_str());
}

void Settings::ReadSettings() {
  eepromRead(adrAPSSID, APSSID);
  eepromRead(adrAPPassword, APPassword);
  // eepromRead(adrAPIpAddress, APIpAddress);
  eepromRead(adrClientSSID, ClientSSID);
  eepromRead(adrClientPassword, ClientPassword);
  eepromRead(adrTShift, APSSID);
}

IPAddress Settings::IPfromString(String s) {
  IPAddress ip;
  ip[0] = s.substring(0, s.indexOf('.')).toInt();
  s.remove(0, s.indexOf('.') + 1);
  ip[1] = s.substring(0, s.indexOf('.')).toInt();
  s.remove(0, s.indexOf('.') + 1);
  ip[2] = s.substring(0, s.indexOf('.')).toInt();
  s.remove(0, s.indexOf('.') + 1);
  ip[3] = s.toInt();
  return ip;
}

void Settings::eepromWrite(int addr, char* value)
{
  // writing byte-by-byte to EEPROM
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(addr, value[i]);
    if (value[i] == '\0')break;
    addr += 1;
  }
  EEPROM.commit();
}

void Settings::eepromRead(int addr, char* value)
{
  // reading byte-by-byte from EEPROM
  for (int i = 0; i < EEPROM_SIZE; i++) {
    byte readValue = EEPROM.read(addr + i);
    if (readValue == '\0') {
      value[i] = char(readValue);
      break;
    }
    value[i] = char(readValue);
  }
}

String Settings::getJson() {
  this->ReadSettings();
  String ptr = "";
  ptr += "{\"APSSID\":\"" ;
  ptr += APSSID ;
  ptr += "\",\"APPassword\":\"" ;
  ptr += APPassword ;
  ptr += "\",\"APIpAddress\":\"" ;
  //  ptr += APIpAddress ;
  ptr += "\",\"SSID\":\"" ;
  ptr += ClientSSID ;
  ptr += "\",\"password\":\"" ;
  ptr += ClientPassword ;
  ptr += "\",\"TShift\":\"" ;
  ptr += TShift ;
  ptr += "\"}";
  return ptr;
}


/** Getters */
char* Settings::getAPSSID() {
  return APSSID;
}
char* Settings::getAPPassword() {
  return APPassword;
}
IPAddress Settings::getAPIpAddress() {

}
IPAddress Settings::getAPGateway() {

}
IPAddress Settings::getAPSubnet() {

}
char* Settings::getClientSSID() {
  return ClientSSID;
}
char* Settings::getClientPassword() {
  return ClientPassword;
}
char* Settings::getTShift() {
  return TShift;
}
char* Settings::getAPChannel() {
  return APChannel;
}
char* Settings::getAPHidden() {
  return APHidden;
}
char* Settings::getAPMaxConnection() {
  return APMaxConnection;
}

/** Setters */
void Settings::setAPSSID() {

}
void Settings::setAPPassword() {

}
void Settings::setAPIpAddress() {

}
void Settings::setAPGateway() {

}
void Settings::setAPSubnet() {

}
void Settings::setClientSSID() {

}
void Settings::setClientPassword() {

}
void Settings::setTShift() {

}
void Settings::setAPChannel() {

}
void Settings::setAPHidden() {

}
void Settings::setAPMaxConnection() {

}
