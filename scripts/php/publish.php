<?php
require("phpMQTT.php");

$topic = $_GET['topic'];
$color = $_GET['color'];
$clientId = $_GET['clientId'];

$mqtt = new phpMQTT("m10.cloudmqtt.com", 17316, $clientId);
if ($mqtt->connect(true, NULL, "flip-joey", "helloworld")) {
    $mqtt->publish($topic, $color, 0);
    $mqtt->close();
}

?>
