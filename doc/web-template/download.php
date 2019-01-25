<?php
session_start();

//check valid
if( empty($_SESSION['userid']) ) {
	echo "The current session is invalid!";
	exit;
}
//timeout
if( $_SESSION['time'] < time() - 3600 ) {
	$_SESSION['userid'] = '';
	echo "The current session expired!";
	exit;
}
$_SESSION['time'] = time();  //update

//file name
$download_id = $_GET['$$DOWNLOAD_ID$$'];
if( empty($download_id) ) {
	echo "The request ID is empty!";
	exit;
}
$$ id, $_SERVER['DOCUMENT_ROOT'], dirname(__FILE__) -> $file_name $$

//download
$file_size = filesize($file_name);
header("Content-type:application/octet-stream");
header("Accept-Length:" . $file_size);
header("Accept-Ranges:bytes");
header("Content-Disposition:attachment;filename='" . basename($file_name) . "'");
$buffer_len = 1024;
$read_count = 0;
$fp = fopen($file_name, "rb");
if( $fp ) {
	while( !feof($fp) and $read_count < $file_size ) {
		$content = fread($fp, $buffer_len);
		echo $content;
		$read_count += $buffer_len;
	}
	fclose($fp);
}
?>
