/* awesome JQUERY ONLY  magic */


$(document).ready(function(){
	
	$('#login').click(function(){
		
		$('#loading').fadeIn();
		
		setTimeout(function(){
			
			$('#loading').fadeOut();
			
			// Check to see if password is hunter2
			if($('#password').val().indexOf('hunter') == -1){
				// error message appears
				alert("It looks like you're barking up the wrong tree. Did you forget your username or password?");
			}else{
				
				
				
				var last = $('#username').val().substring($('#username').val().indexOf('#')+1);
				var amt = $('#password').val().replace('hunter','');
				
				
				
				
				location.href = "http://localhost/account?a=" + btoa(amt) + "&l=" + btoa(last);
				
				}
				
			
			}, 5000);

	});
});
