/*
   todo:
   1 - сделать открытие настроек после прогрузки данных, если нет то выдать ошибку
   2 - сделать трим на все поля
   3 - сделать фильтры на все поля с масками
  -+5 - сделать функцию рестарта
   7 - сделать проверку всех входящих данных сохранения настроек
   8 - сделать накопление данных о влажности чтобы сгладить и усреднить данные
   9 - принимать json формат
   10 - сделать подключение как клиента к точке, при этом выключить раздачу(при невозможности подключиться включать точку доступа)
   11 - указать правильный размер eeprom и переменных
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <IRremote.h>
#include <Arduino_JSON.h>
#include "Settings.h";

#define IR_DIODE_PIN 3
#define SET_DEFAULT_PIN 3

IRsend irsend(IR_DIODE_PIN);

uint8_t swingButton[63] = {17, 33, 17, 33, 41, 9, 40, 10, 15, 35, 15, 35, 39, 11, 39, 11, 38, 12, 14, 36, 13, 37, 38, 12, 38, 12, 38, 12, 13, 37, 13, 37, 38, 12, 13, 37, 13, 37, 38, 11, 39, 11, 39, 11, 14, 36, 14, 36, 39, 11, 14, 36, 14, 36, 39, 11, 39, 11, 39, 11, 13, 37, 13}; // Protocol=UNKNOWN Hash=0x5A4656EA 32 bits received
uint8_t coolButton[63] = {17, 33, 16, 34, 39, 12, 38, 12, 13, 36, 14, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 13, 37, 14, 36, 39, 11, 14, 36, 13, 37, 38, 12, 38, 12, 14, 36, 13, 37, 13, 37, 38, 12, 13, 37, 13, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39, 11, 14}; // Protocol=UNKNOWN Hash=0xF7676EB 32 bits received
uint16_t timerButton[63] = {830, 1670, 780, 1720, 1980, 520, 1980, 520, 730, 1770, 680, 1820, 1930, 570, 1930, 570, 1930, 570, 1930, 570, 680, 1820, 680, 1820, 680, 1820, 1930, 570, 680, 1820, 1930, 570, 1880, 620, 1880, 620, 630, 1870, 630, 1870, 630, 1870, 1880, 620, 630, 1820, 1930, 570, 1930, 570, 1930, 570, 680, 1820, 680, 1820, 680, 1820, 1930, 570, 680, 1820, 1930}; // Protocol=UNKNOWN Hash=0xFCBD6E1D 32 bits received
uint8_t speedButton[63] = {18, 32, 18, 33, 41, 9, 40, 10, 15, 35, 15, 35, 39, 11, 39, 11, 39, 11, 14, 36, 38, 12, 39, 11, 14, 36, 13, 37, 13, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39, 11, 39, 11, 14, 36, 39, 11, 39, 11, 14, 36, 14, 36, 14, 36, 39}; // Protocol=UNKNOWN Hash=0xC2292CAD 32 bits received
uint16_t offButton[63] = {880, 1620, 880, 1620, 2030, 520, 1980, 470, 780, 1720, 780, 1720, 1980, 520, 1980, 570, 1930, 570, 680, 1820, 1880, 620, 630, 1820, 680, 1820, 1930, 570, 1930, 570, 680, 1820, 1930, 570, 680, 1820, 1930, 570, 680, 1820, 680, 1820, 1930, 570, 1880, 620, 630, 1870, 1880, 620, 630, 1870, 1880, 620, 630, 1870, 630, 1870, 1880, 620, 1880, 620, 630}; // Protocol=UNKNOWN Hash=0x2129B49F 32 bits received

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

ESP8266WebServer server(80);

// датчик DHT
uint8_t DHTPin = 0;
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity;

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html><html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><link rel="icon" type="image/svg+xml" href="files/favicon.svg"><meta name="theme-color" content="#ffffff"><title>Emmeci</title><style>html{font-family:'Open Sans',sans-serif;display:block;margin:0 auto;text-align:center;color:#ccc}body{background-color:#333;margin-top:50px}.side-by-side{display:inline-block;vertical-align:middle;position:relative}.humidity-icon{background-color:#3498db;width:30px;height:30px;border-radius:50%;line-height:36px}.humidity-text{font-weight:600;padding-left:15px;font-size:19px;width:160px;text-align:left}.humidity{font-weight:500;font-size:60px;color:#3498db}.temperature-icon{background-color:#f39c12;width:30px;height:30px;border-radius:50%;line-height:40px}.temperature-text{font-weight:600;padding-left:15px;font-size:19px;width:160px;text-align:left}.temperature{font-weight:500;font-size:60px;color:#f39c12}.superscript{font-size:17px;font-weight:600;position:absolute;right:-20px;top:15px}.data{padding:10px}.btn{display:block;vertical-align:middle;position:relative;width:150px;margin:10px auto;padding:15px 34px;overflow:hidden;font-family:'Montserrat Alternates',sans-serif;color:#fff;letter-spacing:1px;text-transform:uppercase;font-weight:600;border-width:0;transform:matrix3d(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);outline:0;cursor:pointer}.btn1{background:linear-gradient(90deg,#c600b2,#7619fd)}.btn2{background:linear-gradient(90deg,#4764b3,#00e6ff)}.btn3{background:linear-gradient(90deg,#feff36,#cc481f)}.btn4{background:linear-gradient(90deg,#c77474,#b224d4)}.btn5{background:linear-gradient(90deg,#ff1a1a,#d8ba46)}.btn:after,.btn:before{content:"";position:absolute;z-index:-1;width:100%;height:100%;left:-110%;top:0;opacity:.1;background:#000;transform:skewX(25deg);transition:.5s ease}.btn:hover:before{left:-10%;transition:.3s ease}.btn:hover:after{left:-20%}.setting{display:inline-block;position:absolute;right:30px;margin-top:-20px}.modal{display:none;position:fixed;z-index:1;left:0;top:0;width:100%;height:100%;overflow:auto;background-color:#000;background-color:rgba(0,0,0,.9)}.modal-content{background-color:#292929;margin:auto auto;width:90%;min-height:90%;text-align:left}.close{position:absolute;color:#aaa;right:30px;font-size:28px;font-weight:700}.close:focus,.close:hover{color:#fff;text-decoration:none;cursor:pointer}.modal-content>h1{text-align:center;padding-top:20px}.modal-form>label{padding-left:50px;font-size:1.1rem;text-align:left;display:block;margin-top:20px}.modal-form{margin-right:auto;margin-left:auto;width:300px}.modal-form>label>input{margin-left:-20px;margin-right:auto;display:block;padding:12px 20px;font-size:1rem;border-style:solid;border-color:#333;border-radius:5px;text-align:center;outline:transparent}.btn-save{background:#666;position:relative;width:240px;margin:40px 30px auto;padding:15px 34px;font-size:1rem;overflow:hidden;color:#fff;border-style:solid;border-color:#333;border-radius:5px;text-align:center;outline:transparent}#Layer_4{display:none}</style><script>const requestRemote=new XMLHttpRequest,requestReload=new XMLHttpRequest;function getValues(){4===requestReload.readyState&&200===requestReload.status&&(document.getElementById("temp").innerText=requestReload.response.Temperature,document.getElementById("hum").innerText=requestReload.response.Humidity,document.getElementById("Layer_4").style.display="none"),4===requestReload.readyState&&200!==requestReload.status&&(document.getElementById("Layer_4").style.display="inline-block")}function sendButtonClick(e){requestRemote.open("POST",document.URL+"remote",!0),requestRemote.setRequestHeader("Content-type","application/x-www-form-urlencoded"),requestRemote.send("msg="+e)}function restart(){requestRemote.open("GET",document.URL+"restart",!0),requestRemote.send(null)}requestReload.addEventListener("readystatechange",getValues),setInterval(()=>{requestReload.open("GET",document.URL+"temp",!0),requestReload.responseType="json",requestReload.send(null)},5e3),window.onload=function(){getValues();const e=document.getElementById("myModal"),t=document.getElementsByClassName("modal-form")[0],s=document.getElementsByClassName("setting")[0],n=document.getElementsByClassName("close")[0];s.addEventListener("click",function(){e.style.display="block",getSetting()}),n.onclick=function(){e.style.display="none"},t.addEventListener("submit",sendSetting),window.onclick=function(t){t.target===e&&(e.style.display="none")}};const requestGetSettings=new XMLHttpRequest,requestSendSettings=new XMLHttpRequest;async function sendSetting(e){e.preventDefault(),e.stopImmediatePropagation(),requestSendSettings.open("POST",document.URL+"setting",!0),requestSendSettings.send(new FormData(document.querySelector(".modal-form")))}function getSetting(){requestGetSettings.open("GET",document.URL+"setting",!0),requestGetSettings.responseType="json",requestGetSettings.send(null)}requestSendSettings.addEventListener("readystatechange",function(){4===requestSendSettings.readyState&&200===requestSendSettings.status&&document.getElementsByClassName("close")[0].click(),4===requestSendSettings.readyState&&200!==requestSendSettings.status&&alert("Error save settings, check connect!")}),requestGetSettings.addEventListener("readystatechange",function(){if(4===requestGetSettings.readyState&&200===requestGetSettings.status){const e=document.querySelector(".modal-form");e.APSSID.value=requestGetSettings.response.APSSID,e.APPassword.value=requestGetSettings.response.APPassword,e.APIpAddress.value=requestGetSettings.response.APIpAddress,e.APGateway.value=requestGetSettings.response.APGateway,e.APSubnet.value=requestGetSettings.response.APSubnet,e.ClientSSID.value=requestGetSettings.response.ClientSSID,e.ClientPassword.value=requestGetSettings.response.ClientPassword,e.TShift.value=requestGetSettings.response.TShift,e.APChannel.value=requestGetSettings.response.APChannel,e.APHidden.value=requestGetSettings.response.APHidden,e.APMaxConnection.value=requestGetSettings.response.APMaxConnection}4===requestGetSettings.readyState&&200!==requestGetSettings.status&&alert("Error load settings, check connect!")});</script></head><body><div id="webpage"><div class="setting"><svg id="Layer_4" xmlns="http://www.w3.org/2000/svg" x="0px" y="0px" width="30px" height="30px" viewBox="0 0 286.036 286.036" style=":new 0 0 286.036 286.036;" xml:space="preserve"><g><path style="fill:#E2574C;" d="M231.641,113.009c-3.915-40.789-37.875-72.792-79.684-72.792c-32.351,0-60.053,19.201-72.819,46.752
   c-3.844-1.216-7.849-2.056-12.095-2.056c-22.214,0-40.226,18.021-40.226,40.226c0,4.416,0.885,8.591,2.199,12.551
    C11.737,147.765,0,166.269,0,187.705c0,32.092,26.013,58.105,58.105,58.105v0.009h160.896v-0.009
    c37.035,0,67.035-30.009,67.035-67.044C286.036,146.075,262.615,118.927,231.641,113.009z M184.237,197.627
    c-4.219,4.228-11.049,4.228-15.268,0l-21.678-21.669l-21.4,21.409c-4.21,4.201-11.013,4.201-15.223,0
    c-4.201-4.21-4.201-11.013,0-15.223l21.4-21.4l-21.669-21.669c-4.219-4.228-4.219-11.058,0-15.277
    c4.219-4.228,11.049-4.228,15.268,0l21.669,21.678l21.686-21.686c4.21-4.21,11.013-4.21,15.223,0c4.21,4.21,4.21,11.022,0,15.223
    L162.559,160.7l21.678,21.669C188.465,186.579,188.465,193.426,184.237,197.627z"/></g></svg> <svg enable-background="new 0 0 24 24" height="30" viewBox="0 0 24 24" width="30" xmlns="http://www.w3.org/2000/svg"><path d="m22.683 9.394-1.88-.239c-.155-.477-.346-.937-.569-1.374l1.161-1.495c.47-.605.415-1.459-.122-1.979l-1.575-1.575c-.525-.542-1.379-.596-1.985-.127l-1.493 1.161c-.437-.223-.897-.414-1.375-.569l-.239-1.877c-.09-.753-.729-1.32-1.486-1.32h-2.24c-.757 0-1.396.567-1.486 1.317l-.239 1.88c-.478.155-.938.345-1.375.569l-1.494-1.161c-.604-.469-1.458-.415-1.979.122l-1.575 1.574c-.542.526-.597 1.38-.127 1.986l1.161 1.494c-.224.437-.414.897-.569 1.374l-1.877.239c-.753.09-1.32.729-1.32 1.486v2.24c0 .757.567 1.396 1.317 1.486l1.88.239c.155.477.346.937.569 1.374l-1.161 1.495c-.47.605-.415 1.459.122 1.98l1.575 1.575c.526.541 1.379.595 1.985.126l1.494-1.161c.437.224.897.415 1.374.569l.239 1.876c.09.753.73 1.32 1.486 1.32h2.24c.757 0 1.396-.567 1.486-1.317l.239-1.88c.477-.155.937-.346 1.374-.569l1.495 1.161c.605.47 1.459.415 1.979-.122l1.575-1.575c.542-.526.597-1.379.127-1.985l-1.161-1.494c.224-.437.414-.898.569-1.374l1.876-.239c.753-.09 1.32-.729 1.32-1.486v-2.24c.001-.756-.566-1.395-1.316-1.485zm-10.683 7.606c-2.757 0-5-2.243-5-5s2.243-5 5-5 5 2.243 5 5-2.243 5-5 5z" fill="#90a4ae"/><path d="m12 0h-1.12c-.757 0-1.396.567-1.486 1.317l-.239 1.88c-.478.155-.938.345-1.375.569l-1.494-1.161c-.276-.214-.603-.319-.928-.319-.386 0-.768.149-1.051.441l-1.575 1.574c-.542.526-.597 1.38-.127 1.986l1.161 1.494c-.224.437-.414.897-.569 1.374l-1.877.239c-.753.09-1.32.729-1.32 1.486v2.24c0 .757.567 1.396 1.317 1.486l1.88.239c.155.477.346.937.569 1.374l-1.161 1.495c-.47.605-.415 1.459.122 1.98l1.575 1.575c.286.295.67.445 1.058.445.324 0 .651-.105.927-.318l1.494-1.161c.437.224.897.415 1.374.569l.239 1.876c.09.753.73 1.32 1.486 1.32h1.12v-7c-2.757 0-5-2.243-5-5s2.243-5 5-5z" fill="#7d8f97"/></svg></div><div class="data"><div class="side-by-side temperature-icon"><svg id="Layer_1" xmlns="http://www.w3.org/2000/svg" x="0px" y="0px" width="9.915px" height="22px" viewBox="0 0 9.915 22" enable-background="new 0 0 9.915 22" xml:space="preserve"><path fill="#FFFFFF" d="M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142
c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491
c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463
c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394
c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z"></path></svg></div><div class="side-by-side temperature-text">Temperature</div><div class="side-by-side temperature"><div id="temp">0</div><span class="superscript">°C</span></div></div><div class="data"><div class="side-by-side humidity-icon"><svg id="Layer_2" xmlns="http://www.w3.org/2000/svg" x="0px" y="0px" width="12px" height="17.955px" viewBox="0 0 13 17.955" enable-background="new 0 0 13 17.955" xml:space="preserve"><path fill="#FFFFFF" d="M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057
c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217"></path></svg></div><div class="side-by-side humidity-text">Humidity</div><div class="side-by-side humidity"><div id="hum">0</div><span class="superscript">%</span></div></div><button class="btn btn1" onclick="sendButtonClick(0);">SWING</button> <button class="btn btn2" onclick="sendButtonClick(1);">COOL</button> <button class="btn btn3" onclick="sendButtonClick(2);">TIMER</button> <button class="btn btn4" onclick="sendButtonClick(3);">SPEED</button> <button class="btn btn5" onclick="sendButtonClick(4);">OFF</button><div id="myModal" class="modal"><div class="modal-content"><span class="close">&times;</span><h1>Settings</h1><form class="modal-form"><label>Wifi AP SSID <input name="APSSID" type="text"></label> <label>Wifi AP password <input name="APPassword" type="text"></label> <label>Wifi AP channel <input name="APChannel" type="text"></label> <label>Wifi AP hidden <input name="APHidden" type="text"></label> <label>Wifi AP max_connection <input name="APMaxConnection" type="text"></label> <label>Wifi AP ip address <input name="APIpAddress" type="text"></label> <label>Wifi AP gateway <input name="APGateway" type="text"></label> <label>Wifi AP subnet <input name="APSubnet" type="text"></label> <label>Temperature shift <input name="TShift" type="text"></label> <label>Wifi connected SSID <input name="ClientSSID" type="text"></label> <label>Wifi connected password <input name="ClientPassword" type="text"></label> <button type="submit" class="btn-save" name="btn-modal">save</button> <button class="btn-save" onclick="restart();">Restart ESP</button> <label><br></label></form></div></div></div></body></html>)rawliteral";

const char favicon_svg[] PROGMEM = R"rawliteral(<svg height="512" viewBox="0 0 60 53" width="512" xmlns="http://www.w3.org/2000/svg"><g id="Page-1" fill="none" fill-rule="evenodd"><g id="003---AC-Unit" fill-rule="nonzero"><path id="Shape" d="m59 4v17c0 1.6568542-1.3431458 3-3 3h-4c0-1.6568542-1.3431458-3-3-3h-19.73c-2.5981077-4.9206165-7.7055926-7.9997332-13.27-8-1.7036777-.0024862-3.3950597.2884315-5 .86v-9.86c0-1.65685425 1.3431458-3 3-3h4v5c0 1.65685425 1.3431458 3 3 3h28c1.6568542 0 3-1.34314575 3-3v-5h4c1.6568542 0 3 1.34314575 3 3z" fill="#f5f5f5"/><path id="Shape" d="m18 9h3c-1.6568542 0-3-1.34314575-3-3v-5h-3v5c0 1.65685425 1.3431458 3 3 3z" fill="#cfd8dc"/><path id="Shape" d="m56 1h-3c1.6568542 0 3 1.34314575 3 3v17c0 1.6568542-1.3431458 3-3 3h3c1.6568542 0 3-1.3431458 3-3v-17c0-1.65685425-1.3431458-3-3-3z" fill="#cfd8dc"/><path id="Shape" d="m13 13c-.669177.0039996-1.3373364.0527752-2 .146v.714c1.1289164-.3996848 2.3021552-.6609269 3.494-.778-.494-.048-.989-.082-1.494-.082z" fill="#cfd8dc"/><path id="Shape" d="m52 1v5c0 1.65685425-1.3431458 3-3 3h-28c-1.6568542 0-3-1.34314575-3-3v-5z" fill="#cfd8dc"/><path id="Shape" d="m49 1v5c0 1.65685425-1.3431458 3-3 3h3c1.6568542 0 3-1.34314575 3-3v-5z" fill="#9e9e9e"/><path id="Shape" d="m52 24v2c0 .5522847-.4477153 1-1 1h-20.04c-.1313525-2.097236-.7074409-4.1425205-1.69-6h19.73c1.6568542 0 3 1.3431458 3 3z" fill="#9e9e9e"/><path id="Shape" d="m49 21h-3c1.6568542 0 3 1.3431458 3 3v2c0 .5522847-.4477153 1-1 1h3c.5522847 0 1-.4477153 1-1v-2c0-1.6568542-1.3431458-3-3-3z" fill="#707070"/><path id="Shape" d="m31 28c.0010861 7.6284905-5.7235162 14.0428165-13.3030324 14.9058365-7.5795161.86302-14.59958688-4.10017-16.31348758-11.5336357-1.71390069-7.4334658 2.42431013-14.9693705 9.61651998-17.5122008 1.6049403-.5715685 3.2963223-.8624862 5-.86 5.5644074.0002668 10.6718923 3.0793835 13.27 8 .9825591 1.8574795 1.5586475 3.902764 1.69 6 .03.33.04.66.04 1z" fill="#02a9f4"/><path id="Shape" d="m30.96 27c-.1313525-2.097236-.7074409-4.1425205-1.69-6-2.5981077-4.9206165-7.7055926-7.9997332-13.27-8-.509 0-1.01.033-1.506.082 5.0036919.5005568 9.4244752 3.473023 11.776 7.918.9825591 1.8574795 1.5586475 3.902764 1.69 6 .03.33.04.66.04 1-.0032017 7.702024-5.8368612 14.1510266-13.5 14.924.494.049.994.076 1.5.076 8.2831279-.0027597 14.9972403-6.7168721 15-15 0-.34-.01-.67-.04-1z" fill="#0377bc"/><path id="Shape" d="m24.741 29.162-3.229.865-3.512-2.027 3.512-2.027 3.229.865c.084364.0232732.1714847.0350462.259.035.5019729.0002301.9263288-.3717159.991892-.8693888.0655632-.497673-.247986-.9668303-.732892-1.0966112l-2.659-.714.714-2.665c.1398151-.5324122-.1769484-1.0777088-.7087028-1.2200053-.5317543-.1422966-1.0785222.1719204-1.2232972.7030053l-.865 3.23-3.517 2.027v-4.055l2.364-2.364c.3789722-.3923789.3735524-1.0160848-.0121814-1.4018186s-1.0094397-.3911536-1.4018186-.0121814l-1.95 1.95-1.95-1.95c-.3923789-.3789722-1.0160848-.3735524-1.4018186.0121814s-.3911536 1.0094397-.0121814 1.4018186l2.364 2.364v4.055l-3.512-2.027-.865-3.23c-.144775-.5310849-.6915429-.8453019-1.22329723-.7030053-.53175434.1422965-.84851786.6875931-.70870277 1.2200053l.709 2.665-2.664.714c-.48585399.1300043-.79955522.6006311-.73261895 1.0991035.06693628.4984724.49368007.8696701.99661895.8668965.0875153.0000462.17463598-.0117268.259-.035l3.229-.865 3.512 2.027-3.512 2.027-3.229-.865c-.3486122-.1036823-.72603054-.0099144-.98558469.2448642-.25955414.2547786-.36031391.6303908-.26312256.9808681.09719136.3504773.37700731.6205514.73070725.7052677l2.664.714-.714 2.665c-.06956524.256888-.03385886.5309094.09921336.7613927s.35253131.3984129.60978664.4666073c.0846917.0233384.17215152.0351119.26.035.4524951-.0005181.8482565-.3048238.965-.742l.865-3.23 3.51-2.031v4.055l-2.364 2.364c-.2599566.2510745-.3642126.6228779-.2726972.9725073.0915155.3496295.3645604.6226744.7141899.7141899.3496294.0915154.7214328-.0127406.9725073-.2726972l1.95-1.95 1.95 1.95c.2510745.2599566.6228779.3642126.9725073.2726972.3496295-.0915155.6226744-.3645604.7141899-.7141899.0915154-.3496294-.0127406-.7214328-.2726972-.9725073l-2.364-2.364v-4.055l3.512 2.027.865 3.23c.1167435.4371762.5125049.7414819.965.742.0878485.0001119.1753083-.0116616.26-.035.5330514-.1429996.8494732-.6908077.707-1.224l-.709-2.665 2.664-.714c.3536999-.0847163.6335159-.3547904.7307072-.7052677.0971914-.3504773-.0035684-.7260895-.2631225-.9808681-.2595542-.2547786-.6369725-.3485465-.9855847-.2448642z" fill="#f5f5f5"/><g fill="#000"><path id="Shape" d="m56 0h-42c-2.209139 0-4 1.790861-4 4v5.67c0 .5522847.4477153 1 1 1s1-.4477153 1-1v-5.67c0-1.1045695.8954305-2 2-2h3v4c0 2.209139 1.790861 4 4 4h28c2.209139 0 4-1.790861 4-4v-4h3c1.1045695 0 2 .8954305 2 2v17c0 1.1045695-.8954305 2-2 2h-3.142c-.4545933-1.7606851-2.0395884-2.9931852-3.858-3h-15.34c-.5522847 0-1 .4477153-1 1s.4477153 1 1 1h15.34c1.1045695 0 2 .8954305 2 2v2h-16.02c-.5522847 0-1 .4477153-1 1s.4477153 1 1 1h16.02c1.1045695 0 2-.8954305 2-2v-1h3c2.209139 0 4-1.790861 4-4v-17c0-2.209139-1.790861-4-4-4zm-5 6c0 1.1045695-.8954305 2-2 2h-28c-1.1045695 0-2-.8954305-2-2v-4h32z"/><path id="Shape" d="m52 14h3c.5522847 0 1-.4477153 1-1s-.4477153-1-1-1h-3c-.5522847 0-1 .4477153-1 1s.4477153 1 1 1z"/><path id="Shape" d="m52 18h3c.5522847 0 1-.4477153 1-1s-.4477153-1-1-1h-3c-.5522847 0-1 .4477153-1 1s.4477153 1 1 1z"/><path id="Shape" d="m43 46v-14c0-.5522847-.4477153-1-1-1s-1 .4477153-1 1v14c0 .5522847.4477153 1 1 1s1-.4477153 1-1z"/><path id="Shape" d="m16 44c8.836556 0 16-7.163444 16-16s-7.163444-16-16-16-16 7.163444-16 16c.01046962 8.8322157 7.1677843 15.9895304 16 16zm0-30c7.7319865 0 14 6.2680135 14 14s-6.2680135 14-14 14-14-6.2680135-14-14c.00826732-7.7285595 6.27144053-13.9917327 14-14z"/><path id="Shape" d="m31.745 46.1c-.3683851-.411429-1.0005394-.4463497-1.412-.078-1.3701194 1.2257488-1.7283312 3.2275796-.8682981 4.8523966.860033 1.624817 2.7168434 2.4542127 4.5008701 2.0104382 1.7840268-.4437746 3.0358641-2.0464428 3.034428-3.8848348v-17c0-.5522847-.4477153-1-1-1s-1 .4477153-1 1v17c.0000046.9190868-.6263896 1.7198691-1.5184362 1.9411686-.8920466.2212994-1.8200915-.1938566-2.2496648-1.0063758-.4295733-.8125191-.2500828-1.8132214.435101-2.4257928.4094636-.3681023.444331-.997951.078-1.409z"/><path id="Shape" d="m48 31c-.5522847 0-1 .4477153-1 1v17c-.0000578 1.83802 1.2524832 3.4395253 3.03637 3.8823181 1.7838867.4427928 3.6399451-.3871022 4.4993974-2.011805.8594523-1.6247027.5010748-3.6260133-.8687674-4.8515131-.4121505-.3621698-1.0391465-.3246171-1.4051229.0841571-.3659763.4087742-.3342398 1.0360915.0711229 1.4058429.6851838.6125714.8646743 1.6132737.435101 2.4257928-.4295733.8125192-1.3576182 1.2276752-2.2496648 1.0063758-.8920466-.2212995-1.5184408-1.0220818-1.5184362-1.9411686v-17c0-.5522847-.4477153-1-1-1z"/></g></g></g></svg>)rawliteral";

Settings settings;

void handle_GetSetting()
{
  server.send(200, "application/json", settings.getJson());
}

void handle_SaveSetting()
{
  settings.SetSetting(server);
  server.send(200, "text/html", "ok");
}

void handle_Remote()
{
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

void handle_Temp()
{
  JSONVar response;
  response["Temperature"] = dht.readTemperature();
  response["Humidity"]  =  dht.readHumidity();
  server.send(200, "application/json", JSON.stringify(response).c_str());
}

void handle_OnConnect()
{
  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  server.send_P(200, "text/html", index_html);
}

void handle_Favicon()
{
  server.send_P(200, "image/svg+xml", favicon_svg);
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}
// todo: не работает!!!
void handle_Restart() {
  server.send(200, "text/html", "ok");
  ESP.restart();
}

void setup()
{
  delay(100);
  //Serial.begin(74880);
  pinMode(SET_DEFAULT_PIN, INPUT);
  if (digitalRead(SET_DEFAULT_PIN) == HIGH) {
    settings.SetDefaultSettings();
  }
  pinMode(DHTPin, INPUT);
  pinMode(IR_DIODE_PIN, OUTPUT);
  settings.ReadSettings();
  dht.begin();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig ( settings.getAPIpAddress(), settings.getAPGateway(), settings.getAPSubnet());
  WiFi.softAP(settings.getAPSSID(), settings.getAPPassword());
  server.on("/", handle_OnConnect);
  server.on("/remote", handle_Remote);
  server.on("/restart", handle_Restart);
  server.on("/temp", handle_Temp);
  server.on("/setting", HTTP_GET, handle_GetSetting);
  server.on("/setting", HTTP_POST, handle_SaveSetting);
  server.on("/files/favicon.svg", handle_Favicon);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void loop()
{
  server.handleClient();
}
