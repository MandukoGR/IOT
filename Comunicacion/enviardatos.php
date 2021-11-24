
<?php
$mysqli = new mysqli("localhost", "root", "", "los_mosquitos");

$Temperatura = $_POST ['Temperatura'];
$Humedad = $_POST ['Humedad'];
$Luz= $_POST ['Luz'];

$sql = "INSERT INTO Temperature(Temp, Fecha ) VALUES ('$Temperatura', CURRENT_TIMESTAMP);";
$sql .= "INSERT INTO Humidity(Hum, Fecha ) VALUES ('$Humedad', CURRENT_TIMESTAMP);";
$sql .= "INSERT INTO Light(Li, Fecha ) VALUES ('$Luz', CURRENT_TIMESTAMP);";



$mysqli->multi_query($sql);



echo "Datos ingresados correctamente.";
?>
