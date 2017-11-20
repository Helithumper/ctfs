<?php

$whitelist = ['127.0.0.1', '::1'];

if( in_array($_SERVER['REMOTE_ADDR'], $whitelist) ){
	$file_url = '/home/ctf/check_flag';
	header('Content-Type: application/octet-stream');
	header('Content-Transfer-Encoding: Binary');
	header('Content-disposition: attachment; filename="check_flag"');
	readfile($file_url);
}
else {
	echo 'permission denied';
}

