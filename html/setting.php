<?php
$fp = fopen("logs/setting.log", "a+");
fwrite($fp, $_SERVER['REQUEST_METHOD'] . "\n");
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    fwrite($fp, implode($_GET) . "\n");
    header('Content-Type: application/json');
    echo json_encode(
        array("APSSID" => rand(15, 40),
            "APPassword" => rand(10, 100),
            "APIpAddress" => rand(10, 100),
            "SSID" => rand(10, 100),
            "password" => rand(10, 100),
            "TShift" => rand(10, 100)
        )
    );
}
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    fwrite($fp, implode($_POST) . "\n");
    echo "ok";
}
fclose($fp);

