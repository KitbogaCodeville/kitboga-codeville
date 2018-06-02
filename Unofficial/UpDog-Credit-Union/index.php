<?php session_start(); 
$_SESSION['extra'] = '';

?>
<!-- TEST -->
<!doctype html>
<html lang="en"> 
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.1/css/bootstrap.min.css" integrity="sha384-WskhaSGFgHYWDcbwN70/dfYBj47jz9qbsMId/iRN3ewGhXQFZCSftd1LZCfmhktB" crossorigin="anonymous">

	<link rel="stylesheet" href="/css /beststyle2018.css"> 
    <title>Login to your online bank account</title>
	
	<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon">
	<link rel="icon" href="/favicon.ico" type="image/x-icon">
	
	
  </head>
  <body class="dog">
	
	<?php include("header.php") ?>
	
	<div class="container">
	
		<div id="loading">
			<h4>Please Just Wait A Moment...</h4>
		</div>
		
		<div class="jumbotron col-md-6 offset-md-6">
			
			<h2>Welcome to UpDog Financial</h2>
			<h4>We keep for your finances out of the doghouse!</h4>
			<p class="lead">
				To get started with our award winning online banking system simply log in below:
			</p>
			
			<hr />
			
			<form>
				<div class="row">
				
					<div class="col">
					
						<input id="username" class="form-control" type="text" name="user" placeholder="username" />
				
					</div>
					
					<div class="col">
					
						<input class="form-control" id="password" type="password" name="pass" placeholder="password" />
					</div>
				</div>	
			</form>	
			<div class="spacer"></div>
			<button id="login" class="btn btn-success" href="#">Log In</button>
			<button class="btn btn-primary" href="#">Apply Today</button>
			<div class="spacer"></div>
			
			<a href"#">Forgot your username or password?</a>
			<br />
			<small>
				Mircrosoft will never ask you for your banking details.
			</small>
		</div>
		
		<div id="footer">
			&copy; Copyright 2018 This fake bank is intended to waste your time.
		</div>

	</div>
	
	
	
    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js" integrity="sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.1.1/js/bootstrap.min.js" integrity="sha384-smHYKdLADwkXOn1EmN1qk/HfnUcbVRZyYmZ4qpPea6sjB/pTJ0euyQp0Mk8ck+5T" crossorigin="anonymous"></script>
	<script type="text/javascript" src="/script.js"></script>
  </body>
</html>





