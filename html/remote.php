<?php
$fp = fopen("logs/remote.log","a+");
fwrite($fp,implode($_REQUEST)."\n");
echo "ok";
