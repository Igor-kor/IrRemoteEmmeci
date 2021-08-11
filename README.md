# IR Remote Control Emmeci
![Alt text](https://github.com/Igor-kor/IrRemoteEmmeci/blob/master/img/screenshot1.png?raw=true "Screenshot")
![Alt text](https://github.com/Igor-kor/IrRemoteEmmeci/blob/master/img/screenshot2.png?raw=true "Screenshot")

# Описание

Веб интерфейс управления напоьным кондиционером EMMECI AC-605AE, c использованием ESP01 с ИК диодом, и датчиком DHT11.

# Веб интерфейс

Используются следующие модули для сборки и минификации всего веб интерфейса: 

install node js (https://nodejs.org/en/)

install gulp (https://gulpjs.com/docs/en/getting-started/quick-start/)

```
npm install --global gulp-cli
```

```
npm install --save-dev gulp
```

install minify (https://www.npmjs.com/package/gulp-htmlmin)

```
npm install --save gulp-htmlmin
```

install twig (https://www.npmjs.com/package/gulp-twig)

```
npm install gulp-twig --save
```

install minifyjs (https://www.npmjs.com/package/gulp-minify)

```
npm install --save-dev gulp-minify
```

install minifyjs (https://www.npmjs.com/package/gulp-rename)

```
npm install --save-dev gulp-rename
```

for run 

```
gulp
```

watcher

```
gulp watch
```
Затем необходимо из html/public/emmeci.html
скопировать в 
```cpp
const char index_html[] PROGMEM = R"rawliteral( ... )rawliteral";
```
# В процессе разработки:
- сделать функцию рестарта
- сделать накопление данных о влажности чтобы сгладить и усреднить данные
- сделать подключение как клиента к точке
- указать правильный размер eeprom и переменных
