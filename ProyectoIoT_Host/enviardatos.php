
<?php

$mysqli = new mysqli("sql203.epizy.com", "epiz_30452561", "POqUq6g1Y0QttN", "epiz_30452561_los_mosquitos");
$Temperatura = $_POST ['Temperatura'];
$Humedad = $_POST ['Humedad'];
$Luz= $_POST ['Luz'];

$sql .= "INSERT INTO Temperature(Temp, Fecha, Hora ) VALUES ('$Temperatura', CURRENT_DATE, CURRENT_TIME);";
$sql .= "INSERT INTO Humidity(Hum, Fecha, Hora ) VALUES ('$Humedad', CURRENT_DATE, CURRENT_TIME);";
$sql .= "INSERT INTO Light(Li, Fecha, Hora ) VALUES ('$Luz', CURRENT_DATE, CURRENT_TIME);";
$sql .= "UPDATE Temperature SET Hora = DATE_SUB(Hora, INTERVAL 1 HOUR);";
$sql .= "UPDATE Humidity SET Hora = DATE_SUB(Hora, INTERVAL 1 HOUR);";
$sql .= "UPDATE Light SET Hora = DATE_SUB(Hora, INTERVAL 1 HOUR);";


$mysqli->multi_query($sql);

echo "Datos ingresados correctamente.";

?>
