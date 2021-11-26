
<?php
$mysqli = new mysqli("localhost", "id18011769_mandukogr", "Mundos0804--", "id18011769_los_mosquitos");

$Temperatura = $_POST ['Temperatura'];
$Humedad = $_POST ['Humedad'];
$Luz= $_POST ['Luz'];

$sql = "INSERT INTO Temperature(Temp, Fecha, Hora ) VALUES ('$Temperatura', CURRENT_DATE, CURRENT_TIME);";
$sql .= "INSERT INTO Humidity(Hum, Fecha, Hora ) VALUES ('$Humedad', CURRENT_DATE, CURRENT_TIME);";
$sql .= "INSERT INTO Light(Li, Fecha, Hora ) VALUES ('$Luz', CURRENT_DATE, CURRENT_TIME);";



$mysqli->multi_query($sql);

echo "Datos ingresados correctamente.";
?>
