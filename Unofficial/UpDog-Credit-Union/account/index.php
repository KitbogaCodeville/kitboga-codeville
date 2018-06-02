<?php session_start(); ?>

<!doctype html>
<html lang="en"> 
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.1/css/bootstrap.min.css" integrity="sha384-WskhaSGFgHYWDcbwN70/dfYBj47jz9qbsMId/iRN3ewGhXQFZCSftd1LZCfmhktB" crossorigin="anonymous">

	<link rel="stylesheet" href="/css /beststyle2018.css"> 
    <title>Your UpDog Account Details</title>
	
	<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon">
	<link rel="icon" href="/favicon.ico" type="image/x-icon">
	
	
  </head>
  <body>
	
	<?php 
	$_SESSION['extra'] =  '<li class="nav-item"><a class="nav-link" href="#">Log Out</a></li>';
	include("../header.php");
	$_SESSION['cc'] = base64_decode($_REQUEST['l']);
	$_SESSION['amount'] = '$' . base64_decode($_REQUEST['a']);
	?>
	
	<div class="container">
	
		<div id="loading">
			<h3>Please Just Wait A Moment...</h3>
		</div>
		
		<div class="alert alert-info" style="margin-top;1em">
			A new branch opened on 69th street next to the Piggly Wiggly. Contact your branch manager today.
		</div>
		
		<div class="alert alert-danger">
			A recent pending transaction for <?php echo $_SESSION['amount']; ?> was flagged as suspicious. To verify please <a href='https://en.wikipedia.org/wiki/Technical_support_scam'>click here</a>
		</div>
		
		
		
		
		<div class="row">
			
			<div class="col-md-8">
				
				<h3>Accounts</h3>
				<hr />
				<table id='accounts' class="table table-striped">
					<thead>
						<tr>
							<th>Type</th>
							<th>Account Number</th>
							<th>Current Balance</th>
							<th>Available Balance</th>
						</tr>
	
					</thead>
					<tbody>
						<tr>
							<td>Personal Checking</td>
							<td>****1337</td>
							<td>$9,337.00</td>
							<td>$1,234.00</td>
						</tr>
	
						<tr>
							<td>Platinum AARP Credit Card</td>
							<td>****<?php echo $_SESSION['cc']; ?></td>
							<td>$420.00</td>
							<td>$24,580.00</td>
						</tr>
	
						<tr>
							<td>Online Savings</td>
							<td>****1337</td>
							<td>$19,293.44</td>
							<td>$15,011.34</td>
						</tr>
	
						<tr>
							<td>Roth IRA</td>
							<td>****1930</td>
							<td>$86,753.09</td>
							<td>$87,482.10</td>
						</tr>
				
						<tr>
							<td>Fixed Rate War Bond</td>
							<td>****6969</td>
							<td>$11,420.00</td>
							<td>$12,492.10</td>
						</tr>
						
					</tbody>
				</table>
			</div>
		
			
				
			
			
			<div class="col-md-4">
				<h3>Pending Transactions</h3>
				<hr />
					<table id='accounts' class="table table-striped">
					<thead>
						<tr>
							<th>Date</th>
							<th>Description</th>
							<th>Amount</th>
							
						</tr>
	
					</thead>
					<tbody>
						<tr>
							<td><?php echo date('m/d'); ?></td>
							<td>APL*ITUNES.COM/BILL</td>
							<td><span class="badge badge-danger">-50.00</td>
						</tr>
						
						<tr>	
							<td><?php echo date('m/d'); ?></td>
							<td>Netflix</td>
							<td><span class="badge badge-danger">-14.99</td>
						</tr>
						
						<tr>
							<td><?php echo date('m/d'); ?></td>
							<td>CandyCrush Checkout</td>
							<td><span class="badge badge-danger">-13.37</td>
						</tr>
						
						<tr>
							<td><?php echo date('m/d'); ?></td>
							<td>Apple iTunes</td>
							<td><span class="badge badge-danger">-50.00</td>
						</tr>
						
						<tr>
							<td><?php echo date('m/d'); ?></td>
							<td>Checque #103 Deposit</td>
							<td><span class="badge badge-success">$50.00</td>
						</tr>
	
						<tr>
							<td><?php echo date('m/d'); ?></td>
							<td>Social Security</td>
							<td><span class="badge badge-success">$1,393.19</td>
						</tr>
						
						<tr>
							<td><?php echo date('m/d', strtotime('-1 day')); ?></td>
							<td>AMZN MARKETPLACE</td>
							<td><span class="badge badge-danger">-$100.00</td>
						</tr>
						
						<tr>
							<td><?php echo date('m/d', strtotime('-1 days')); ?></td>
							<td>WINRAR/BILL</td>
							<td><span class="badge badge-danger">-$30.00</td>
						</tr>
						

						<tr>
							<td><?php echo date('m/d', strtotime('-1 days')); ?></td>
							<td>QVC*SHOPPING BILL</td>
							<td><span class="badge badge-danger">-$140.50.00</td>
						</tr>
						
						
						<tr>
							<td><?php echo date('m/d', strtotime('-2 days')); ?></td>
							<td>Fashion Barn</td>
							<td><span class="badge badge-danger">-$333.50</td>
						</tr>
						
						
						<tr>
							<td><?php echo date('m/d',  strtotime('-4 days')); ?></td>
							<td>McDonald's</td>
							<td><span class="badge badge-danger">-$18.20</td>
						</tr>
					</tbody>
				</table>

			</div>
		
		<div class="row">
					
					<div class="col">
					

						<div class="card">
							<div class="card-body">
								<h4 class="class-title">What's your Credit Score?</h4>
								</p>Find out what your credit score is right now, seriousy it's really easy.</p>
								<a href="#">Learn More</a>
							</div>
						</div>
					
					</div>
					
					<div class="col">
					

						<div class="card">
							<div class="card-body">
								<h4 class="class-title">Join AARP Rewards</h4>
								</p>You could be saving serious coin on everyday purchases.
								</p>
								<a href="#">Learn More</a>
							</div>
						</div>
					
					</div>
					
					<div class="col">
					

						<div class="card">
							<div class="card-body">
								<h4 class="class-title">Record Low Interest Rates</h4>
								</p>You're already approved. It takes just a moment to open an account</p>
								<a href="#">Learn More</a>
							</div>
						</div>
					
					</div>
					
					
					<div class="spacer"></div>
					
					

					<button class="btn btn-warning">Contact Support</button>
					
				</div>
		
		
		
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





