#ifndef SETTING_H
#define SETTING_H
#define EEPROM_SIZE_VALUE 64
#define EEPROM_SIZE EEPROM_SIZE_VALUE*6

#include <ESP8266WebServer.h>
#include <EEPROM.h>


class Settings {
  
  private:
    char APSSID[EEPROM_SIZE_VALUE];
    char APPassword[EEPROM_SIZE_VALUE];
    IPAddress APIpAddress;
    IPAddress APGateway;
    IPAddress APSubnet;
    char ClientSSID[EEPROM_SIZE_VALUE];
    char ClientPassword[EEPROM_SIZE_VALUE];
    char TShift[EEPROM_SIZE_VALUE];
    char APChannel[EEPROM_SIZE_VALUE];
    char APHidden[EEPROM_SIZE_VALUE];
    char APMaxConnection[EEPROM_SIZE_VALUE];

    const int adrAPSSID = 0 * EEPROM_SIZE_VALUE;
    const int adrAPPassword = 1 * EEPROM_SIZE_VALUE;
    const int adrAPIpAddress  =  2 * EEPROM_SIZE_VALUE;
    const int adrClientSSID  =  3 * EEPROM_SIZE_VALUE;
    const int adrClientPassword  =  4 * EEPROM_SIZE_VALUE;
    const int adrTShift  = 5 * EEPROM_SIZE_VALUE;

    void eepromWrite(int addr, char* value);
    void eepromRead(int addr, char* value);

  public:
    Settings();
    void SaveSetting(ESP8266WebServer &server);
    void ReadSettings();
    IPAddress IPfromString(String s);
    String getJson();
    /** Getters */
    char* getAPSSID();
    char* getAPPassword();
    IPAddress getAPIpAddress();
    IPAddress getAPGateway();
    IPAddress getAPSubnet();
    char* getClientSSID();
    char* getClientPassword();
    char* getTShift();
    char* getAPChannel();
    char* getAPHidden();
    char* getAPMaxConnection();

    /** Setters */
    void setAPSSID();
    void setAPPassword();
    void setAPIpAddress();
    void setAPGateway();
    void setAPSubnet();
    void setClientSSID();
    void setClientPassword();
    void setTShift();
    void setAPChannel();
    void setAPHidden();
    void setAPMaxConnection();

};
#endif
