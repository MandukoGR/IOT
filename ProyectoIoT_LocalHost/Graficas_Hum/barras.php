<?php
    require_once "php/conexion.php";
	$conexion = conexion();
	$sql = "SELECT Hora, Hum from Humidity order by Hora";
	$result = mysqli_query($conexion, $sql);
	$valoresY = array(); // Temp
	$valoresX = array(); // fechas

	while($ver = mysqli_fetch_row($result)){
		$valoresY[]= $ver[1]; 
		$valoresX[]= $ver[0]; 
	}
	$datosX = json_encode($valoresX);
	$datosY = json_encode($valoresY);
?>

<div id = "graficaBarras"></div>
<script type="text/javascript">
	function crearCadenaBarras(json){
		var parsed = JSON.parse(json);
		var arr = [];
		for(var x in parsed){
			arr.push(parsed[x]);
		}
		return arr;
	}
</script>

<script type= "text/javascript">
  datosX = crearCadenaLineal('<?php echo $datosX ?>');
  datosY = crearCadenaLineal('<?php echo $datosY ?>');
  var data = [
  {
    x: datosX,
    y: datosY,
    type: 'bar'
  }
];

Plotly.newPlot('graficaBarras', data);

</script>