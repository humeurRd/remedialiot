<?php

    header('Content-Type: application/json');
    $body = json_decode(file_get_contents("php://input"), true);
    $resp = array();

    $esp = $body['esp'];

    $ar = fopen("json/$esp.json", "w+");

    fwrite($ar, json_encode($body));

    if( $esp != null ){

        $resp["success"] = true;

    }

    echo json_encode($resp);