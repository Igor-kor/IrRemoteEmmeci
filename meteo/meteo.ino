#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <IRremote.h>

IRsend irsend(3);

decode_results results;
uint8_t swingButton[63] = {17, 33, 17, 33, 41, 9, 40, 10, 15, 35, 15, 35, 39, 11, 39, 11, 38, 12, 14, 36, 13, 37, 38, 12, 38, 12, 38, 12, 13, 37, 13, 37, 38, 12, 13, 37, 13, 37, 38, 11, 39, 11, 39, 11, 14, 36, 14, 36, 39, 11, 14, 36, 14, 36, 39, 11, 39, 11, 39, 11, 13, 37, 13}; // Protocol=UNKNOWN Hash=0x5A4656EA 32 bits received
uint8_t coolButton[63] = {17, 33, 16, 34, 39, 12, 38, 12, 13, 36, 14, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 13, 37, 14, 36, 39, 11, 14, 36, 13, 37, 38, 12, 38, 12, 14, 36, 13, 37, 13, 37, 38, 12, 13, 37, 13, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39, 11, 14}; // Protocol=UNKNOWN Hash=0xF7676EB 32 bits received
uint16_t timerButton[63] = {830, 1670, 780, 1720, 1980, 520, 1980, 520, 730, 1770, 680, 1820, 1930, 570, 1930, 570, 1930, 570, 1930, 570, 680, 1820, 680, 1820, 680, 1820, 1930, 570, 680, 1820, 1930, 570, 1880, 620, 1880, 620, 630, 1870, 630, 1870, 630, 1870, 1880, 620, 630, 1820, 1930, 570, 1930, 570, 1930, 570, 680, 1820, 680, 1820, 680, 1820, 1930, 570, 680, 1820, 1930}; // Protocol=UNKNOWN Hash=0xFCBD6E1D 32 bits received
uint8_t speedButton[63] = {18, 32, 18, 33, 41, 9, 40, 10, 15, 35, 15, 35, 39, 11, 39, 11, 39, 11, 14, 36, 38, 12, 39, 11, 14, 36, 13, 37, 13, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39}; // Protocol=UNKNOWN Hash=0xC2292CAD 32 bits received
uint16_t offButton[63] = {880, 1620, 880, 1620, 2030, 520, 1980, 470, 780, 1720, 780, 1720, 1980, 520, 1980, 570, 1930, 570, 680, 1820, 1880, 620, 630, 1820, 680, 1820, 1930, 570, 1930, 570, 680, 1820, 1930, 570, 680, 1820, 1930, 570, 680, 1820, 680, 1820, 1930, 570, 1880, 620, 630, 1870, 1880, 620, 630, 1870, 1880, 620, 630, 1870, 630, 1870, 1880, 620, 1880, 620, 630}; // Protocol=UNKNOWN Hash=0x2129B49F 32 bits received

// Раскомментируйте одну из строк ниже в зависимости от того, какой датчик вы используете!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/* Установите здесь свои SSID и пароль */
const char* ssid = "wifi";   // SSID
const char* password = "12300111";  // пароль

ESP8266WebServer server(80);

// датчик DHT
uint8_t DHTPin = 0;

// инициализация датчика DHT.
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity;

void setup()
{
  //Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  pinMode(3, OUTPUT);
  dht.begin();
  // подключаемся к локальной wi-fi сети
  //WiFi.begin(ssid, password);
  WiFi.softAP("EMMECI", "12345678");
  
  // проверить, подключился ли wi-fi модуль к wi-fi сети
 /* while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }*/
  server.on("/", handle_OnConnect);
  server.on("/remote", handle_Remote);
  server.on("/temp", handle_Temp);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void handle_Remote() {
  String data = server.arg("msg");
  switch (data.toInt()) {
    case 0:
      irsend.sendRaw(swingButton, 63, 38);
      break;
    case 1:
      irsend.sendRaw(coolButton, 63, 38);
      break;
    case 2:
      irsend.sendRaw(timerButton, 63, 38);
      break;
    case 3:
      irsend.sendRaw(speedButton, 63, 38);
      break;
    case 4:
      irsend.sendRaw(offButton, 63, 38);
      break;
    default:
      break;
  }
  server.send(200, "text/html", "ok");
}

void handle_Temp() {
  Temperature = dht.readTemperature(); // получить значение температуры
  Humidity = dht.readHumidity();       // получить значение влажности
  String ptr = "";
  ptr += (int)Temperature;
  ptr += ";" ;
  ptr += (int)Humidity;
  server.send(200, "text/html", ptr);
}

void handle_OnConnect()
{
  Temperature = dht.readTemperature(); // получить значение температуры
  Humidity = dht.readHumidity();       // получить значение влажности
  server.send(200, "text/html", SendHTML(Temperature, Humidity));
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat, float Humiditystat)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>EMMECI</title>\n";
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 50px auto 30px;}\n";
  ptr += ".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  ptr += ".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}\n";
  ptr += ".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  ptr += ".humidity{font-weight: 300;font-size: 60px;color: #3498db;}\n";
  ptr += ".temperature-icon{background-color: #f39c12;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  ptr += ".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  ptr += ".temperature{font-weight: 300;font-size: 60px;color: #f39c12;}\n";
  ptr += ".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  ptr += ".data{padding: 10px;}\n";
  ptr += ".btn {";
  ptr += " margin: 10px 20px;";
  ptr += " padding: 15px 34px;";
  ptr += "overflow: hidden;";
  ptr += "font-family: 'Montserrat Alternates', sans-serif;";
  ptr += "color: white;";
  ptr += "letter-spacing: 1px;";
  ptr += " text-transform: uppercase;";
  ptr += "font-weight: 600;";
  ptr += "border-width: 0;";
  ptr += "transform: matrix3d(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);";
  ptr += " outline: none;";
  ptr += "cursor: pointer;";
  ptr += "}";
  ptr += ".btn1 {";
  ptr += "  background: linear-gradient(90deg, #c600b2, #7619fd);";
  ptr += "}";
  ptr += ".btn2 {";
  ptr += " background: linear-gradient(90deg, #a3ffb0, #2bd9e8);";
  ptr += "}";
  ptr += ".btn3 {";
  ptr += "   background: linear-gradient(90deg, #49c2ff, #6a27d2);";
  ptr += "}";
  ptr += ".btn:before, .btn:after {";
  ptr += "  content: \"\";";
  ptr += " position: absolute;";
  ptr += " z-index: -1;";
  ptr += " width: 100%;";
  ptr += "height: 100%;";
  ptr += "left: -110%;";
  ptr += "top: 0;";
  ptr += "opacity: 0.1;";
  ptr += "background: black;";
  ptr += "transform: skewX(25deg);";
  ptr += "transition: .5s ease;";
  ptr += "}";
  ptr += ".btn:hover:before {";
  ptr += "left: -10%;";
  ptr += "transition: .3s ease;";
  ptr += "}";
  ptr += ".btn:hover:after {";
  ptr += "  left: -20%;";
  ptr += "}";
  ptr += "</style>\n";
  ptr += "<script> const request = new XMLHttpRequest(); </script>\n";
  ptr += "<script> const requestreload = new XMLHttpRequest(); </script>\n";
  ptr += "<script> requestreload.addEventListener(\"readystatechange\", () => { \n";
  ptr += " if(requestreload.readyState === 4 && requestreload.status === 200) {   \n"; 
  ptr += "  document.getElementById(\"temp\").innerText  = requestreload.responseText.split(';')[0];\n";
  ptr += "  document.getElementById(\"hum\").innerText = requestreload.responseText.split(';')[1];\n";
  ptr += "  }\n";
  ptr += "});\n";
  ptr += "let timerId = setInterval(() => {\n";
  ptr += "requestreload.open('POST', document.URL+'temp', true);requestreload.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');requestreload.send('msg=0')\n";
  ptr += "}, 2000);</script>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<div class=\"data\">\n";
  ptr += "<div class=\"side-by-side temperature-icon\">\n";
  ptr += "<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n";
  ptr += "width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">\n";
  ptr += "<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142\n";
  ptr += "c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491\n";
  ptr += "c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463\n";
  ptr += "c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394\n";
  ptr += "c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>\n";
  ptr += "</svg>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"side-by-side temperature-text\">Temperature</div>\n";
  ptr += "<div class=\"side-by-side temperature\"><div id=\"temp\" >";
  ptr += (int)Temperaturestat;
  ptr += "</div><span class=\"superscript\">°C</span></div>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"data\">\n";
  ptr += "<div class=\"side-by-side humidity-icon\">\n";
  ptr += "<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n\"; width=\"12px\" height=\"17.955px\" viewBox=\"0 0 13 17.955\" enable-background=\"new 0 0 13 17.955\" xml:space=\"preserve\">\n";
  ptr += "<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057\n";
  ptr += "c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>\n";
  ptr += "</svg>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"side-by-side humidity-text\">Humidity</div>\n";
  ptr += "<div class=\"side-by-side humidity\"><div id=\"hum\" >";
  ptr += (int)Humiditystat;
  ptr += "</div><span class=\"superscript\">%</span></div>\n";
  ptr += "</div>\n";
  ptr += "<button class=\"btn btn1\" onclick=\"request.open('POST', document.URL+'remote', true);request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');request.send('msg=0');\">SWING</button><br>";
  ptr += "<button class=\"btn btn2\" onclick=\"request.open('POST', document.URL+'remote', true);request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');request.send('msg=1');\">COOL</button><br>";
  ptr += "<button class=\"btn btn3\" onclick=\"request.open('POST', document.URL+'remote', true);request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');request.send('msg=2');\">TIMER</button><br>";
  ptr += "<button class=\"btn btn1\" onclick=\"request.open('POST', document.URL+'remote', true);request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');request.send('msg=3');\">SPEED</button><br>";
  ptr += "<button class=\"btn btn2\" onclick=\"request.open('POST', document.URL+'remote', true);request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');request.send('msg=4');\">OFF</button><br>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
