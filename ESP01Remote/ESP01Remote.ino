#include <IRremote.h>
 
IRsend irsend(3);

decode_results results;
 uint8_t swingButton[63] = {17, 33, 17, 33, 41, 9, 40, 10, 15, 35, 15, 35, 39, 11, 39, 11, 38, 12, 14, 36, 13, 37, 38, 12, 38, 12, 38, 12, 13, 37, 13, 37, 38, 12, 13, 37, 13, 37, 38, 11, 39, 11, 39, 11, 14, 36, 14, 36, 39, 11, 14, 36, 14, 36, 39, 11, 39, 11, 39, 11, 13, 37, 13}; // Protocol=UNKNOWN Hash=0x5A4656EA 32 bits received
  uint8_t coolButton[63] = {17, 33, 16, 34, 39, 12, 38, 12, 13, 36, 14, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 13, 37, 14, 36, 39, 11, 14, 36, 13, 37, 38, 12, 38, 12, 14, 36, 13, 37, 13, 37, 38, 12, 13, 37, 13, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39, 11, 14}; // Protocol=UNKNOWN Hash=0xF7676EB 32 bits received
  uint16_t timerButton[63] = {830, 1670, 780, 1720, 1980, 520, 1980, 520, 730, 1770, 680, 1820, 1930, 570, 1930, 570, 1930, 570, 1930, 570, 680, 1820, 680, 1820, 680, 1820, 1930, 570, 680, 1820, 1930, 570, 1880, 620, 1880, 620, 630, 1870, 630, 1870, 630, 1870, 1880, 620, 630, 1820, 1930, 570, 1930, 570, 1930, 570, 680, 1820, 680, 1820, 680, 1820, 1930, 570, 680, 1820, 1930}; // Protocol=UNKNOWN Hash=0xFCBD6E1D 32 bits received
  uint8_t speedButton[63] = {18, 32, 18, 33, 41, 9, 40, 10, 15, 35, 15, 35, 39, 11, 39, 11, 39, 11, 14, 36, 38, 12, 39, 11, 14, 36, 13, 37, 13, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39}; // Protocol=UNKNOWN Hash=0xC2292CAD 32 bits received
  uint16_t offButton[63] = {880, 1620, 880, 1620, 2030, 520, 1980, 470, 780, 1720, 780, 1720, 1980, 520, 1980, 570, 1930, 570, 680, 1820, 1880, 620, 630, 1820, 680, 1820, 1930, 570, 1930, 570, 680, 1820, 1930, 570, 680, 1820, 1930, 570, 680, 1820, 680, 1820, 1930, 570, 1880, 620, 630, 1870, 1880, 620, 630, 1870, 1880, 620, 630, 1870, 630, 1870, 1880, 620, 1880, 620, 630}; // Protocol=UNKNOWN Hash=0x2129B49F 32 bits received
  int data;
void setup() {
  Serial.begin(9600); // выставляем скорость COM порта
}

void loop() {
 
  if (Serial.available() > 0) {
    data = Serial.read();

    switch (data) {
      case 48:
        irsend.sendRaw(swingButton, 63, 38);
        break;
      case 49:
        irsend.sendRaw(coolButton, 63, 38);
        break;
      case 50:
        irsend.sendRaw(timerButton, 63, 38);
        break;
      case 51:
        irsend.sendRaw(speedButton, 63, 38);
        break;
      case 52:
        irsend.sendRaw(offButton, 63, 38);
        break;
      default:
        break;
    }
    Serial.print("I received: ");
    Serial.println(data);
  }
  delay(10);
}
