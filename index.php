<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <?php

    $curl = curl_init();

    curl_setopt_array($curl, array(
    CURLOPT_URL => 'http://pinware.tech/iotResp/json/ESP32-3C61053095C8.json',
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_ENCODING => '',
    CURLOPT_MAXREDIRS => 10,
    CURLOPT_TIMEOUT => 0,
    CURLOPT_FOLLOWLOCATION => true,
    CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
    CURLOPT_CUSTOMREQUEST => 'GET',
    ));

    $response = curl_exec($curl);

    curl_close($curl);

    $nombre = json_decode($response, true)['esp'];
    $estado = json_decode($response, true)['estado'];
    $datum = mktime(date('H') + 0, date('i'), date('s'), date('m'), date('d'), date('y'));
    $uploadDate = date('Y.m.d_H:i:s_', $datum);

    if($estado == "on"){
        $estado = "off";
    }

    if($estado == "off"){
        $estado = "on";
    }

    echo '<form action="newInstruction.php" method="post">';
    echo 'Dispositivo : <input type="text" name="esp" id="esp" value="'.$nombre.'"> </br>';
    echo 'Estado : <input type="text" name="estado" id="estado" value="'.$estado.'"></br>';
    echo 'Fecha: <input type="text" name="fecha" id="fecha" value="'.$uploadDate.'"></br>';
    echo '<input type="submit" value="Enviar">';
    echo '</form>';
    echo "El estado del dispositivo ".$nombre . " es: ". $estado . "Se prendió por última vez : " $uploadDate;

    ?>
    
    
</body>
</html>