
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

<div id="graficaLineal"></div>

<script type="text/javascript">
	function crearCadenaLineal(json){
		var parsed = JSON.parse(json);
		var arr = [];
		for(var x in parsed){
			arr.push(parsed[x]);
		}
		return arr;
	}
</script>

<script type="text/javascript">
	datosX = crearCadenaLineal('<?php echo $datosX ?>');
	datosY = crearCadenaLineal('<?php echo $datosY ?>');
	var trace1 = {
		x: datosX,
		y: datosY,
		type: 'scatter'
	};
	
	var data = [trace1];
	Plotly.newPlot('graficaLineal', data);
</script>
