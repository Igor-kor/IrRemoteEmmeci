#ifndef SETTING_H
#define SETTING_H
#define EEPROM_SIZE_VALUE 64
#define EEPROM_SIZE EEPROM_SIZE_VALUE*12

#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <IPAddress.h>
#include <Arduino_JSON.h>


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
    const int adrAPGateway  =  3 * EEPROM_SIZE_VALUE;
    const int adrAPSubnet  =  4 * EEPROM_SIZE_VALUE;
    const int adrClientSSID  =  5 * EEPROM_SIZE_VALUE;
    const int adrClientPassword  =  6 * EEPROM_SIZE_VALUE;
    const int adrTShift  = 7 * EEPROM_SIZE_VALUE;
    const int adrAPChannel  = 8 * EEPROM_SIZE_VALUE;
    const int adrAPHidden  = 9 * EEPROM_SIZE_VALUE;
    const int adrAPMaxConnection  = 10 * EEPROM_SIZE_VALUE;

    void eepromWrite(int addr, IPAddress value);
    void eepromWrite(int addr, int size, char* value);
    IPAddress eepromRead(int addr);
    void eepromRead(int addr, int size, char* value);
    void SaveSettings();

  public:
    Settings();
    void SetDefaultSettings();
    void SetSetting(ESP8266WebServer &server);
    void ReadSettings();
    IPAddress IPfromString(String s);
    String IpToString(IPAddress address);
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
    int getIntTShift();
    char* getAPChannel();
    char* getAPHidden();
    char* getAPMaxConnection();

    /** Setters */
    void setAPSSID(String s);
    void setAPPassword(String s);
    void setAPIpAddress(String s);
    void setAPGateway(String s);
    void setAPSubnet(String s);
    void setClientSSID(String s);
    void setClientPassword(String s);
    void setTShift(String s);
    void setAPChannel(String s);
    void setAPHidden(String s);
    void setAPMaxConnection(String s);

};
#endif
