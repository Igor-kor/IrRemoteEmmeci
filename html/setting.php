<?php
$fp = fopen("logs/setting.log","a+");
fwrite($fp,implode($_REQUEST)."\n");
echo "ok";
