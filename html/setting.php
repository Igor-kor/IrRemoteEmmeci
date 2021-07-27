<?php
$fp = fopen("logs/setting.log", "a+");
fwrite($fp, $_SERVER['REQUEST_METHOD'] . "\n");
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    fwrite($fp, implode($_GET) . "\n");
    header('Content-Type: application/json');
    echo' {"APSSID":"esp01","APPassword":"12345678","APIpAddress":"1.0.0.0","APGateway":"1.0.0.0","APSubnet":"1.0.0.0","ClientSSID":"WIFI","ClientPassword":"12345678","TShift":"0","APChannel":"1","APHidden":"0","APMaxConnection":"8"}';

}
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    fwrite($fp, implode($_POST) . "\n");
    echo "ok";
}
fclose($fp);

