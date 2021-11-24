
<?php
$mysqli = new mysqli("localhost", "root", "", "tutorial");

$chipid = $_POST ['chipid'];
$temperatura = $_POST ['temperatura'];

$sql = "INSERT INTO tabla (id, chipId, fecha, temperatura) VALUES (NULL, '$chipid', CURRENT_TIMESTAMP, '$temperatura');";

$mysqli->query($sql);

echo "Datos ingresados correctamente.";
?>