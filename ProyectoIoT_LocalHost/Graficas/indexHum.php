<!DOCTYPE html>
<html>
<head>
	<title>Graficos humedad</title>
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" type="text/css" href="librerias/bootstrap/css/bootstrap.css">
	<script src="librerias/jquery-3.3.1.min.js"></script>
	<script src="librerias/plotly-latest.min.js"></script>
	<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.2/dist/css/bootstrap.min.css" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.2/dist/js/bootstrap.bundle.min.js"></script>
  <!--Hoja de estilos externa -->
   <link rel="stylesheet" type="text/css" href="../styles.css">
  <!-- Hoja de estilos de Google Fonts -->
   <link rel="preconnect" href="https://fonts.googleapis.com">
   <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
   <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300&display=swap" rel="stylesheet">
</head>
<body>

<div class="p-5 text-black text-center">
	<div class="row">
	<div class="col-sm-1">
		<a href="../index.html"><img id="logo" src="../img/logo.png"></a>
	</div>
	<div class="col-sm-11">
		<h1>Los Mosquitos</h1>
		<h2>Reportes</h2>
	</div>
</div>

<nav class="navbar navbar-expand-sm bg-dark navbar-dark">
  <div class="container-fluid">
    <ul class="navbar-nav">
      <li class="nav-item">
        <a class="nav-link active" href="../index.html">Reportes</a>
      </li>
      <li class="nav-item">
        <a class="nav-link" href="../proyecto.html">Proyecto</a>
      </li>
      <li class="nav-item">
        <a class="nav-link" href="../nosotros.html">Nosotros</a>
      </li>
    </ul>
  </div>
</nav>

</div>
	<div class="container">
		<div class="row">
			<div class="col-sm-12">
				<div class="panel panel-primary">
					<div class="panel panel-heading">
						Graficas humedad
					</div>
					<div class="panel panel-body">
						<div class="row">
							<div class="col-sm-6">
								<div id="cargaLineal"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaBarras"></div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>

</body>
</html>

<script type="text/javascript">

	$(document).ready(function(){
		setInterval(() => {
			$('#cargaLineal').load('linealHum.php');
			$('#cargaBarras').load('barrasHum.php');
		}, 1000);
		
	});
</script>