<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
 <head>
 <!-- file MUST go to wxMsVer/ NOT to wxMS/......
 <!-- CANNOT be encoded in UTF-8 else it causes trouble -->
  <title>wxMs Version</title>
  <meta http-equiv="Content-Type" content="text/html"  />
 </head>
 <body>
 <?php   
// first line of PHP
$defaultTimeZone='UTC'; 
$agent = $_SERVER['HTTP_USER_AGENT'];
// Note file MUST exist
$file = fopen("wxMs.log", "a");   
$ip=$_SERVER['REMOTE_ADDR'];
$dt = date("F j, Y, g:i a");
fputs($file,  "$ip connected $dt, Agent: $agent\n");   
fclose($file);   
?>  
 Latest Version: 0.4.55 </body>
</html>