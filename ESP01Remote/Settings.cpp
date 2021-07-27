#include "Settings.h";

Settings::Settings() {
  EEPROM.begin(EEPROM_SIZE);
}

void Settings::SetSetting(ESP8266WebServer &server)
{
  setAPSSID(server.arg("APSSID"));
  setAPPassword(server.arg("APPassword"));
  setAPIpAddress(server.arg("APIpAddress"));
  setAPGateway(server.arg("APGateway"));
  setAPSubnet(server.arg("APSubnet"));
  setClientSSID(server.arg("ClientSSID"));
  setClientPassword(server.arg("ClientPassword"));
  setTShift(server.arg("TShift"));
  setAPChannel(server.arg("APChannel"));
  setAPHidden(server.arg("APHidden"));
  setAPMaxConnection(server.arg("APMaxConnection"));
  SaveSettings();
}

void Settings::SaveSettings() {
  eepromWrite(adrAPSSID, EEPROM_SIZE_VALUE, APSSID);
  eepromWrite(adrAPPassword, EEPROM_SIZE_VALUE, APPassword);
  eepromWrite(adrAPIpAddress, APIpAddress);
  eepromWrite(adrAPGateway, APGateway);
  eepromWrite(adrAPSubnet, APSubnet);
  eepromWrite(adrClientSSID, EEPROM_SIZE_VALUE, ClientSSID);
  eepromWrite(adrClientPassword, EEPROM_SIZE_VALUE, ClientPassword);
  eepromWrite(adrTShift, EEPROM_SIZE_VALUE, APSSID);
  eepromWrite(adrAPChannel, EEPROM_SIZE_VALUE, APChannel);
  eepromWrite(adrAPHidden, EEPROM_SIZE_VALUE, APHidden);
  eepromWrite(adrAPMaxConnection, EEPROM_SIZE_VALUE, APMaxConnection);
}

void Settings::ReadSettings() {
  eepromRead(adrAPSSID, EEPROM_SIZE_VALUE, APSSID);
  eepromRead(adrAPPassword, EEPROM_SIZE_VALUE, APPassword);
  APIpAddress = eepromRead(adrAPIpAddress );
  APGateway = eepromRead(adrAPGateway);
  APSubnet = eepromRead(adrAPSubnet);
  eepromRead(adrClientSSID, EEPROM_SIZE_VALUE, ClientSSID);
  eepromRead(adrClientPassword, EEPROM_SIZE_VALUE, ClientPassword);
  eepromRead(adrTShift, EEPROM_SIZE_VALUE, APSSID);
  eepromRead(adrAPChannel, EEPROM_SIZE_VALUE, APChannel);
  eepromRead(adrAPHidden, EEPROM_SIZE_VALUE, APHidden);
  eepromRead(adrAPMaxConnection, EEPROM_SIZE_VALUE, APMaxConnection);
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

String Settings::IpToString(IPAddress address) {
  return String() + address[0] + "." + address[1] + "." + address[2] + "." + address[3];
}

IPAddress Settings::eepromRead(int addr) {
  IPAddress value;
  for (int i = 0; i <= 3; i++) {
    byte readValue = EEPROM.read(addr + i);
    value[i] = int(readValue);
  }
  return value;
}

void Settings::eepromWrite(int addr, IPAddress value)
{
  uint8_t ipBuffer[4] = {(int)value[0], (int)value[1], (int)value[2], (int)value[3]};
  for (int i = 0; i <= 3; i++) {
    EEPROM.write(addr, ipBuffer[i]);
    addr += 1;
  }
  EEPROM.commit();
}

void Settings::eepromWrite(int addr, int size, char* value)
{
  // writing byte-by-byte to EEPROM
  for (int i = 0; i < size; i++) {
    EEPROM.write(addr, value[i]);
    if (value[i] == '\0')break;
    addr += 1;
  }
  EEPROM.commit();
}

void Settings::eepromRead(int addr, int size, char* value)
{
  // reading byte-by-byte from EEPROM
  for (int i = 0; i < size; i++) {
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
  ptr += IpToString(APIpAddress) ;
  ptr += "\",\"APGateway\":\"" ;
  ptr += IpToString(APGateway) ;
  ptr += "\",\"APSubnet\":\"" ;
  ptr += IpToString(APSubnet) ;
  ptr += "\",\"ClientSSID\":\"" ;
  ptr += ClientSSID ;
  ptr += "\",\"ClientPassword\":\"" ;
  ptr += ClientPassword ;
  ptr += "\",\"TShift\":\"" ;
  ptr += TShift ;
  ptr += "\",\"APChannel\":\"" ;
  ptr += APChannel ;
  ptr += "\",\"APHidden\":\"" ;
  ptr += APHidden ;
  ptr += "\",\"APMaxConnection\":\"" ;
  ptr += APMaxConnection ;
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
  return APIpAddress;
}
IPAddress Settings::getAPGateway() {
  return APGateway;
}
IPAddress Settings::getAPSubnet() {
  return APSubnet;
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
void Settings::setAPSSID(String s) {
  strcpy(APSSID, s.c_str());
}
void Settings::setAPPassword(String s) {
  strcpy(APPassword, s.c_str());
}
void Settings::setAPIpAddress(String s) {
  APIpAddress = IPfromString(s);
}
void Settings::setAPGateway(String s) {
  APGateway = IPfromString(s);
}
void Settings::setAPSubnet(String s) {
  APSubnet = IPfromString(s);
}
void Settings::setClientSSID(String s) {
  strcpy(ClientSSID, s.c_str());
}
void Settings::setClientPassword(String s) {
  strcpy(ClientPassword, s.c_str());
}
void Settings::setTShift(String s) {
  strcpy(TShift, s.c_str());
}
void Settings::setAPChannel(String s) {
  strcpy(APChannel, s.c_str());
}
void Settings::setAPHidden(String s) {
  strcpy(APHidden, s.c_str());
}
void Settings::setAPMaxConnection(String s) {
  strcpy(APMaxConnection, s.c_str());
}

void Settings::SetDefaultSettings() {
  setAPSSID(String("EMMECI"));
  setAPPassword(String("12345678"));
  setAPIpAddress(String("192.168.4.1"));
  setAPGateway(String("192.168.4.1"));
  setAPSubnet(String("255.255.255.0"));
  setClientSSID(String("WIFI"));
  setClientPassword(String("12345678"));
  setTShift(String("0"));
  setAPChannel(String("1"));
  setAPHidden(String("0"));
  setAPMaxConnection(String("8"));
  SaveSettings();
}
