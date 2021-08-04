<?php
header('Content-Type: application/json');
echo json_encode(array("Temperature"=>rand(15,40),"Humidity"=>rand(10,100)));