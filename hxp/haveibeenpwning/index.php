<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>have I been pwning</title>
  </head>
  <body>
	<h1>Have I been pwning</h1>
	<form method="post">
		<input name="flag" placeholder="hxp{...}">
		<input type="submit" value="check">
	</form>



	<h2>Admin Area</h2>
	<a href="download.php">Download flag checker</a>

<?php
	if( isset($_POST['flag']) ){
			if (preg_match('/^hxp\{[a-z0-9_]{1,128}\}$/', $_POST['flag'])){

				exec("/home/ctf/check_flag '".escapeshellarg($_POST['flag'])."'", $output, $retval);
				if($retval === 0) {
					die('pwned!!! please submit');
				}
				else {
					die("likely safe! you haven't been pwning :(");
				}
			}
			else {
				die("no hacks please!");
			}
	}
?>































































































































































































































































































































<!--

DEBUG ACCESS:

-----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
QyNTUxOQAAACDquK/7iBah+2oNfzgOxkzOEyoU1XiD1AZjJL1VU6bt5QAAAJAvoKT4L6Ck
+AAAAAtzc2gtZWQyNTUxOQAAACDquK/7iBah+2oNfzgOxkzOEyoU1XiD1AZjJL1VU6bt5Q
AAAEDtWYU9ArlZT6SD0QhhGaizujbGxsL7qF1HhqQLej0BaOq4r/uIFqH7ag1/OA7GTM4T
KhTVeIPUBmMkvVVTpu3lAAAAC2JiQG5vdGVib29rAQI=
-----END OPENSSH PRIVATE KEY-----

sftp://ctf@ip:2222/

-->
