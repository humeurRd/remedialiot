<?php
    $esp = $_POST["esp"];
    $estado = $_POST["estado"];

    if($estado == "on"){
        $estado = "off";
    } else {
        $estado = "on";
    }

    $curl = curl_init();

    curl_setopt_array($curl, array(
    CURLOPT_URL => 'http://pinware.tech/iotResp/execute.php',
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_ENCODING => '',
    CURLOPT_MAXREDIRS => 10,
    CURLOPT_TIMEOUT => 0,
    CURLOPT_FOLLOWLOCATION => true,
    CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
    CURLOPT_CUSTOMREQUEST => 'POST',
    CURLOPT_POSTFIELDS =>'{
        "esp" : "'.$esp.'",
        "estado" : "'.$estado.'"
    }',
    CURLOPT_HTTPHEADER => array(
        'Content-Type: text/plain'
    ),
    ));

    $response = curl_exec($curl);

    curl_close($curl);

    header('Location: http://pinware.tech/iotResp');  
